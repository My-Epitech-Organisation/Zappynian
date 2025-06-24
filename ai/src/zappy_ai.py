#!/usr/bin/env python3

import argparse
import sys
import os
from ai.src.connection import Connection
from ai.src.command_queue import CommandQueue
from ai.src.inventory_parser import WorldState
from ai.src.vision_parser import Vision
from ai.src.roles.survivor import Survivor
from ai.src.roles.role_selector import select_role


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
        self.role = Survivor()

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

    def read_passive_messages(self):
        while True:
            line = self.conn.read_line()
            if not line:
                break
            if line.startswith("message"):
                if hasattr(self.role, "on_broadcast"):
                    self.role.on_broadcast(line, self.queue, self.world, self.vision)

    def run(self):
        print(f"[INFO] Starting AI for team '{self.team_name}' on {self.host}:{self.port}")
        self.conn.connect()
        self.conn.handshake()
        self.queue = CommandQueue(self.conn)

        while True:
            inv_line = self.queue.send_and_wait("Inventory")
            self.world.parse_inventory(inv_line)
            look_line = self.queue.send_and_wait("Look")
            self.vision.parse_look(look_line)
            connect_nbr = self.queue.send_and_wait("Connect_nbr")
            nbr = int(connect_nbr.split()[0])
            if nbr > 0:
                self.spawn_player()
            self.read_passive_messages()
            self.role = select_role(self.world, self.vision)
            self.role.decide(self.queue, self.world, self.vision)
            self.queue.flush()


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
