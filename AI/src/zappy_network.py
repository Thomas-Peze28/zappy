from collections import deque

class ZappyNetwork:
    MAX_PENDING = 10

    def __init__(self, sock, manager, response_handler):
        self.sock = sock
        self.manager = manager
        self._handle_response = response_handler
        self.read_buffer = ""
        self.command_queue = deque()
        self.sent_commands = deque()

    def queue_command(self, command):
        self.command_queue.append(command)
        self._flush_commands()

    def receive_data(self):
        try:
            data = self.sock.recv(4096)
            if not data:
                self.manager.disconnect()
                return
            self.read_buffer += data.decode("utf-8")
            self._process_buffer()
        except BlockingIOError:
            pass
        except Exception:
            self.manager.disconnect()

    def has_pending(self):
        return bool(self.command_queue or self.sent_commands)

    def clear_queue(self):
        self.command_queue.clear()

    def _flush_commands(self):
        while self.command_queue and len(self.sent_commands) < self.MAX_PENDING:
            cmd = self.command_queue.popleft()
            try:
                self.sock.sendall((cmd + "\n").encode("utf-8"))
                self.sent_commands.append(cmd)
            except Exception:
                self.manager.disconnect()
                return

    def _process_buffer(self):
        while "\n" in self.read_buffer:
            line, self.read_buffer = self.read_buffer.split("\n", 1)
            line = line.strip()
            if line == "dead":
                print("[*] Le drone est mort de faim.")
                self.manager.disconnect()
                return
            if line.startswith("message"):
                parts = line.replace("message ", "").split(",", 1)
                if len(parts) == 2:
                    try:
                        self._handle_broadcast_line(int(parts[0].strip()), parts[1].strip())
                    except ValueError:
                        pass
                continue
            if line.startswith("eject:"):
                continue
            if line.startswith("Current level:"):
                self._handle_level_up(line)
                continue
            if self.sent_commands:
                original_cmd = self.sent_commands.popleft()
                self._handle_response(original_cmd, line)
            self._flush_commands()

    def set_broadcast_handler(self, fn):
        self._handle_broadcast_line = fn

    def set_level_up_handler(self, fn):
        self._handle_level_up = fn
