#!/usr/bin/env python3

import argparse

from src.zappy_ai_manager import ZappyAIManager


def parse_arguments():
    parser = argparse.ArgumentParser(description="Zappy AI Manager", add_help=False)
    parser.add_argument("-p", type=int, required=True, help="Port number")
    parser.add_argument("-n", type=str, required=True, help="Name of the team")
    parser.add_argument("-h", type=str, default="localhost", help="server IP (default localhost)")
    parser.add_argument("--help", action="help", default=argparse.SUPPRESS)
    return parser.parse_args()


def main():
    args = parse_arguments()
    manager = ZappyAIManager(host=args.h, port=args.p, team_name=args.n)
    manager.run()


if __name__ == "__main__":
    main()