from ai.src.roles.base_role import Role
import random

class Scout(Role):
    def __init__(self):
        super().__init__("Scout")

    def decide(self, queue, world, vision):
        nbr = random.randint(1, 3)

        queue.push("Look")
        if nbr == 1:
            queue.push("Forward")
            queue.push("Look")
        elif nbr == 2:
            queue.push("Right")
            queue.push("Forward")
            queue.push("Look")
        elif nbr == 3:
            queue.push("Left")
            queue.push("Forward")
            queue.push("Look")
        else:
            queue.push("Look")
