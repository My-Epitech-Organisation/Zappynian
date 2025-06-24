from ai.src.roles.survivor import Survivor
from ai.src.roles.miner import Miner
from ai.src.roles.breeder import Breeder
from ai.src.roles.leader import Leader
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

last_role = None

def select_role(world, vision) -> object:
    global last_role
    food = world.get_food_count()

    if food < 10:
        last_role = Survivor()
        return last_role
    if isinstance(last_role, Survivor) and food < 55:
        return last_role
    if isinstance(last_role, Leader) and last_role.is_incanting:
        return last_role
    level = world.level
    reqs = INCANTATION_REQUIREMENTS.get(level)
    if reqs:
        has_enough = all(
            world.inventory.get(res, 0) >= amt
            for res, amt in reqs.items()
            if res != "players"
        )
        if has_enough and not world.leader:
            print(f"[INFOXX] {world.leader}")
            last_role = Leader()
            return last_role
    last_role = Miner()
    return last_role
