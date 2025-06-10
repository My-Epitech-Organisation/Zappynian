from typing import Dict

class WorldState:

    def __init__(self):
        # Initialise l'inventaire comme un dictionnaire vide
        self.inventory: Dict[str, int] = {}

    def parse_inventory(self, line: str):
        # Analyse une ligne d'inventaire et met à jour l'inventaire interne
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
        # Retourne la quantité de nourriture dans l'inventaire
        return self.inventory.get("food", 0)

    def __str__(self):
        # Retourne une représentation chaîne de l'inventaire
        return str(self.inventory)
