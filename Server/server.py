import utils
import socket
from _thread import *
import threading
from Server.protocol import ClientReq
import constants as const
import struct

lock = threading.Lock()

def new_client(client):
    while True:
        data = client.recv(1024)
        if not data:
            print("No DATA")
            lock.release()
            break

        client_req = ClientReq(data)
        req_code = int.from_bytes(client_req.get_code(), byteorder=const.ENDIAN)

        if req_code == const.REGISTER_CODE:
            return_signal = client_req.register_req()
            if return_signal == const.ERROR_USERNAME_EXISTS:
                print("USERNAME ALREADY EXISTS")
                #Return to client using struct
            elif return_signal == const.OK:
                print("ADDED USERNAME")
                #Return to client using struct

        elif req_code == const.CLIENTS_LIST_CODE:
            pass
        elif req_code == const.CLIENT_PUB_KEY_CODE:
            pass
        elif req_code == const.SEND_MSG_CODE:
            pass
        elif req_code == const.WAITING_MESSAGES_CODE:
            pass

    client.close()

def main():
    port = utils.read_port(const.FILE_NAME)

    sock = utils.set_up_server(port)

    #Clients Loop
    while True:
        client, addr = sock.accept()
        lock.acquire()
        print('Connected to :', addr[0], ':', addr[1])

        start_new_thread(new_client,(client,))





if __name__ ==  "__main__":
    main()