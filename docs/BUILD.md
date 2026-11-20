# Build Avelorn 1.0

Avelorn is a persistent, shared LPMud set a century after the Conquest. Solo players and parties of up to four use the same world. The opening at Westwatch should yield a fight, quest reward, or safe checkpoint in ten minutes. The longer campaign carries players through four coastal traditions, the Concordance, and the mine beneath the Durnathi volcano.

## Authoring

- The five nations, shared history, and frozen physical and historical atlas give new places their setting. Each locality gets its own LPC area file, with a stable atlas entrance and optional trails and staffed travel links. Use geography, livelihood, and the local institution to explain what the place does and what brings the player there.
- Author rooms, creatures, NPCs, items, and quests in individual LPC files with short names. Group them by locality or by item category. Reuse LPC bases for ordinary behavior. Preserve Place paths, GMCP IDs, blueprint IDs, and quest IDs. The compressed JSON atlas supplies the ground between authored places.
- Open a room description with one or two short paragraphs. Let the shared formatter wrap to each player's ruler width and hyphenate unbroken text when it must split. The default width is 60 columns. Introduce inspectable scenery naturally, using markers such as `[lamp]` in the LPC source and a corresponding `details` entry. Save extra description, clues, and clues to local actions for examination. Every scenery response needs a visible label in the current room. Players start with plain prose. The `details` command lists scenery, and `hints on` and `hints off` control bracket markers. Brief mode keeps a compact detail line.
- Verify expansion by adding a sample village and cave through the LPC registry, walking from an established stop to both, completing the new objective, and restoring a character saved before the additions.
- Extend character and item systems through LPC definitions, too. Classes supply their starter kit, attributes, resource, styles, and techniques. New techniques can use a shared effect or call their own LPC behavior. Item files can declare additional equipment slots and implement use. Race and guild registries hold independent identities and rules, and the advancement table controls the cap and thresholds. Keep IDs and earlier level costs stable as new entries are added. Test extensions in an isolated copy alongside an existing character save.

## Play

- Ship five city hubs, fifteen smaller settlements, at least ten adventure sites, forty authored quests, and ten repeatable contracts. Give players working markets, inns, trainers, and travel services. Show the wider civilization through trade goods, occupations, civic institutions, and local disputes.
- Every player character is Human, with equivalent male and female choices. Fighter, Ranger, Mage, and Cleric each have three techniques and two styles. Keep level 10 as the cap, with rotating contracts, expedition variants, equipment sidegrades, and cosmetic rewards there. Let every class finish the campaign alone, with NPC practitioners available for the Concordance. Support party participation and individual quest credit in the same shared world.
- Free services link Westwatch and the major hubs in at most three legs, each at most two minutes. Mark walking circuits with discoveries or choices. Wilderness reserve starts at 100, with successful moves costing one, two, or four by terrain. Inns replenish it. Camping for 15 seconds spends one ration and restores 40. Exhaustion slows wilderness steps to one every three seconds.
- Process at most six commands per second from a 16-command queue. Cap queued movement at eight steps and allow one wilderness step per second, or four entirely protected steps. Keep settled and indoor travel free. Resolve exits when each move executes. Keep `stop` and `quit` available through a paste.

## Release gates

- Verify the opener and the full campaign with each class solo and with two- and four-player parties. Exercise Telnet and GMCP, the scenery hooks with hints off and on, and cap-level activities. Check both directions of the match between visible scenery markers and examination responses.
- Test the 10,000-line paste with a second player connected. Verify one authenticated session per account, legacy migration, durable quest claims, alternating filesystem saves, transit recovery, and bounded use of atlas rooms. Run the supervised private service for 24 hours with up to 32 clients and verify it recovers from a worker failure.
- Deliver the tested package, player quickstart, area authoring guide, and operator runbook. Verify an encrypted staging connection and rehearse backup, restore, and rollback.
