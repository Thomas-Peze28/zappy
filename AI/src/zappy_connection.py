import socket


def connect_client(host, port, team_name):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host, port))

        buf = ""

        def read_line():
            nonlocal buf
            while "\n" not in buf:
                data = sock.recv(4096)
                if not data:
                    return None
                buf += data.decode("utf-8")
            line, buf = buf.split("\n", 1)
            return line.strip()

        if read_line() != "WELCOME":
            sock.close()
            return None

        sock.sendall((team_name + "\n").encode("utf-8"))

        client_num_str = read_line()
        map_size_str = read_line()

        if not client_num_str or not map_size_str:
            sock.close()
            return None

        client_num = int(client_num_str)
        parts = map_size_str.split()
        map_x, map_y = int(parts[0]), int(parts[1])

        sock.setblocking(False)
        return sock, client_num, map_x, map_y
    except Exception:
        return None
    