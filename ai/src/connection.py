"""Zappy server connection and protocol handler."""

import socket
import select


class Connection:

    def __init__(self, host: str, port: int, team_name: str):
        # Initialise la connexion avec les paramètres du serveur et le nom d'équipe.
        self.host = host
        self.port = port
        self.team_name = team_name
        self.socket = None
        self.buffer = b""
        self.map_size = (0, 0)
        self.client_slots = 0

    def connect(self):
        # Établit la connexion TCP avec le serveur Zappy.
        try:
            self.socket = socket.create_connection((self.host, self.port))
            self.socket.setblocking(False)
        except Exception as e:
            raise ConnectionError(f"Failed to connect to {self.host}:{self.port}: {e}")

    def send_command(self, command: str):
        # Envoie une commande au serveur, ajoutant un saut de ligne si nécessaire.
        if not self.socket:
            raise ConnectionError("Socket is not connected.")
        if not command.endswith("\n"):
            command += "\n"
        self.socket.sendall(command.encode("utf-8"))

    def read_line(self) -> str | None:
        # Lit une ligne depuis le serveur de façon non bloquante.
        try:
            ready_to_read, _, _ = select.select([self.socket], [], [], 0)
            if self.socket in ready_to_read:
                data = self.socket.recv(4096)
                if not data:
                    raise ConnectionAbortedError("Server closed the connection.")
                self.buffer += data
        except BlockingIOError:
            return None

        if b"\n" in self.buffer:
            line, self.buffer = self.buffer.split(b"\n", 1)
            return line.decode("utf-8").strip()

        return None

    def handshake(self):
        # Effectue la poignée de main initiale avec le serveur (WELCOME, nom d'équipe, slots, taille de la map).
        while True:
            line = self.read_line()
            if line == "WELCOME":
                break
        self.send_command(self.team_name)
        while self.client_slots == 0 or self.map_size == (0, 0):
            line = self.read_line()
            if line is None:
                continue
            if line.isdigit():
                self.client_slots = int(line)
            elif " " in line:
                parts = line.split()
                if len(parts) == 2 and all(p.isdigit() for p in parts):
                    self.map_size = (int(parts[0]), int(parts[1]))

    def close(self):
        # Ferme la connexion avec le serveur proprement.
        if self.socket:
            self.socket.close()
            self.socket = None
