from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

GOALS = {
    "linemate": 2,
    "deraumere": 2,
    "sibur": 2,
    "mendiane": 1,
    "phiras": 1,
    "thystame": 1,
}

class Miner(Role):
    def __init__(self):
        super().__init__("Miner")

    def decide(self, queue, world, vision):
        print("[Miner]")
        vision_data = queue.send_and_wait("Look")
        vision.parse_look(vision_data)
        inv_data = queue.send_and_wait("Inventory")
        world.parse_inventory(inv_data)
        if world.get_food_count() < 5:
            return
        for stone, goal in GOALS.items():
            if world.inventory.get(stone, 0) < goal:
                tile = vision.find_nearest(stone)
                if tile == -1:
                    queue.send_and_wait("Right")
                    queue.send_and_wait("Forward")
                    queue.send_and_wait("Left")
                    return
                path = route_to(tile)
                for cmd in path:
                    queue.send_and_wait(cmd)
                queue.send_and_wait(f"Take {stone}")
                return
