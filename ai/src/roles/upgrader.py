from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

UPGRADEGOALS = {
    "linemate": 2,
    "deraumere": 2,
    "sibur": 2,
    "mendiane": 2,
    "phiras": 2,
    "thystame": 2,
}

class Upgrader(Role):
    def __init__(self):
        super().__init__("Upgrader")

    def decide(self, queue, world, vision):
        vision_data = queue.send_and_wait("Look")
        vision.parse_look(vision_data)
        inv_data = queue.send_and_wait("Inventory")
        world.parse_inventory(inv_data)
        if world.get_food_count() < 5:
            return
        for stone, goal in UPGRADEGOALS.items():
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
