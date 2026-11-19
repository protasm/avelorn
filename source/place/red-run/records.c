inherit "system/place";

mapping definition() {
  return ([
    "name": "Spare Sluice Shed",
    "description":
        "Twine bundles are laid out for repairing cracks as they open. A dry board still has a "
        + "crook for the [lantern] of whichever warden last left the shed.",
    "details": ([
      "lantern":
          "Someone has drawn a circle on the board so the next shift can see at a glance where "
          + "the missing lantern belongs. Inside the circle is the time of the first runoff."
    ]),
    "environment": "interior",
    "exits": ([
      "up": "place/red-run/works"
    ]),
    "services": ({ }),
    "npcs": ({ }),
    "safe": 0
  ]);
}
