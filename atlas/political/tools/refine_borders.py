#!/usr/bin/env python3
"""Derive a reviewable cell-level political overlay from a regional sketch.

Coordinates in data are always native atlas X/east, Y/north. Display rows are
top-down only. Solid draft regions are fixed seeds; only pink regions grow.
Requires NumPy and Pillow. No game files or frozen physical files are edited.
"""

from __future__ import annotations

import argparse
import gzip
import hashlib
import heapq
import json
from collections import Counter
from collections import deque
from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw, ImageFilter, ImageFont

ROOT = Path(__file__).resolve().parents[3]
PHYSICAL = ROOT / 'atlas/qgis/unnamed_new_zealand/export/edition1_256m_atlas'
PALETTE = {1: (46, 116, 222), 2: (231, 157, 25), 3: (31, 187, 180),
           4: (147, 83, 211), 5: (99, 171, 62)}
NAMES = {1: 'Western coast', 2: 'Northern coast', 3: 'Eastern coast',
         4: 'Southern coast', 5: 'Central highlands'}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write_json(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n')


def component(mask, seed):
    """Select four-neighbor spatial membership, independent of walkability."""
    im = Image.fromarray(mask.astype(np.uint8)).copy()
    assert im.getpixel(seed) == 1
    ImageDraw.floodfill(im, seed, 2)
    return np.asarray(im) == 2


def load_physical():
    terrain = np.zeros((5120, 5120), np.uint8)
    elevation = np.zeros_like(terrain, dtype=np.int16)
    catchment = np.zeros_like(terrain, dtype=np.int32)
    manifest = json.loads((PHYSICAL / 'package.json').read_text())
    for entry in manifest['regions']:
        path = PHYSICAL / entry['path']
        assert digest(path) == entry['sha256'], path
        r = json.loads(gzip.decompress(path.read_bytes()))
        a = np.asarray(r['squares'], np.int32)
        x = r['region_x'] * 64 + a[:, 0] % 64
        y = r['region_y'] * 64 + a[:, 0] // 64
        terrain[y, x], elevation[y, x], catchment[y, x] = a[:, 2], a[:, 1], a[:, 3]
    island = component(terrain > 0, (113, 3336))
    y, x = np.where(island)
    bounds = (int(x.min()) // 64, int(y.min()) // 64,
              int(x.max()) // 64, int(y.max()) // 64)
    x0, y0, x1, y1 = bounds
    crop = np.s_[y0*64:(y1+1)*64, x0*64:(x1+1)*64]
    return tuple(a[crop].copy() for a in (terrain, elevation, catchment, island)), bounds


def region_slice(rx, ry, bounds):
    return np.s_[(ry-bounds[1])*64:(ry-bounds[1]+1)*64,
                 (rx-bounds[0])*64:(rx-bounds[0]+1)*64]


def make_constraints(draft, island, bounds):
    fixed = np.zeros(island.shape, np.uint8)
    allowed = np.zeros_like(fixed)
    for r in draft['regions']:
        s = region_slice(r['region_x'], r['region_y'], bounds)
        fixed[s] = r['display_code']
        allowed[s] = sum(1 << n for n in r['candidate_nations'])
    fixed[~island] = 0
    allowed[~island] = 0
    return fixed, allowed


def terrain_cost(terrain, elevation, catchment):
    """Heuristic crossing resistance, not a claim to exact watershed divides.

    Stronger river channels are costly to cross. Locally prominent high ground
    and steep relief also resist expansion. Ordinary ground still has positive
    cost, giving a compact partition where no physical dividing feature exists.
    """
    h = elevation.astype(np.float32)
    # Pillow Gaussian blur uses an 8-bit height proxy only for local prominence.
    proxy = Image.fromarray(np.clip(h / 12, 0, 255).astype(np.uint8))
    smooth = np.asarray(proxy.filter(ImageFilter.GaussianBlur(6)), dtype=float)*12
    ridge = np.maximum(0, h - smooth)
    dy, dx = np.gradient(h)
    cost = 1 + np.minimum(np.hypot(dx, dy) / 12, 12) + np.minimum(ridge / 10, 20)
    # Flow accumulation supplies continuity through wetlands, gorges, and other
    # terrain classes where a channel is not the dominant square classification.
    channel = (terrain == 3) | (catchment >= 10000)
    cost[channel] += 40 + 30*np.maximum(0, np.log10(np.maximum(catchment[channel], 1))-3)
    cost[np.isin(terrain, [1, 2])] += 100
    return np.rint(cost*100).astype(np.int32)


def grow(fixed, allowed, island, cost):
    """Deterministic connected multi-source growth within candidate countries.

    A newly assigned square must touch an already assigned square of that
    country. Stable numeric nation/cell ordering resolves equal costs. Initial
    cores are never changed. Runtime water/cliff movement rules do not apply to
    historical territorial ownership.
    """
    owner = fixed.copy()
    rows, width = owner.shape
    total = owner.size
    o, a, c = owner.ravel(), allowed.ravel(), cost.ravel()
    distances = np.full(total, np.iinfo(np.int64).max, np.int64)
    frontier = island & (fixed > 0)
    unresolved = island & (fixed == 0)
    near = np.zeros_like(island)
    near[1:] |= unresolved[:-1]
    near[:-1] |= unresolved[1:]
    near[:, 1:] |= unresolved[:, :-1]
    near[:, :-1] |= unresolved[:, 1:]
    queue = []
    for cell in np.flatnonzero(frontier & near):
        cell = int(cell)
        distances[cell] = 0
        heapq.heappush(queue, (0, int(o[cell]), cell))
    while queue:
        distance, nation, cell = heapq.heappop(queue)
        if distance != distances[cell] or (o[cell] and o[cell] != nation):
            continue
        o[cell] = nation
        y, x = divmod(cell, width)
        neighbors = []
        if y: neighbors.append(cell-width)
        if y+1 < rows: neighbors.append(cell+width)
        if x: neighbors.append(cell-1)
        if x+1 < width: neighbors.append(cell+1)
        for other in neighbors:
            if o[other] or not (int(a[other]) & (1 << nation)):
                continue
            candidate = distance + (int(c[cell])+int(c[other]))//2
            if candidate < distances[other]:
                distances[other] = candidate
                heapq.heappush(queue, (candidate, nation, other))
    assert np.all(owner[island] > 0), f'{np.count_nonzero(island & (owner == 0))} unreachable squares'
    return owner


def border_mask(owner):
    b = np.zeros_like(owner, dtype=bool)
    v = (owner[1:] > 0) & (owner[:-1] > 0) & (owner[1:] != owner[:-1])
    h = (owner[:, 1:] > 0) & (owner[:, :-1] > 0) & (owner[:, 1:] != owner[:, :-1])
    b[1:] |= v; b[:-1] |= v
    b[:, 1:] |= h; b[:, :-1] |= h
    return b


def boundaries(owner):
    """Group exact shared square edges by neighboring country pair."""
    rows, width = owner.shape
    result = {}
    y, x = np.where((owner[:, 1:] > 0) & (owner[:, :-1] > 0) & (owner[:, 1:] != owner[:, :-1]))
    for yy, xx in zip(y, x):
        yy, xx = int(yy), int(xx)
        pair = tuple(sorted((int(owner[yy, xx]), int(owner[yy, xx+1]))))
        a = yy*(width+1)+xx+1
        result.setdefault(pair, set()).add((a, a+width+1))
    y, x = np.where((owner[1:] > 0) & (owner[:-1] > 0) & (owner[1:] != owner[:-1]))
    for yy, xx in zip(y, x):
        yy, xx = int(yy), int(xx)
        pair = tuple(sorted((int(owner[yy, xx]), int(owner[yy+1, xx]))))
        a = (yy+1)*(width+1)+xx
        result.setdefault(pair, set()).add((a, a+1))
    return result


def edge_cells(a, b, width, rows):
    """Return squares on either side of an edge between corner vertices."""
    y, x = divmod(a, width+1)
    if b-a == 1:
        if y == 0 or y == rows: return None
        return (y-1)*width+x, y*width+x
    if x == 0 or x == width: return None
    return y*width+x-1, y*width+x


def relabel(fixed, island, border_groups):
    """Flood fixed cores through mutable squares without crossing border edges."""
    rows, width = fixed.shape
    blocked = set()
    for edges in border_groups.values():
        for a, b in edges:
            cells = edge_cells(a, b, width, rows)
            if cells: blocked.add(tuple(sorted(cells)))
    owner = fixed.copy(); flat = owner.ravel(); present = island.ravel()
    near = np.zeros_like(island); unknown = island & (fixed == 0)
    near[1:] |= unknown[:-1]; near[:-1] |= unknown[1:]
    near[:, 1:] |= unknown[:, :-1]; near[:, :-1] |= unknown[:, 1:]
    queue = deque(int(x) for x in np.flatnonzero(near & (fixed > 0)))
    while queue:
        cell = queue.popleft(); y, x = divmod(cell, width)
        for other in ((cell-width if y else -1),
                      (cell+width if y+1 < rows else -1),
                      (cell-1 if x else -1),
                      (cell+1 if x+1 < width else -1)):
            if other < 0 or not present[other] or flat[other]: continue
            if (min(cell, other), max(cell, other)) in blocked: continue
            flat[other] = flat[cell]; queue.append(other)
    assert np.all(owner[island]), 'Border route enclosed an unseeded pocket'
    # Every change of nation must be explained by a planned edge, not flood order.
    actual = boundaries(owner)
    assert all(edges <= border_groups.get(pair, set()) for pair, edges in actual.items()), 'Border route failed to separate the fixed cores'
    return owner


def snap_routes(owner, fixed, allowed, island, terrain, elevation, catchment, bounds):
    """Route the separating edges themselves along rivers and prominent relief.

    Each country-pair route retains its coast/junction endpoints. Paths may move
    only inside the pair's existing territory and the original pink corridor;
    fixed same-country interiors and other country boundaries are forbidden.
    A river waypoint in HH12 preserves the explicitly requested northern cap.
    """
    rows, width = owner.shape
    h = elevation.astype(float)
    blurred = np.asarray(Image.fromarray(np.clip(h/12,0,255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(6)), dtype=float)*12
    ridge = np.maximum(h-blurred, 0)
    # Border-routing cost is low on water banks or prominent ridges.
    feature = 30/(1+ridge/18)
    feature = np.maximum(feature, 3)
    feature[terrain == 3] = 1
    feature[np.isin(terrain, [1,2])] = 2
    feature[catchment >= 1000] = np.minimum(feature[catchment >= 1000], 6)
    feature[catchment >= 10000] = 1
    groups = boundaries(owner)
    reports = []
    for pair in [(2,4), (2,5), (1,2), (1,4), (3,5), (4,5), (2,3), (3,4)]:
        groups = boundaries(owner)
        edges = groups.get(pair, set())
        if not edges: continue
        degree = Counter(v for edge in edges for v in edge)
        ends = sorted(v for v,n in degree.items() if n == 1)
        if len(ends) != 2 or any(n > 2 for n in degree.values()):
            reports.append({'pair': pair, 'status': 'retained initial route; junction topology', 'endpoints': len(ends)})
            continue
        flat = owner.ravel(); core = fixed.ravel(); permit = allowed.ravel()
        value = feature.ravel()
        other_vertices = {v for other,e in groups.items() if other != pair for edge in e for v in edge} - set(ends)

        def edge_cost(a, b):
            cells = edge_cells(min(a,b), max(a,b), width, rows)
            if cells is None: return None
            c1,c2 = cells
            if flat[c1] not in pair or flat[c2] not in pair: return None
            if core[c1] and core[c2] and core[c1] == core[c2]: return None
            # Either orientation must be legal under region candidate constraints.
            if not ((int(permit[c1]) & (1<<pair[0]) and int(permit[c2]) & (1<<pair[1]))
                    or (int(permit[c1]) & (1<<pair[1]) and int(permit[c2]) & (1<<pair[0]))): return None
            return int(round(100*min(value[c1],value[c2])))

        def path(start, end, avoid):
            queue=[(0,start)]; distance={start:0}; parent={}
            while queue:
                cost,v=heapq.heappop(queue)
                if cost != distance[v]: continue
                if v == end:
                    out=[v]
                    while v != start: v=parent[v]; out.append(v)
                    return list(reversed(out))
                yy,xx=divmod(v,width+1)
                for u in (v-1 if xx else -1, v+1 if xx<width else -1,
                          v-width-1 if yy else -1, v+width+1 if yy<rows else -1):
                    if u < 0 or u in other_vertices or (u in avoid and u != end): continue
                    step=edge_cost(v,u)
                    if step is None: continue
                    new=cost+step
                    if new < distance.get(u,10**20):
                        distance[u]=new; parent[u]=v; heapq.heappush(queue,(new,u))
            raise ValueError(f'No route for {pair}')

        waypoints=[]
        if pair == (2,5):
            s=region_slice(34,65,bounds)
            yy,xx=np.where(terrain[s] == 3)
            # Choose channel nearest the region center, rather than its downstream edge.
            rank=(yy-32)**2+(xx-32)**2
            i=int(rank.argmin())
            wy=(65-bounds[1])*64+int(yy[i]); wx=(34-bounds[0])*64+int(xx[i])
            waypoint=wy*(width+1)+wx
            waypoints=[waypoint]
        try:
            if waypoints:
                # Reserve the opposite endpoint while reaching the waypoint.
                first=path(ends[0],waypoints[0],{ends[1]})
                second=path(waypoints[0],ends[1],set(first[:-1]))
                route=first+second[1:]
            else:
                route=path(ends[0],ends[1],set())
            replacement={tuple(sorted((a,b))) for a,b in zip(route,route[1:])}
            revised=dict(groups); revised[pair]=replacement
            candidate=relabel(fixed,island,revised)
            # A path must not silently erase other country boundaries.
            actual=boundaries(candidate)
            assert actual.get(pair,set()) == replacement
            owner=candidate
            reports.append({'pair':pair,'status':'terrain-routed','old_edges':len(edges),
                            'new_edges':len(replacement),
                            'waypoints_atlas_corners':[[v%(width+1)+bounds[0]*64,v//(width+1)+bounds[1]*64] for v in waypoints]})
        except (ValueError,AssertionError) as error:
            reports.append({'pair':pair,'status':'retained initial route','reason':str(error)})
    return owner,reports


def validate(owner, fixed, allowed, island, terrain, elevation, bounds, draft):
    assert np.array_equal(owner > 0, island)
    assert np.array_equal(owner[fixed > 0], fixed[fixed > 0])
    assert np.all((allowed[island] & (1 << owner[island])) > 0)
    statistics = {}
    for nation, name in NAMES.items():
        mask = owner == nation
        y, x = np.where(mask)
        connected = component(mask, (int(x[0]), int(y[0])))
        statistics[name] = {'code': nation, 'squares': int(mask.sum()),
                            'area_km2': round(int(mask.sum())*.065536, 3),
                            'outside_first_component': int((mask & ~connected).sum())}
        assert np.array_equal(mask, connected), f'{name} disconnected'
    sea = terrain == 0
    coastal = np.zeros_like(island)
    coastal[1:] |= sea[:-1]; coastal[:-1] |= sea[1:]
    coastal[:, 1:] |= sea[:, :-1]; coastal[:, :-1] |= sea[:, 1:]
    assert not np.any(coastal & (owner == 5)), 'Central country touches ocean'
    for nation in range(1, 5):
        assert np.any(coastal & (owner == nation)), f'Country {nation} lacks coast'
    # Seed the actual contiguous lake, not merely the enclosing region rectangle.
    lake_area = np.zeros_like(island)
    lake_area[(59-bounds[1])*64:(62-bounds[1])*64,
              (34-bounds[0])*64:(36-bounds[0])*64] = True
    open_water = (terrain == 1) & lake_area
    ly, lx = np.where(open_water)
    lake = component(np.isin(terrain, [1, 2]), (int(lx[len(lx)//2]), int(ly[len(ly)//2])))
    assert np.all(owner[lake] == 5), 'Great Lake ownership split'
    summit_y, summit_x = 3723-bounds[1]*64, 2416-bounds[0]*64
    assert elevation[summit_y, summit_x] == 2651
    assert owner[summit_y, summit_x] == 5
    boundary_regions = []
    for r in draft['regions']:
        if r['display_code']:
            continue
        values, counts = np.unique(owner[region_slice(r['region_x'], r['region_y'], bounds)], return_counts=True)
        boundary_regions.append({'region_x': r['region_x'], 'region_y': r['region_y'],
                                 'nation_squares': {str(int(n)): int(c) for n, c in zip(values, counts) if n}})
    return {'coverage_squares': int(island.sum()), 'countries': statistics,
            'fixed_region_assignments_preserved': True, 'candidate_constraints_respected': True,
            'central_landlocked': True, 'great_lake_entirely_central': True,
            'volcanic_summit_central': True, 'boundary_regions': boundary_regions}


def runs(indices):
    result = []
    for value in indices:
        value = int(value)
        if result and result[-1][0]+result[-1][1] == value:
            result[-1][1] += 1
        else:
            result.append([value, 1])
    return result


def export(owner, bounds, draft, draft_path, output, qa):
    manifest = {'schema': 1, 'era': 'pre-conquest', 'status': 'cell-border-draft-01; for review',
                'physical_package_sha256': digest(PHYSICAL/'package.json'),
                'builder_sha256': digest(Path(__file__)),
                'regional_draft': str(draft_path.relative_to(ROOT)), 'regional_draft_sha256': digest(draft_path),
                'square_size_m': 256, 'region_axis_squares': 64,
                'coordinates': 'zero-based atlas x east, y north; local_index = local_y * 64 + local_x',
                'display_columns': draft['display_columns'], 'display_rows': draft['display_rows'],
                'nations': [{'code': n, 'id': name.lower().replace(' ', '-'), 'name': name} for n, name in NAMES.items()],
                'regions': []}
    decoded = np.zeros_like(owner)
    for r in draft['regions']:
        rx, ry = r['region_x'], r['region_y']
        block = owner[region_slice(rx, ry, bounds)].ravel()
        included = np.flatnonzero(block)
        default = Counter(int(x) for x in block[included]).most_common(1)[0][0]
        overrides = [[int(i), int(block[i])] for i in included if block[i] != default]
        record = {'schema': 1, 'region_x': rx, 'region_y': ry, 'default_nation': default,
                  'coverage_runs': runs(included), 'overrides': overrides}
        relative = f'regions/x{rx:02d}/r{rx:02d}_{ry:02d}.json.gz'
        path = output/relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(gzip.compress(json.dumps(record, separators=(',', ':')).encode(), mtime=0))
        # Decode the actual serialized output independently of the source block.
        reread = json.loads(gzip.decompress(path.read_bytes()))
        restored = np.zeros(4096, np.uint8)
        for start, length in reread['coverage_runs']:
            assert start >= 0 and length > 0 and start+length <= 4096
            assert not restored[start:start+length].any()
            restored[start:start+length] = reread['default_nation']
        for index, nation in reread['overrides']:
            assert restored[index] > 0 and nation in NAMES
            restored[index] = nation
        decoded[region_slice(rx, ry, bounds)] = restored.reshape((64, 64))
        manifest['regions'].append({'region_x': rx, 'region_y': ry, 'path': relative,
                                    'sha256': digest(path), 'square_count': len(included),
                                    'default_nation': default, 'override_count': len(overrides)})
    assert np.array_equal(owner, decoded), 'Serialized roundtrip differs'
    border_groups = boundaries(decoded)
    segments = []
    width = decoded.shape[1]
    for pair, edges in sorted(border_groups.items()):
        for a, b in sorted(edges):
            ay, ax = divmod(a, width+1); by, bx = divmod(b, width+1)
            segments.append([ax+bounds[0]*64, ay+bounds[1]*64,
                             bx+bounds[0]*64, by+bounds[1]*64, *pair])
    write_json(output/'borders.json', {'schema': 1, 'derived_from': 'region ownership records',
                                      'coordinates': 'native atlas square corners; x east, y north',
                                      'segment_fields': ['x1','y1','x2','y2','nation_a','nation_b'],
                                      'segments': segments})
    manifest['borders'] = {'path': 'borders.json', 'sha256': digest(output/'borders.json'),
                           'segment_count': len(segments)}
    qa['shared_border_edges'] = len(segments)
    qa['shared_border_length_km'] = round(len(segments)*.256,3)
    qa['split_regions'] = sum(r['override_count'] > 0 for r in manifest['regions'])
    qa['uniform_regions'] = len(manifest['regions'])-qa['split_regions']
    qa['serialized_roundtrip_exact'] = True
    qa['region_files'] = len(manifest['regions'])
    write_json(output/'package.json', manifest)
    write_json(output/'validation.json', qa)


def render(owner, terrain, elevation, catchment, bounds, output):
    m = owner > 0
    h = elevation.astype(float)
    stops = [0, 200, 600, 1200, 1800, 2700]
    colors = np.array([[164,187,134],[149,173,121],[179,177,131],[177,153,121],[153,139,131],[241,238,226]])
    rgb = np.stack([np.interp(h, stops, colors[:, c]) for c in range(3)], axis=-1)
    dy, dx = np.gradient(h)
    rgb *= np.clip(1+(-dx-dy)/700, .72, 1.18)[:, :, None]
    for n, color in PALETTE.items():
        rgb[owner == n] = rgb[owner == n]*.65 + np.array(color)*.35
    rgb[m & np.isin(terrain, [1,2,3])] = [65,135,165]
    rgb[m & (catchment >= 10000)] = [65,135,165]
    rgb[~m] = [224,236,238]
    # Draw exact shared cell edges, not a smoothed vector approximation.
    edge_overlay = Image.new('RGBA', (owner.shape[1]+1, owner.shape[0]+1))
    e = ImageDraw.Draw(edge_overlay)
    edges = []
    y, x = np.where((owner[:,1:] > 0) & (owner[:,:-1] > 0) & (owner[:,1:] != owner[:,:-1]))
    for yy, xx in zip(y, x): edges.append((int(xx+1), int(yy), int(xx+1), int(yy+1)))
    y, x = np.where((owner[1:] > 0) & (owner[:-1] > 0) & (owner[1:] != owner[:-1]))
    for yy, xx in zip(y, x): edges.append((int(xx), int(yy+1), int(xx+1), int(yy+1)))
    for segment in edges: e.line(segment, fill=(255,250,235,240), width=3)
    for segment in edges: e.line(segment, fill=(73,48,50,255), width=1)
    base = Image.fromarray(np.clip(rgb,0,255).astype(np.uint8)).convert('RGBA')
    raw_base = base.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
    base = Image.alpha_composite(base, edge_overlay.crop((0,0,base.width,base.height)))
    base = base.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
    fonts = Path('/System/Library/Fonts/Supplemental')
    regular = fonts/'Arial.ttf'; bold = fonts/'Arial Bold.ttf'
    font = lambda n: ImageFont.truetype(str(regular), n)
    bfont = lambda n: ImageFont.truetype(str(bold), n)
    left, top = 104, 160
    w, hh = base.size
    canvas = Image.new('RGB', (w+204, hh+315), '#f5f3ec')
    canvas.paste(base.convert('RGB'), (left, top))
    d = ImageDraw.Draw(canvas)
    d.text((left,28),'AVELORN / FIVE NATIONS — CELL BORDER DRAFT 01',font=bfont(37),fill='#243d40')
    d.text((left,80),'Individual 256 m squares · Terrain-guided first pass within Draft 05 boundary regions · Lines follow shared cell edges',font=font(24),fill='#41565a')
    overlay = Image.new('RGBA',canvas.size); g=ImageDraw.Draw(overlay)
    for i in range(bounds[2]-bounds[0]+2):
        px=left+i*64; g.line((px,top,px,top+hh),fill=(31,56,61,65))
    for j in range(bounds[3]-bounds[1]+2):
        py=top+j*64; g.line((left,py,left+w,py),fill=(31,56,61,65))
    canvas=Image.alpha_composite(canvas.convert('RGBA'),overlay).convert('RGB'); d=ImageDraw.Draw(canvas)
    for rx in range(bounds[0],bounds[2]+1):
        label=chr(65+(rx-1)%26)*(1+(rx-1)//26)
        for py in (top-22,top+hh+22): d.text((left+(rx-bounds[0])*64+32,py),label,font=bfont(18),fill='#243d40',anchor='mm')
    for ry in range(bounds[1],bounds[3]+1):
        py=top+(bounds[3]-ry)*64+32
        for px in (left-27,left+w+27): d.text((px,py),str(77-ry),font=bfont(18),fill='#243d40',anchor='mm')
    d.text((left+w-25,45),'N ↑',font=bfont(27),fill='#243d40',anchor='rm')
    for title,xx,yy in [('WESTERN COAST',14,57.7),('NORTHERN COAST',34.5,71.2),('EASTERN COAST',46,58.6),('SOUTHERN COAST',36,52.4),('CENTRAL HIGHLANDS',35.5,62.3),('Great Lake',34,60.5),('Volcanic mountain',38.5,58)]:
        p=(left+(xx-bounds[0])*64,top+(bounds[3]+1-yy)*64)
        d.text(p,title,font=bfont(22 if title[0].islower() or title in ('Great Lake','Volcanic mountain') else 26),fill='#26373b',anchor='mm',stroke_width=2,stroke_fill='#f5f3ec')
    for i,n in enumerate(NAMES):
        px=left+i*540; py=top+hh+65
        d.rectangle((px,py,px+28,py+24),fill=PALETTE[n])
        d.text((px+40,py),NAMES[n],font=bfont(22),fill='#243d40')
    d.text((left,top+hh+106),'Review draft · Region edges: 16.384 km · Columns A–Z, AA, BB … ZZ · Rows 1–29 downward · Political data stored separately from physical atlas',font=font(21),fill='#41565a')
    canvas.save(output/'north-island-pre-conquest-cell-borders-draft-01.png')
    # Two enlargements expose the requested river corridor and northern cap.
    for name,box in [('river-corridor',(29,59,34,63)), ('northern-border',(31,63,38,67))]:
        ax,ay,bx,by=box
        crop=raw_base.crop(((ax-bounds[0])*64,(bounds[3]+1-by)*64,(bx-bounds[0])*64,(bounds[3]+1-ay)*64))
        scale=5
        crop=crop.resize((crop.width*scale,crop.height*scale),Image.Resampling.NEAREST).convert('RGB')
        panel=Image.new('RGB',(crop.width+160,crop.height+200),'#f5f3ec'); panel.paste(crop,(80,110))
        draw=ImageDraw.Draw(panel)
        draw.text((80,25),name.replace('-',' ').title()+' / 256 m cells',font=bfont(29),fill='#243d40')
        for rx in range(ax,bx):
            lab=chr(65+(rx-1)%26)*(1+(rx-1)//26)
            draw.text((80+(rx-ax+.5)*64*scale,83),lab,font=bfont(21),fill='#243d40',anchor='mm')
        for ry in range(ay,by):
            draw.text((43,110+(by-ry-.5)*64*scale),str(77-ry),font=bfont(21),fill='#243d40',anchor='mm')
        grids=Image.new('RGBA',panel.size); gg=ImageDraw.Draw(grids)
        for xx in range(0,crop.width+1,scale): gg.line((80+xx,110,80+xx,110+crop.height),fill=(25,50,50,22),width=1)
        for yy in range(0,crop.height+1,scale): gg.line((80,110+yy,80+crop.width,110+yy),fill=(25,50,50,22),width=1)
        for xx in range(0,crop.width+1,64*scale): gg.line((80+xx,110,80+xx,110+crop.height),fill=(25,50,50,130),width=2)
        for yy in range(0,crop.height+1,64*scale): gg.line((80,110+yy,80+crop.width,110+yy),fill=(25,50,50,130),width=2)
        panel=Image.alpha_composite(panel.convert('RGBA'),grids).convert('RGB')
        pd=ImageDraw.Draw(panel)
        detail_edges=[]
        for x1,y1,x2,y2 in edges:
            world_x1=x1+bounds[0]*64; world_x2=x2+bounds[0]*64
            world_y1=y1+bounds[1]*64; world_y2=y2+bounds[1]*64
            if min(world_x1,world_x2)<ax*64 or max(world_x1,world_x2)>bx*64: continue
            if min(world_y1,world_y2)<ay*64 or max(world_y1,world_y2)>by*64: continue
            detail_edges.append((80+(world_x1-ax*64)*scale,110+(by*64-world_y1)*scale,
                                 80+(world_x2-ax*64)*scale,110+(by*64-world_y2)*scale))
        for edge in detail_edges: pd.line(edge,fill='#fff9eb',width=4)
        for edge in detail_edges: pd.line(edge,fill='#493032',width=2)
        ImageDraw.Draw(panel).text((80,panel.height-55),'Dark line: border · Blue: water / major flow paths · Fine grid: 256 m · Heavy grid: regions',font=font(21),fill='#41565a')
        panel.save(output/f'north-island-cell-borders-{name}-draft-01.png')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--draft',type=Path,default=ROOT/'output/maps/north-island-pre-conquest-ownership-draft-05.json')
    parser.add_argument('--output',type=Path,default=ROOT/'atlas/political/pre-conquest/cell-draft-01')
    parser.add_argument('--maps',type=Path,default=ROOT/'output/maps')
    args=parser.parse_args()
    draft=json.loads(args.draft.read_text())
    print('Reading and checking frozen atlas JSON...',flush=True)
    (terrain,elevation,catchment,island),bounds=load_physical()
    fixed,allowed=make_constraints(draft,island,bounds)
    # EE17 is the triple-junction boundary region: permit the northern country
    # to meet the river here rather than force its edge onto the region grid.
    allowed[region_slice(31,60,bounds)] |= np.uint8(1 << 2)
    allowed[~island]=0
    print(f'Refining {np.count_nonzero(island & (fixed==0)):,} squares in boundary regions...',flush=True)
    owner=grow(fixed,allowed,island,terrain_cost(terrain,elevation,catchment))
    print('Routing boundary edges along rivers and relief...',flush=True)
    owner,route_reports=snap_routes(owner,fixed,allowed,island,terrain,elevation,catchment,bounds)
    qa=validate(owner,fixed,allowed,island,terrain,elevation,bounds,draft)
    qa['route_refinement']=route_reports
    qa['candidate_adjustments']=[{'region_x':31,'region_y':60,'display_region':'EE17','added_nation':2,'reason':'three-country river junction; boundary region only'}]
    print(json.dumps(route_reports,indent=2),flush=True)
    print(json.dumps(qa['countries'],indent=2),flush=True)
    export(owner,bounds,draft,args.draft,args.output,qa)
    args.maps.mkdir(parents=True,exist_ok=True)
    render(owner,terrain,elevation,catchment,bounds,args.maps)
    print(f'Saved political overlay: {args.output}',flush=True)
    print(f'Saved maps: {args.maps}',flush=True)


if __name__=='__main__':
    main()
