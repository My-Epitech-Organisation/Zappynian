DIRECTION_MAP = {
    0: [],
    1: ["Forward", "Left", "Forward"],
    2: ["Forward"],
    3: ["Forward", "Right", "Forward"],
    4: ["Forward", "Forward", "Left", "Forward", "Forward"],
    5: ["Forward", "Forward", "Left", "Forward"],
    6: ["Forward", "Forward"],
    7: ["Forward", "Forward", "Right", "Forward"],
    8: ["Forward", "Forward", "Right", "Forward", "Forward"],
}

def route_to(tile_index: int) -> list[str]:
    return DIRECTION_MAP.get(tile_index, ["Look"])
