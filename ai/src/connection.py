import socket
import select

class Connection:

    def __init__(self, host: str, port: int, team_name: str):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.buffer = b""
        self.map_size = (0, 0)
        self.client_slots = 0

    def connect(self):
        try:
            self.socket = socket.create_connection((self.host, self.port))
            self.socket.setblocking(False)
        except Exception as e:
            raise ConnectionError(f"Failed to connect to {self.host}:{self.port}: {e}")

    def send_command(self, command: str):
        if not self.socket:
            raise ConnectionError("Socket is not connected.")
        if not command.endswith("\n"):
            command += "\n"
        self.socket.sendall(command.encode("utf-8"))

    def read_line(self) -> str | None:
        if not self.socket:
            return None
        try:
            ready_to_read, _, _ = select.select([self.socket], [], [], 0.01)
            if self.socket in ready_to_read:
                data = self.socket.recv(4096)
                if not data:
                    raise ConnectionAbortedError("Server closed the connection.")
                self.buffer += data
        except socket.error as e:
            if hasattr(e, 'errno') and e.errno not in (socket.EAGAIN, socket.EWOULDBLOCK):
                raise ConnectionError(f"Socket error: {e}")
            return None
        if b"\n" in self.buffer:
            line, self.buffer = self.buffer.split(b"\n", 1)
            return line.decode("utf-8").strip()
        return None

    def handshake(self):
        while True:
            line = self.read_line()
            if line == "WELCOME":
                break
        self.send_command(self.team_name)
        handshake_complete = False
        while not handshake_complete:
            line = self.read_line()
            if line is None:
                continue
            print(f"[DEBUG] Handshake received line: {line}")
            if line.isdigit():
                if self.client_slots == 0:
                    self.client_slots = int(line)
            elif " " in line:
                parts = line.split()
                if len(parts) == 2 and all(p.isdigit() for p in parts):
                    if self.map_size == (0, 0):
                        self.map_size = (int(parts[0]), int(parts[1]))
            if self.client_slots > 0 and self.map_size != (0, 0):
                handshake_complete = True

    def close(self):
        if self.socket:
            self.socket.close()
            self.socket = None
