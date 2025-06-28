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

GOAL_MAP = {
    0: [],
    1: ["Forward"],
    2: ["Forward", "Left", "Forward"],
    3: ["Left", "Forward"],
    4: ["Left", "Left", "Forward", "Right", "Forward"],
    5: ["Left", "Left", "Forward"],
    6: ["Right", "Right", "Forward", "Left", "Forward"],
    7: ["Right", "Forward"],
    8: ["Forward", "Right", "Forward"],
}

def goal_to(tile_index: int) -> list[str]:
    return GOAL_MAP.get(tile_index, ["Look"])
