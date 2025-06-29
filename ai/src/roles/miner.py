from ai.src.utils.route_factory import route_to
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

def miner(queue, world, vision) -> int:
    level = world.level
    goals = INCANTATION_REQUIREMENTS.get(level, {})
    for stone, goal in goals.items():
        if stone == "players":
            continue
        if world.inventory.get(stone, 0) < goal:
            tile = vision.find_nearest(stone)
            if tile == -1:
                return 1
            path = route_to(tile)
            for cmd in path:
                queue.send_and_wait(cmd)
            queue.send_and_wait(f"Take {stone}")
            return 0
    else:
        return 1
