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
        vision_data = queue.send_and_wait("Look")
        vision.parse_look(vision_data)
        inv_data = queue.send_and_wait("Inventory")
        world.parse_inventory(inv_data)
        for stone, goal in goals.items():
            current = world.inventory.get(stone, 0)
            if current < goal:
                tile = vision.find_nearest(stone)
                if tile is not None:
                    path = route_to(tile)
                    for cmd in path:
                        queue.send_and_wait(cmd)
                    queue.send_and_wait(f"Take {stone}")
                else:
                    queue.send_and_wait("Forward")
                    queue.send_and_wait("Forward")
                    queue.send_and_wait("Forward")
                return
