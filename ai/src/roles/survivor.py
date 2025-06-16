from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

class Survivor(Role):
    def __init__(self):
        super().__init__("Survivor")

    def decide(self, queue, world, vision):
        tile = vision.find_nearest("food")

        path = route_to(tile)
        for cmd in path:
            queue.push(cmd)
        queue.push("Take food")
