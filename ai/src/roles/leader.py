from ai.src.roles.base_role import Role
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

class Leader(Role):
    def __init__(self):
        super().__init__("Leader")
        self.broadcast_timer = 0
        self.is_incanting = False

    def decide(self, queue, world, vision):
        level = world.level
        needs = INCANTATION_REQUIREMENTS.get(level)
        if not needs:
            return
        num_players = vision.get_tile(0).count("player")
        if num_players < needs["players"]:
            if self.broadcast_timer < 10:
                queue.send_and_wait("Broadcast i_m_leader")
                queue.send_and_wait(f"Broadcast {world.team_name}_leader_incantation")
                self.broadcast_timer += 1
            return
        self.is_incanting = True
        self.broadcast_timer = 0
        for res, amount in needs.items():
            if res == "players":
                continue
            inv_amount = world.inventory.get(res, 0)
            drop_count = min(inv_amount, amount)
            for _ in range(drop_count):
                queue.send_and_wait(f"Set {res}")
        line = queue.send_and_wait("Incantation")
        if line.startswith("Current level:"):
            try:
                new_level = int(line.split(":")[1].strip())
                world.level = new_level
            except ValueError:
                print(f"[WARN] Failed to parse new level from line: {line}")
        elif line == "ko":
            print("[INFO] Elevation failed.")

    def on_broadcast(self, message, queue, world, vision):
        return
