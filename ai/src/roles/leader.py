from ai.src.roles.base_role import Role
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

class Leader(Role):
    def __init__(self):
        super().__init__("Leader")

    def decide(self, queue, world, vision):
        vision_data = queue.send_and_wait("Look")
        vision.parse_look(vision_data)
        inv_data = queue.send_and_wait("Inventory")
        world.parse_inventory(inv_data)

        level = world.inventory.get("level", 1)
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
            msg = f"JOIN_INCANTATION {level}"
            queue.send_and_wait(f"Broadcast {msg}")
            return
        queue.send_and_wait("Incantation")
