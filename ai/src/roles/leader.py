from ai.src.roles.base_role import Role
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

class Leader(Role):
    def __init__(self):
        super().__init__("Leader")

    def decide(self, queue, world, vision):
        print("Leader")
        vision_data = queue.send_and_wait("Look")
        vision.parse_look(vision_data)
        inv_data = queue.send_and_wait("Inventory")
        world.parse_inventory(inv_data)
        level = world.level
        needs = INCANTATION_REQUIREMENTS.get(level, None)
        if not needs:
            return
        for res, count in needs.items():
            if res == "players":
                continue
            if world.inventory.get(res, 0) < count:
                return
        num_players = vision.get_tile(0).count("player")
        if num_players < needs["players"]:
            msg = f"incantation"
            queue.send_and_wait(f"Broadcast {msg}")
            return
        queue.send_and_wait("Incantation")
        while True:
            line = queue.connection.read_line()
            if not line:
                continue
            if line.startswith("Current level:"):
                new_level = int(line.split(":")[1].strip())
                world.level = new_level
                print(f"[INFO] Elevation succeeded. New level: {new_level}")
                break
            elif line == "ko":
                print("[INFO] Elevation failed.")
                break
