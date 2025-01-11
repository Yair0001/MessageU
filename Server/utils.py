import socket
LOCAL_HOST = "127.0.0.1"


def read_port(file):
    port = 1357
    try:
        file = open(file, 'r')
        port = int(file.read())
        file.close()
    except FileNotFoundError:
        print("Using default port")
    finally:
        return port

def set_up_server(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((LOCAL_HOST, port))
    print("socket binded to port", port)
    sock.listen()
    return sock


if __name__ == "__main__":
    pass