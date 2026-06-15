import select
import sys

from .zappy_agent import ZappyAI
from .zappy_connection import connect_client


class ZappyAIManager:
    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.clients = {}
        self.poller = select.poll()

    def spawn_client(self):
        res = connect_client(self.host, self.port, self.team_name)
        if res:
            sock, client_num, map_x, map_y = res
            fd = sock.fileno()
            client = ZappyAI(self, sock, self.team_name, client_num, map_x, map_y)
            self.clients[fd] = client
            self.poller.register(sock, select.POLLIN | select.POLLERR | select.POLLHUP)
            print(f"[*] [FD {fd}] Nouveau drone connecté. Total actifs : {len(self.clients)}")
            return client
        return None

    def remove_client(self, fd):
        if fd in self.clients:
            client = self.clients[fd]
            print(f"[-] [FD {fd}] Suppression du drone de la boucle centrale.")
            try:
                self.poller.unregister(client.sock)
                client.sock.close()
            except Exception:
                pass
            del self.clients[fd]

    def run(self):
        print(f"[*] Connexion du drone éclaireur sur {self.host}:{self.port}...")
        first_client = self.spawn_client()
        if not first_client:
            print("[!] Erreur : Impossible de connecter le premier drone.", file=sys.stderr)
            sys.exit(1)

        initial_slots = first_client.initial_slots
        print(f"[*] Le serveur autorise {initial_slots} drones supplémentaires immédiatement.")
        for _ in range(initial_slots):
            self.spawn_client()

        print("[*] Lancement de la boucle centrale...")
        while True:
            if not self.clients:
                print("[*] Tous les drones sont morts ou déconnectés. Fin du programme.")
                break

            try:
                events = self.poller.poll(100)
            except KeyboardInterrupt:
                print("\n[*] Interruption utilisateur. Fermeture de la colonie.")
                break

            for fd, event in events:
                client = self.clients.get(fd)
                if not client:
                    continue
                if event & select.POLLIN:
                    client.receive_data()
                if event & (select.POLLERR | select.POLLHUP):
                    self.remove_client(fd)

            for fd in list(self.clients.keys()):
                client = self.clients.get(fd)
                if client:
                    client.think()
