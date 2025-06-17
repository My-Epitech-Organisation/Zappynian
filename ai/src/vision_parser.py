from typing import List

class Vision:

    def __init__(self):
        self.tiles: List[List[str]] = []

    def parse_look(self, line: str):
        if not line.startswith("[") or not line.endswith("]"):
            return
        content = line[1:-1].strip()
        self.tiles.clear()
        if not content:
            return
        raw_tiles = content.split(",")
        for raw in raw_tiles:
            items = raw.strip().split()
            self.tiles.append(items)

    def get_tile(self, index: int) -> List[str]:
        if 0 <= index < len(self.tiles):
            return self.tiles[index]
        return []

    def find_nearest(self, target: str) -> int:
        for i, tile in enumerate(self.tiles):
            if target in tile:
                return i
        return -1

    def __str__(self):
        return str(self.tiles)
