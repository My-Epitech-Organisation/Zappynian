from typing import Dict

class WorldState:

    def __init__(self):
        self.inventory: Dict[str, int] = {}
        self.level = 1

    def parse_inventory(self, line: str):
        if not line.startswith("[") or not line.endswith("]"):
            return
        content = line[1:-1].strip()
        self.inventory = {}
        if not content:
            return
        items = content.split(",")
        for item in items:
            parts = item.strip().split()
            if len(parts) == 2:
                name, count = parts
                try:
                    self.inventory[name] = int(count)
                except ValueError:
                    continue

    def get_food_count(self) -> int:
        return self.inventory.get("food", 0)

    def __str__(self):
        return str(self.inventory)
