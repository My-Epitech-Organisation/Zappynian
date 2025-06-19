from abc import ABC, abstractmethod
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision

class Role(ABC):
    def __init__(self, name: str):
        self.name = name

    @abstractmethod
    def decide(self, queue: CommandQueue, world: WorldState, vision: Vision):
        pass

    def on_broadcast(self, message: str, queue: CommandQueue, world: WorldState, vision: Vision):
        pass
