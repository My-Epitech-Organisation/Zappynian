#!/usr/bin/env python3

import argparse
import sys
from ai.src.connection import Connection
from ai.src.command_queue import CommandQueue


class ZappyAI:

    def __init__(self, host: str, port: int, team_name: str):
        # Initialise l'IA avec les paramètres de connexion et le nom d'équipe.
        self.host = host
        self.port = port
        self.team_name = team_name
        self.conn = Connection(host, port, team_name)

    def run(self):
        # Lance la connexion, effectue le handshake et démarre la logique principale de l'IA.
        print(f"[INFO] Starting AI for team '{self.team_name}' on {self.host}:{self.port}")
        self.conn.connect()
        self.conn.handshake()
        self.queue = CommandQueue(self.conn)
        # Test très simple pour envoyer des commandes en boucle.
        while True:
            self.queue.push("Forward")
            self.queue.flush()
            line = self.conn.read_line()
            if line:
                print("[RECV]", line)
                self.queue.handle_response(line)


def parse_args():
    if len(sys.argv) == 2 and sys.argv[1] == "-help":
        print("USAGE: ./zappy_ai -p port -n name -h machine")
        sys.exit(0)

    parser = argparse.ArgumentParser(
        description='Zappy AI Client',
        add_help=False
    )
    parser.add_argument('-p', '--port', type=int, required=True, help='Port number of the server')
    parser.add_argument('-n', '--team', type=str, required=True, help='Name of the team')
    parser.add_argument('-h', '--host', type=str, default='localhost', help='Server hostname (default: localhost)')
    return parser.parse_args()


def main():
    # Point d'entrée principal du programme, gère les exceptions et lance l'IA.
    args = parse_args()
    try:
        ai = ZappyAI(args.host, args.port, args.team)
        ai.run()
    except KeyboardInterrupt:
        print("\n[INFO] Interrupted by user. Exiting...")
        sys.exit(0)
    except Exception as error:
        print(f"[ERROR] Unexpected exception: {error}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
