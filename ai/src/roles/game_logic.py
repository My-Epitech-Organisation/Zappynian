from ai.src.roles.survivor import survivor
from ai.src.utils.route_factory import goal_to
from ai.src.roles.miner import miner
from ai.src.roles.breeder import breeder
from ai.src.roles.leader import leader
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS, MINIMUM_FOOD_REQUIREMENTS

r_value = 0

def move_to_target(queue, target_path):
    print(f"[INFO] Moving to target path: {target_path}")
    if target_path:
        for cmd in target_path:
            queue.push(cmd)
        return True
    return False

def parse_broadcast(queue, message, world):
    parts = message.split(",", 1)
    if len(parts) != 2:
        return
    if parts[1] == f" leader_{world.level}":
        world.leader = True
        return
    elif parts[1] == " notleader":
        world.leader = False
        world.leader_direction = -69
        return
    elif parts[1].startswith(" incantation"):
        try:
            msg = parts[1].strip().split("_", 2)
            if msg[1] != world.team_name or msg[2] != str(world.level):
                return
            direction_str, content = parts
            tokens = direction_str.strip().split()
            if len(tokens) < 2:
                return
            world.leader_direction = int(tokens[1])
            content = content.strip().lower()
            world.target_path = goal_to(world.leader_direction)
            if world.target_path:
                world.following_leader = True
                move_to_target(queue, world.target_path)
                world.target_path = None
        except Exception as e:
            print(f"[WARN] Failed to parse broadcast: {e}")

def security_move(queue):
    queue.push("Right")
    queue.push("Forward")
    queue.push("Left")
    queue.push("Forward")
    queue.push("Forward")

def select_role(self, queue, world, vision):
    food = world.get_food_count()
    min_food = MINIMUM_FOOD_REQUIREMENTS.get(world.level, 25)

    if food < 15:
        self.last_role = "Survivor"
        r_value = survivor(queue, vision)
        if r_value == 1:
            security_move(queue)
        return

    if world.following_leader:
        if world.leader_direction == 0:
            print("[INFO] Leader is at the same POSITION, not moving.")
        elif world.leader_direction == -69:
            world.following_leader = False
            world.leader_direction = -1
        else:
            return
        return

    if self.last_role == "Survivor" and food < min_food:
        r_value = survivor(queue, vision)
        if r_value == 1:
            security_move(queue)
        return

    if food > min_food - 1:
        self.last_role = "Breeder"
        breeder(queue)
        return

    if self.last_role == "leader" and self.is_incanting:
        r_value = leader(self, queue, world, vision)
        if r_value == 1:
            security_move(queue)
        return

    reqs = INCANTATION_REQUIREMENTS.get(world.level)
    if reqs and not world.leader:
        has_enough = all(
            world.inventory.get(res, 0) >= amt
            for res, amt in reqs.items()
            if res != "players"
        )
        if has_enough:
            self.last_role = "Leader"
            r_value = leader(self, queue, world, vision)
            if r_value == 1:
                security_move(queue)
            return

    self.last_role = "Miner"
    r_value = miner(queue, world, vision)
    if r_value == 1:
        security_move(queue)
    return
