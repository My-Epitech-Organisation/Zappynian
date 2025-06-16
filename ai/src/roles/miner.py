from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

class Miner(Role):
    def __init__(self):
        super().__init__("Miner")

    def decide(self, queue, world, vision):
        goals = {
            "linemate": 2,
            "deraumere": 2,
            "sibur": 2,
            "mendiane": 1,
            "phiras": 1,
            "thystame": 1,
        }

        for stone in goals:
            current = world.inventory.get(stone, 0)
            if current < goals[stone]:
                tile = vision.find_nearest(stone)
                path = route_to(tile)
                for cmd in path:
                    queue.push(cmd)
                queue.push(f"Take {stone}")
                break
