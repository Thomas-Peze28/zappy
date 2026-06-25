import select
import sys

from .zappy_agent import ZappyAI
from .zappy_connection import connect_client


class ZappyAIManager:
    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.client = None

    def connect_single_client(self):
        res = connect_client(self.host, self.port, self.team_name)
        if res:
            sock, client_num, map_x, map_y = res
            self.client = ZappyAI(self, sock, self.team_name, client_num, map_x, map_y)
            print("[*] Drone connecté avec succès.")
            return True
        return False

    def disconnect(self):
        if self.client:
            print("[-] Déconnexion du drone et fermeture du socket.")
            try:
                self.client.sock.close()
            except Exception:
                pass
            self.client = None

    def run(self):
        print(f"[*] Connexion du drone sur {self.host}:{self.port}...")
        if not self.connect_single_client():
            print("[!] Erreur : Impossible de connecter le drone.", file=sys.stderr)
            sys.exit(1)
        print("[*] Lancement de la boucle centrale...")
        while self.client:
            try:
                readable, _, exceptional = select.select([self.client.sock], [], [self.client.sock], 0.1)
                if exceptional:
                    print("[!] Erreur détectée sur le socket de connexion.")
                    self.disconnect()
                    break
                if readable:
                    self.client.receive_data()
                if self.client:
                    self.client.think()

            except KeyboardInterrupt:
                print("\n[*] Interruption utilisateur. Déconnexion.")
                self.disconnect()
                break
            except Exception as e:
                print(f"\n[!] Une erreur inattendue est survenue : {e}")
                self.disconnect()
                break

        print("[*] Fin du programme.")