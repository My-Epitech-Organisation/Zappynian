from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to
import random

class Scout(Role):
    def __init__(self):
        super().__init__("Scout")

    def decide(self, queue, world, vision):
        tile = random.randint(1, 8)

        path = route_to(tile)
        for cmd in path:
            queue.push(cmd)
