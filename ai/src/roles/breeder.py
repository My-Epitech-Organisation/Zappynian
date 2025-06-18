from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import route_to

class Breeder(Role):
    def __init__(self):
        super().__init__("Breeder")
        self.target_direction = None

    def decide(self, queue, world, vision):
        if self.target_direction is not None:
            path = route_to(self.target_direction)
            for cmd in path:
                queue.send_and_wait(cmd)
            queue.send_and_wait("Look")
            self.target_direction = None
        queue.send_and_wait("Fork")

    def on_broadcast(self, message, queue, world, vision):
        parts = message.split(",", 1)
        if len(parts) != 2:
            return
        direction = int(parts[0].split()[1])
        content = parts[1].strip()
        if "incantation" in content.lower():
            self.target_direction = direction
