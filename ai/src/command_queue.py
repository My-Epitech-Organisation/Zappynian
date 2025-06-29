from collections import deque
from ai.src.connection import Connection


class CommandQueue:

    def __init__(self, connection: Connection):
        self.connection = connection
        self.queue = deque()
        self.pending = 0
        self.expected_responses = []

    def push(self, command: str):
        if not isinstance(command, str):
            raise TypeError(f"[ERROR] Command must be a string, got: {type(command)} with value {command}")
        self.queue.append(command)
        self.expected_responses.append(command)

    def flush(self):
        while self.queue:
            cmd = self.queue.popleft()
            self.connection.send_command(cmd)
            self.pending += 1

    def handle_response(self, world, line: str):
        if line in ("ok", "ko", "dead") or line.startswith("message") or line.startswith("[") or line.isdigit():
            self.pending = max(0, self.pending - 1)
        if line.startswith("Current level:"):
            try:
                new_level = int(line.split(":")[1].strip())
                world.level = new_level
                self.pending = max(0, self.pending - 1)
                print(f"[INFO] Current level updated to: {new_level}")
                self.push("Broadcast notleader")
            except (ValueError, IndexError) as e:
                print(f"[ERROR] Failed to parse new level from line: {line} - {e}")
                return

    def reset(self):
        self.queue.clear()
        self.pending = 0
        self.expected_responses.clear()
