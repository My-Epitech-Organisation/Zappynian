from ai.src.roles.base_role import Role
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS
from ai.src.utils.route_factory import route_to

class Leader(Role):
    def __init__(self):
        super().__init__("Leader")
        self.broadcast_timer = 0
        self.is_incanting = False
        self.max_broadcast_attempts = 15

    def decide(self, queue, world, vision):
        level = world.level
        if level > 1:
            queue.send_and_wait(f"Broadcast leader_{level}")
        needs = INCANTATION_REQUIREMENTS.get(level)
        if not needs:
            print(f"[INFO] No incantation requirements for level {level}")
            return
        num_players = vision.get_tile(0).count("player")
        print(f"[INFO] Current level: {level}, Players needed: {needs['players']}, Current players: {num_players}")
        if num_players < needs["players"]:
            if self.broadcast_timer < self.max_broadcast_attempts:
                print("BROOOOOADDDDDDDDDCCCCCCCAAAASSSSSSSSTTTTTTT")
                queue.send_and_wait(f"Broadcast incantation_{world.team_name}_{world.level}")
                self.broadcast_timer += 1
                print(f"[INFO] Calling for players ({num_players}/{needs['players']}) - attempt {self.broadcast_timer}")
            elif self.broadcast_timer >= self.max_broadcast_attempts:
                print("[WARN] Max broadcast attempts reached, resetting timer")
                self.broadcast_timer = 0
            return
        print(f"[INFO] Starting incantation with {num_players} players")
        self.is_incanting = True
        self.broadcast_timer = 0
        for res, amount in needs.items():
            if res == "players":
                continue
            inv_amount = world.inventory.get(res, 0)
            drop_count = min(inv_amount, amount)
            print(f"[INFO] Dropping {drop_count}/{amount} {res}")
            for _ in range(drop_count):
                queue.send_and_wait(f"Set {res}")
        line = queue.send_and_wait("Incantation")
        self.is_incanting = False
        if line.startswith("Current level:"):
            try:
                new_level = int(line.split(":")[1].strip())
                world.level = new_level
                print(f"[SUCCESS] Elevated to level {new_level}")
                queue.send_and_wait("Broadcast notleader")
            except (ValueError, IndexError) as e:
                print(f"[ERROR] Failed to parse new level from line: {line} - {e}")
        elif line == "ko":
            print("[ERROR] Incantation failed")
        else:
            print(f"[WARN] Unexpected incantation response: {line}")

    def on_broadcast(self, message, queue, world, vision):
        return
