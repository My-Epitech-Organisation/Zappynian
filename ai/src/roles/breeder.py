from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import *

class Breeder(Role):
    def __init__(self):
        super().__init__("Breeder")
        self.target_direction = None

    def decide(self, queue, world, vision):
        # if self.target_direction is not None:
        #     print(f"Breeder : Moving to target direction: {self.target_direction}")
        #     path = goal_to(self.target_direction)
        #     for cmd in path:
        #         queue.send_and_wait(cmd)
        #     self.target_direction = None
        #     return
        queue.send_and_wait("Fork")

    def on_broadcast(self, message, queue, world, vision):
        return
        # parts = message.split(",", 1)
        # if len(parts) != 2:
        #     return
        # try:
        #     tokens = parts[0].split()
        #     if len(tokens) < 2 or not tokens[1].isdigit():
        #         return
        #     direction = int(tokens[1])
        # except (IndexError, ValueError):
        #     return
        # content = parts[1].strip()
        # if "incantation" in content.lower():
        #     self.target_direction = direction
