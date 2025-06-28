from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import *

class Breeder(Role):
    def __init__(self):
        super().__init__("Breeder")
        self.target_path = None

    def decide(self, queue, world, vision):
        if self.target_path:
            for cmd in self.target_path:
                queue.send_and_wait(cmd)
            queue.send_and_wait("Look")
            self.target_path = None
            return
        queue.send_and_wait("Fork")

    def on_broadcast(self, message, queue, world, vision):
        print(f"[BREEDERMESS] {message}")
        parts = message.split(",", 1)
        if len(parts) != 2:
            return
        if parts[1] == f"leader_{world.level}":
            world.leader = True
            return
        elif parts[1] == "notleader":
            world.leader = False
            return
        elif parts[1].startswith("incantation"):
            try:
                msg = parts[1].strip().split("_", 2)
                if msg[1] != world.team_name or msg[2] != str(world.level):
                    return
                direction_str, content = parts
                tokens = direction_str.strip().split()
                if len(tokens) < 2:
                    return
                direction = int(tokens[1])
                content = content.strip().lower()
                self.target_path = goal_to(direction)
            except Exception as e:
                print(f"[WARN] Failed to parse broadcast: {e}")
