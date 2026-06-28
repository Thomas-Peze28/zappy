import os
import select
import subprocess
import sys
import time

from .zappy_agent import ZappyAI
from .zappy_connection import connect_client


class ZappyAIManager:
    MAX_LOCAL_CHILDREN = 4

    def __init__(self, host, port, team_name):
        self.host = host
        self.port = port
        self.team_name = team_name
        self.client = None
        self.spawned_children = 0

    def connect_single_client(self):
        res = connect_client(self.host, self.port, self.team_name)
        if res:
            sock, client_num, map_x, map_y = res
            self.client = ZappyAI(self, sock, self.team_name, client_num, map_x, map_y)
            print(f"[*] Drone connecte avec succes. id={self.client.drone_id} slots={client_num} map={map_x}x{map_y}", flush=True)
            return True
        return False

    def spawn_child_drone(self):
        if self.spawned_children >= self.MAX_LOCAL_CHILDREN:
            print("[IA] Limite locale de spawn atteinte pour ce processus.", flush=True)
            return False

        launcher = os.path.abspath(sys.argv[0])
        if not os.path.exists(launcher):
            print(f"[IA] Launcher introuvable: {launcher}", flush=True)
            return False

        args = [launcher, "-p", str(self.port), "-n", self.team_name, "-h", self.host]
        log_dir = os.environ.get("ZAPPY_AI_LOG_DIR", "./logs_drones")
        os.makedirs(log_dir, exist_ok=True)
        child_index = self.spawned_children + 1
        log_path = os.path.join(log_dir, f"auto_drone_{os.getpid()}_{child_index}_{int(time.time() * 1000)}.log")

        try:
            log_file = open(log_path, "a", buffering=1)
            subprocess.Popen(
                args,
                stdout=log_file,
                stderr=subprocess.STDOUT,
                start_new_session=True,
                env={**os.environ, "PYTHONUNBUFFERED": "1", "ZAPPY_AI_LOG_DIR": log_dir},
            )
            self.spawned_children += 1
            print(f"[IA] Child drone lance: {' '.join(args)} log={log_path}", flush=True)
            return True
        except Exception as exc:
            print(f"[IA] Impossible de lancer un nouveau drone automatiquement: {exc}", flush=True)
            return False

    def disconnect(self):
        if self.client:
            print("[-] Deconnexion du drone et fermeture du socket.", flush=True)
            try:
                self.client.sock.close()
            except Exception:
                pass
            self.client = None

    def run(self):
        print(f"[*] Connexion du drone sur {self.host}:{self.port}...", flush=True)
        if not self.connect_single_client():
            print("[!] Erreur : Impossible de connecter le drone.", file=sys.stderr, flush=True)
            sys.exit(1)
        print("[*] Lancement de la boucle centrale...", flush=True)
        while self.client:
            try:
                readable, _, exceptional = select.select([self.client.sock], [], [self.client.sock], 0.1)
                if exceptional:
                    print("[!] Erreur detectee sur le socket de connexion.", flush=True)
                    self.disconnect()
                    break
                if readable:
                    self.client.net.receive_data()
                if self.client:
                    self.client.think()

            except KeyboardInterrupt:
                print("\n[*] Interruption utilisateur. Deconnexion.", flush=True)
                self.disconnect()
                break
            except Exception as e:
                print(f"\n[!] Une erreur inattendue est survenue : {e}", flush=True)
                self.disconnect()
                break

        print("[*] Fin du programme.", flush=True)
