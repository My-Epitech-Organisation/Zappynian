from ai.src.roles.survivor import Survivor
from ai.src.roles.miner import Miner
from ai.src.roles.scout import Scout
from ai.src.roles.breeder import Breeder
from ai.src.roles.upgrader import Upgrader

MINERGOALS = {
    "linemate": 1,
    "deraumere": 1,
    "sibur": 1,
    "mendiane": 1,
    "phiras": 1,
    "thystame": 1,
}

UPGRADERGOALS = {
    "linemate": 2,
    "deraumere": 2,
    "sibur": 2,
    "mendiane": 2,
    "phiras": 2,
    "thystame": 2,
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
    if world.get_food_count() > 10:
        return Breeder()
    return Scout()
