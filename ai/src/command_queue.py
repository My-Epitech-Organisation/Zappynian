from collections import deque
from ai.src.connection import Connection


class CommandQueue:

    def __init__(self, connection: Connection):
        self.connection = connection
        self.queue = deque()
        self.pending = 0

    def push(self, command: str):
        if not isinstance(command, str):
            raise TypeError(f"[ERROR] Command must be a string, got: {type(command)} with value {command}")
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

    def send_and_wait(self, command: str) -> str:
        self.push(command)
        self.flush()

        while True:
            line = self.connection.read_line()
            print(f"[DEBUG] Waiting for response to command: {command}")
            if not line:
                continue
            print(f"[DEBUG] Received line: {line}")
            if line.startswith("message"):
                continue
            self.handle_response(line)
            if command == "Incantation":
                if line == "Elevation underway":
                    continue
                elif line.startswith("Current level:") or line == "ko":
                    return line
                else:
                    continue
            return line
