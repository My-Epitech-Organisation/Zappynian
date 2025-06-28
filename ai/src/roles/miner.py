from ai.src.roles.base_role import Role
from ai.src.utils.route_factory import *
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

class Miner(Role):
    def __init__(self):
        super().__init__("Miner")
        self.target_path = None

    def decide(self, queue, world, vision):
        if self.target_path:
            for cmd in self.target_path:
                queue.send_and_wait(cmd)
            queue.send_and_wait("Look")
            self.target_path = None
            return
        level = world.level
        goals = INCANTATION_REQUIREMENTS.get(level, {})
        for stone, goal in goals.items():
            if stone == "players":
                continue
            if world.inventory.get(stone, 0) < goal:
                tile = vision.find_nearest(stone)
                if tile == -1:
                    queue.send_and_wait("Right")
                    queue.send_and_wait("Forward")
                    queue.send_and_wait("Left")
                    queue.send_and_wait("Forward")
                    return
                path = route_to(tile)
                for cmd in path:
                    queue.send_and_wait(cmd)
                queue.send_and_wait(f"Take {stone}")
                return
        else:
            queue.send_and_wait("Right")
            queue.send_and_wait("Forward")
            queue.send_and_wait("Left")
            queue.send_and_wait("Forward")

    def on_broadcast(self, message, queue, world, vision):
        print(f"[MINERMESS] {message}")
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
