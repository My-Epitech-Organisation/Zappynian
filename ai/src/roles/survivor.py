from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

class Survivor(Role):
    def __init__(self):
        super().__init__("Survivor")

    def decide(self, queue, world, vision):
        food_tile = vision.find_nearest("food")
        if food_tile != -1:
            path = route_to(food_tile)
            for cmd in path:
                queue.send_and_wait(cmd)
            queue.send_and_wait("Take food")
        else:
            queue.send_and_wait("Right")
            queue.send_and_wait("Forward")
            queue.send_and_wait("Left")
            queue.send_and_wait("Forward")
