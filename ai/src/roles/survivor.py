from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

class Survivor(Role):
    def __init__(self):
        super().__init__("Survivor")
        self.target_direction = None

    def decide(self, queue, world, vision):
        if self.target_direction is not None:
            path = route_to(self.target_direction)
            for cmd in path:
                queue.send_and_wait(cmd)
            queue.send_and_wait("Look")
            self.target_direction = None
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

    def on_broadcast(self, message, queue, world, vision):
        parts = message.split(",", 1)
        if len(parts) != 2:
            return
        direction = int(parts[0].split()[1])
        content = parts[1].strip()
        if "incantation" in content.lower():
            self.target_direction = direction
