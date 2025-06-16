#!/usr/bin/env python3

import argparse
import sys
from ai.src.connection import Connection
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision
from ai.src.roles.scout import Scout
from ai.src.roles.role_selector import select_role


class ZappyAI:

    def __init__(self, host: str, port: int, team_name: str):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.conn = Connection(host, port, team_name)
        self.queue = None
        self.world = WorldState()
        self.vision = Vision()
        self.role = Scout()

    def run(self):
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
                if any(char.isdigit() for char in line):
                    self.world.parse_inventory(line)
                    print("[DEBUG] Inventory:", self.world)
                else:
                    self.vision.parse_look(line)
            self.role = select_role(self.world, self.vision)
            self.role.decide(self.queue, self.world, self.vision)
            self.queue.push("Inventory")
            self.queue.push("Look")


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
