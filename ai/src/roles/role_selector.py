from ai.src.roles.survivor import Survivor
from ai.src.roles.miner import Miner
from ai.src.roles.scout import Scout
from ai.src.roles.breeder import Breeder
from ai.src.roles.upgrader import Upgrader
from ai.src.roles.leader import Leader
from ai.src.utils.incantation_data import INCANTATION_REQUIREMENTS

MINERGOALS = {
    "linemate": 1,
    "deraumere": 1,
    "sibur": 2,
    "mendiane": 2,
    "phiras": 1,
    "thystame": 0,
}

UPGRADERGOALS = {
    "linemate": 2,
    "deraumere": 2,
    "sibur": 3,
    "mendiane": 3,
    "phiras": 2,
    "thystame": 1,
}

def select_role(world, vision) -> object:

    if world.get_food_count() < 5:
        return Survivor()
    for resource, goal in MINERGOALS.items():
        current = world.inventory.get(resource, 0)
        if current < goal:
            return Miner()
    for resource, goal in UPGRADERGOALS.items():
        current = world.inventory.get(resource, 0)
        if current < goal:
            return Upgrader()
    level = world.inventory.get("level", 1)
    reqs = INCANTATION_REQUIREMENTS.get(level)
    if reqs:
        enough = True
        for res, amount in reqs.items():
            if res == "players":
                continue
            if world.inventory.get(res, 0) < amount:
                enough = False
                break
        if enough:
            return Leader()
    if world.get_food_count() > 10:
        return Breeder()
    return Scout()
