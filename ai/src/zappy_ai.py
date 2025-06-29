#!/usr/bin/env python3

import argparse
import sys
import os
from ai.src.connection import Connection
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision
from ai.src.roles.game_logic import *


class ZappyAI:

    def __init__(self, host: str, port: int, team_name: str):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.conn = Connection(host, port, team_name)
        self.queue = None
        self.world = WorldState()
        self.world.team_name = team_name
        self.world.leader = False
        self.vision = Vision()
        self.target_path = None
        self.is_incanting = False
        self.last_role = None
        self.broadcast_timer = 0

    def spawn_player(self):
        pid = os.fork()
        if pid == 0:
            os.execv(sys.executable, [
                sys.executable, __file__,
                "-p", str(self.port),
                "-n", self.team_name,
                "-h", self.host
            ])
            sys.exit(1)

    def run(self):
        print(f"[INFO] Starting AI for team '{self.team_name}' on {self.host}:{self.port}")
        self.conn.connect()
        self.conn.handshake()
        self.queue = CommandQueue(self.conn)
        connect_nbr = 0

        while True:
            self.queue.push("Inventory")
            self.queue.push("Look")
            self.queue.push("Connect_nbr")
            self.queue.flush()
            waiting_incantation = False

            while self.queue.pending > 0:
                line = self.conn.read_line()
                if not line:
                    continue
                if line.startswith("message"):
                    parse_broadcast(self.queue, line, self.world)
                    continue
                if waiting_incantation:
                    if line.startswith("Current level:") or line == "ko":
                        self.queue.handle_response(self.world, line)
                        waiting_incantation = False
                    continue
                if line == "Elevation underway":
                    waiting_incantation = True
                    continue
                if not self.queue.expected_responses:
                    continue
                current_expected = self.queue.expected_responses.pop(0)
                if current_expected == "Inventory":
                    if line.startswith("[") and any(char.isdigit() for char in line):
                        self.world.parse_inventory(line)
                    else:
                        print(f"[WARNING] Expected Inventory, but got: {line}")

                elif current_expected == "Look":
                    if line.startswith("[") and not any(char.isdigit() for char in line):
                        self.vision.parse_look(line)
                    else:
                        print(f"[WARNING] Expected Look, but got: {line}")
                elif current_expected == "Connect_nbr" and line.strip().isdigit():
                    connect_nbr = int(line.strip())
                self.queue.handle_response(self.world, line)
            self.queue.reset()
            if connect_nbr > 1:
                self.spawn_player()
            select_role(self, self.queue, self.world, self.vision)


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
