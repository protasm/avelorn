#!/usr/bin/env python3
"""Validate every region header and sample chunk NBT without modifying the save.

Checks exact rectangular chunk coverage, offsets, overlap and compression, then
reads a representative chunk in every region plus source-based landmarks. This
is a structural/sample check, not a claim to have played every square.
"""
import gzip
import io
import json
import math
from pathlib import Path
import struct
import zlib

ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/"output/minecraft"
SAVE=OUT/"saves/Avelorn - North Island (4x)"
REGION=SAVE/"dimensions/minecraft/overworld/region"


def nbt(data):
    f=io.BytesIO(data)
    def read(fmt):
        return struct.unpack(fmt,f.read(struct.calcsize(fmt)))[0]
    def string():
        return f.read(read(">H")).decode("utf-8")
    def payload(t):
        if t in range(1,7): return read({1:">b",2:">h",3:">i",4:">q",5:">f",6:">d"}[t])
        if t==7: return f.read(read(">i"))
        if t==8: return string()
        if t==9:
            subtype=read(">B"); count=read(">i")
            return [payload(subtype) for _ in range(count)]
        if t==10:
            result={}
            while True:
                subtype=read(">B")
                if subtype==0: return result
                name=string(); result[name]=payload(subtype)
        if t in (11,12):
            count=read(">i"); fmt=">i" if t==11 else ">q"
            return [read(fmt) for _ in range(count)]
        raise ValueError(f"Unknown NBT type {t}")
    kind=read(">B"); string()
    return payload(kind)


def read_chunk(path,lx,lz):
    with path.open("rb") as f:
        f.seek((lx+lz*32)*4)
        raw=int.from_bytes(f.read(4),"big"); offset=raw>>8
        assert offset, (path,lx,lz,"missing chunk")
        f.seek(offset*4096)
        size=int.from_bytes(f.read(4),"big"); compression=f.read(1)[0]
        data=f.read(size-1)
    if compression==2: data=zlib.decompress(data)
    elif compression==1: data=gzip.decompress(data)
    elif compression!=3: raise ValueError(f"Unexpected compression {compression}")
    return nbt(data)


def block_at(chunk,x,y,z):
    for section in chunk["sections"]:
        if section["Y"]!=y//16: continue
        states=section["block_states"]; palette=states["palette"]
        if len(palette)==1: return palette[0]["Name"]
        bits=max(4,(len(palette)-1).bit_length()); per_long=64//bits
        idx=(y%16)*256+(z%16)*16+x%16
        value=(states["data"][idx//per_long]>>(idx%per_long*bits))&((1<<bits)-1)
        return palette[value]["Name"]
    return "minecraft:air"


def main():
    manifest=json.loads((OUT/"build-manifest.json").read_text())
    w,h=manifest["dimensions_blocks"]; cw,ch=w//16,h//16
    files=list(REGION.glob("r.*.*.mca"))
    assert len(files)==math.ceil(cw/32)*math.ceil(ch/32),len(files)
    chunks=0; samples=[]
    for path in sorted(files):
        _,rx,rz,_=path.name.split("."); rx,rz=int(rx),int(rz)
        assert 0<=rx<math.ceil(cw/32) and 0<=rz<math.ceil(ch/32)
        with path.open("rb") as f: header=f.read(8192)
        assert len(header)==8192
        occupied=set([0,1]); present=[]; total_sectors=path.stat().st_size//4096
        for index in range(1024):
            raw=int.from_bytes(header[index*4:index*4+4],"big")
            lx,lz=index%32,index//32
            expected=rx*32+lx<cw and rz*32+lz<ch
            assert bool(raw)==expected,(path,lx,lz,"coverage")
            if not raw: continue
            offset,count=raw>>8,raw&255
            assert count>0 and offset>=2 and offset+count<=total_sectors
            sectors=set(range(offset,offset+count)); assert not sectors&occupied
            occupied.update(sectors); present.append((lx,lz)); chunks+=1
        lx,lz=present[len(present)//2]
        chunk=read_chunk(path,lx,lz)
        assert chunk["xPos"]==rx*32+lx and chunk["zPos"]==rz*32+lz
        assert chunk["Status"] in ("minecraft:full","full"),chunk["Status"]
        assert chunk["sections"]
        samples.append({"region":path.name,"data_version":chunk["DataVersion"]})
    assert chunks==cw*ch,(chunks,cw*ch)
    level=nbt(gzip.decompress((SAVE/"level.dat").read_bytes()))
    data=level.get("Data",level)
    landmarks=[]
    for point in json.loads((OUT/"landmarks.json").read_text()):
        label,x,z=point["name"],point["x"],point["z"]
        cx,cz=x//16,z//16
        chunk=read_chunk(REGION/f"r.{cx//32}.{cz//32}.mca",cx%32,cz%32)
        solids=[y for y in range(-64,320) if block_at(chunk,x,y,z) not in ("minecraft:air","minecraft:cave_air","minecraft:water")]
        top=max(solids)
        landmarks.append({"name":label,"x":x,"z":z,"top_y":top,"block":block_at(chunk,x,top,z)})
    report={"region_files":len(files),"chunks":chunks,"expected_chunks":cw*ch,
        "every_region_header_valid":True,"sampled_chunks":len(samples)+len(landmarks),
        "data_versions":sorted(set(s["data_version"] for s in samples)),
        "level":data,"landmarks":landmarks,
        "limits":"All region headers checked; one chunk per region plus landmarks decoded. Live Minecraft check recorded separately."}
    (OUT/"validation.json").write_text(json.dumps(report,indent=2,default=str)+"\n")
    print(json.dumps({k:v for k,v in report.items() if k!="level"},indent=2))


if __name__=="__main__": main()
