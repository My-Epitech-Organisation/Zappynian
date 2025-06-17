from ai.src.roles.base_role import Role

class Breeder(Role):
    def __init__(self):
        super().__init__("Breeder")

    def decide(self, queue, world, vision):
        queue.send_and_wait("Fork")
