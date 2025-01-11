import utils
import socket
from _thread import *
import threading

lock = threading.Lock()

def new_client(client):
    while True:
        data = client.recv(1024)
        if not data:
            print("No DATA")
            lock.release()
            break

        client.send(data)
    client.close()

def main():
    FILE_NAME = "myport.info"
    port = utils.read_port(FILE_NAME)

    sock = utils.set_up_server(port)

    #Clients Loop
    while True:
        client, addr = sock.accept()
        lock.acquire()
        print('Connected to :', addr[0], ':', addr[1])

        start_new_thread(new_client,(client,))





if __name__ ==  "__main__":
    main()