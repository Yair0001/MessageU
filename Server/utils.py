import socket
import hashlib
import struct
LOCAL_HOST = "127.0.0.1"

def generate_user_id(username):
    # Create a SHA-256 hash of the username
    hash_object = hashlib.sha256(username.encode())
    user_id = struct.pack("!s",hash_object.hexdigest()[:16])  # Take the first 16 characters for compactness
    return user_id

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