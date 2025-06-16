from collections import deque
from ai.src.connection import Connection


class CommandQueue:

    def __init__(self, connection: Connection):
        self.connection = connection
        self.queue = deque()
        self.pending = 0

    def push(self, command: str):
        self.queue.append(command)

    def flush(self):
        while self.queue:
            cmd = self.queue.popleft()
            self.connection.send_command(cmd)
            self.pending += 1

    def handle_response(self, line: str):
        if line in ("ok", "ko", "dead") or line.startswith("message") or line.startswith("["):
            self.pending = max(0, self.pending - 1)

    def reset(self):
        self.queue.clear()
        self.pending = 0
