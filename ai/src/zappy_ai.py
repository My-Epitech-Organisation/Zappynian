#!/usr/bin/env python3

import argparse
import sys
from ai.src.connection import Connection
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision



class ZappyAI:

    def __init__(self, host: str, port: int, team_name: str):
        # Initialise l'IA avec les paramètres de connexion et le nom d'équipe.
        self.host = host
        self.port = port
        self.team_name = team_name
        self.conn = Connection(host, port, team_name)
        self.queue = None
        self.world = WorldState()
        self.vision = Vision()

    def run(self):
        # Lance la connexion, effectue le handshake et démarre la logique principale de l'IA.
        print(f"[INFO] Starting AI for team '{self.team_name}' on {self.host}:{self.port}")
        self.conn.connect()
        self.conn.handshake()
        self.queue = CommandQueue(self.conn)
        self.queue.push("Inventory")
        self.queue.push("Look")
        while True:
            self.queue.flush()
            line = self.conn.read_line()
            if not line:
                continue
            print("[RECV]", line)
            self.queue.handle_response(line)
            if line.startswith("["):
                if "food" in line or "linemate" in line:
                    self.world.parse_inventory(line)
                    print("[DEBUG] Inventory:", self.world)
                elif "player" in line or " " in line or line.strip().startswith("["):
                    self.vision.parse_look(line)
                    print("[DEBUG] Vision:", self.vision)
            # Exemple de décision simple :
            if self.world.get_food_count() <= 2:
                tile = self.vision.find_nearest("food")
                print(f"[DECISION] Food low. Found at tile {tile}")
                if tile == 0:
                    self.queue.push("Take food")
                elif tile == 1:
                    self.queue.push("Forward")
                elif tile in [2, 3]:
                    self.queue.push("Right")
                    self.queue.push("Forward")
                elif tile in [4, 5]:
                    self.queue.push("Left")
                    self.queue.push("Forward")
                else:
                    print("[INFO] Food is too far or not found.")


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
