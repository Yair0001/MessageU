import utils
import socket
from _thread import *
import threading
from protocol import ClientReq
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
            cid = client_req.register_req()
            if cid == const.ERROR_USERNAME_EXISTS:
                print("USERNAME ALREADY EXISTS")
                #Return to client using struct
            else:
                print("ADDED USERNAME")
                #Return cid to client using struct

        elif req_code == const.CLIENTS_LIST_CODE:
            pass
        elif req_code == const.CLIENT_PUB_KEY_CODE:
            pub_key = client_req.pub_key_req()
            if pub_key == const.ERROR_CID_NOT_EXISTS:
                print("CID DOES NOT EXIST")
                #Return to client
            else:
                print("Public key: ", pub_key)
                #Return public key and cid using struct

        elif req_code == const.SEND_MSG_CODE:
            msg_id = client_req.send_msg_req()
            print("Message ID: ", msg_id)
            #Return cid and msg_id to client

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