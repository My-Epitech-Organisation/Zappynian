from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS


max_broadcast_attempts = 12

def leader(self, queue, world, vision) -> int:
    level = world.level
    if level > 1:
        queue.send_and_wait(f"Broadcast leader_{level}")
    needs = INCANTATION_REQUIREMENTS.get(level)
    if not needs:
        return 1
    num_players = vision.get_tile(0).count("player")
    if num_players < needs["players"]:
        if self.broadcast_timer < max_broadcast_attempts:
            queue.send_and_wait(f"Broadcast incantation_{world.team_name}_{world.level}")
            self.broadcast_timer += 1
        elif self.broadcast_timer >= max_broadcast_attempts:
            self.broadcast_timer = 0
        return 0
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
            queue.send_and_wait("Broadcast notleader")
        except (ValueError, IndexError) as e:
            print(f"[ERROR] Failed to parse new level from line: {line} - {e}")
            return 1
        return 0
    elif line == "ko":
        print("[ERROR] Incantation failed")
        return 1
    else:
        print(f"[WARN] Unexpected incantation response: {line}")
        return 1
