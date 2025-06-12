from collections import deque
from ai.src.connection import Connection


class CommandQueue:

    def __init__(self, connection: Connection):
        # Initialise la file de commandes avec une connexion donnée
        self.connection = connection
        self.queue = deque()
        self.pending = 0

    def push(self, command: str):
        # Ajoute une commande à la file d'attente
        self.queue.append(command)

    def flush(self):
        # Envoie les commandes tant que la limite de commandes en attente n'est pas atteinte
        while self.queue:
            cmd = self.queue.popleft()
            self.connection.send_command(cmd)
            self.pending += 1

    def handle_response(self, line: str):
        # Gère la réponse du serveur et met à jour le nombre de commandes en attente
        if line in ("ok", "ko", "dead") or line.startswith("message") or line.startswith("["):
            self.pending = max(0, self.pending - 1)

    def reset(self):
        # Réinitialise la file de commandes et le compteur de commandes en attente
        self.queue.clear()
        self.pending = 0
