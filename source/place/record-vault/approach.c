inherit "system/place";

mapping definition() {
  return ([
    "name": "Record Vault Banks",
    "description":
        "Flood scratches at eye level show where the river once reached the archive. A badly "
        + "fitted door faces north, and wax has spilled over its [sill].",
    "details": ([
      "sill":
          "The spilled wax carries the impression of the court's oldest seal. Fresh footprints "
          + "have broken the brittle edge heading north."
    ]),
    "environment": "wilderness",
    "exits": ([
      "out": "place/atlas/x2517/y4377",
      "north": "place/record-vault/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
