from ai.src.roles.base_role import Role

class Survivor(Role):
    def __init__(self):
        super().__init__("Survivor")

    def decide(self, queue, world, vision):
        tile = vision.find_nearest("food")

        if tile == 0:
            queue.push("Take food")
            queue.push("Forward")
        elif tile == 1:
            queue.push("Forward")
            queue.push("Left")
            queue.push("Forward")
            queue.push("Take food")
        elif tile in [2, 3]:
            queue.push("Forward")
            queue.push("Take food")
            queue.push("Right")
            queue.push("Forward")
            queue.push("Take food")
        elif tile in [4, 5]:
            queue.push("Forward")
            queue.push("Forward")
            queue.push("Take food")
            queue.push("Left")
            queue.push("Forward")
            queue.push("Take food")
            queue.push("Forward")
            queue.push("Take food")
        else:
            queue.push("Right")
            queue.push("Forward")
            queue.push("Look")
