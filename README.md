# Avelorn

Avelorn is a persistent multiplayer LPMud built on JVMud. It opens at Westwatch, a guest-house governed by Arvenni rules of shelter and obligation. A failed signal lamp leads from a short job on the dunes into the struggle over the occupied island.

The same world serves solo adventurers and groups of up to four. Fighters, Rangers, Mages, and Clerics can all complete the campaign. Each nation has its own settlements, institutions, markets, and adventures. At level 10, players can return for weekly contracts, rotating encounters, and distinctive equipment and keepsakes.

## Play and operate

- [Player quickstart](docs/PLAY.md)
- [Private staging, backup, and deployment](docs/OPERATIONS.md)

Install the pinned JVMud engine from a sibling checkout. Set `JVMUD_ROOT` if the sibling is elsewhere. Use JDK 25 or newer and Maven.

```sh
scripts/avelorn-install-jvmud
mvn test
scripts/avelorn-start
```

Connect a Telnet or GMCP-capable client to the Avelorn player port reported at startup. The staging guide covers an encrypted connection for persistent accounts.

## Build more of the world

[Area authoring](docs/AUTHORING.md) walks through adding a locality. Individual LPC files define authored rooms, inhabitants, equipment, and quests. The area registry attaches them to the [Edition 1 atlas](atlas/tiled/README.md), which supplies terrain between destinations. The [design index](docs/README.md) links the build plan, lore, and historical borders.
