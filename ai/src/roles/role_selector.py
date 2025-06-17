from ai.src.roles.survivor import Survivor
from ai.src.roles.miner import Miner
from ai.src.roles.scout import Scout

GOALS = {
    "linemate": 2,
    "deraumere": 2,
    "sibur": 2,
    "mendiane": 1,
    "phiras": 1,
    "thystame": 1,
}

def select_role(world, vision) -> object:

    if world.get_food_count() <= 3:
        return Survivor()

    for resource, goal in GOALS.items():
        current = world.inventory.get(resource, 0)
        if current < goal:
            return Miner()

    return Scout()
