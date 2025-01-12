import utils
import socket
from _thread import *
import threading
from protocol import ClientReq
import constants as const
import struct

lock = threading.Lock()

def pack_server_prot(client):
    version = struct.pack("!c", client.get_version())
    code = struct.pack("!h", client.get_code())
    payload_size = struct.pack("!I", client.get_payload_size())
    return version+code+payload_size

def new_client(client):
    while True:
        data = client.recv(1024)
        if not data:
            print("No DATA")
            lock.release()
            break
        client_req = ClientReq(struct.pack("!s",data))
        req_code = int.from_bytes(client_req.get_code(), byteorder=const.ENDIAN)

        if req_code == const.REGISTER_CODE:
            cid = client_req.register_req()
            if cid == const.ERROR_USERNAME_EXISTS:
                print("USERNAME ALREADY EXISTS")
                error_code = struct.pack("!h",const.ERROR_SERVER)
                client.send(error_code)
            else:
                print("ADDED USERNAME")
                prot = pack_server_prot(client_req)
                client.send(prot+cid)

        elif req_code == const.CLIENTS_LIST_CODE:
            clients_list = client_req.clients_list_req()
            if len(clients_list) == 0:
                print("NO CLIENTS YET")
                error_code = struct.pack("!h",const.ERROR_SERVER)
                client.send(error_code)
            else:
                #Return clients_list to user
                prot = pack_server_prot(client_req)
                client.send(prot+clients_list)

        elif req_code == const.CLIENT_PUB_KEY_CODE:
            pub_key = client_req.pub_key_req()
            if pub_key == const.ERROR_CID_NOT_EXISTS:
                print("CID DOES NOT EXIST")
                error_code = struct.pack("!h", const.ERROR_SERVER)
                client.send(error_code)
            else:
                print("Public key: ", pub_key)
                prot = pack_server_prot(client_req)
                pub_key_packed = struct.pack("!s",pub_key)
                cid_packed = struct.pack("!s",client_req.get_client_id())
                client.send(prot+pub_key_packed)

        elif req_code == const.SEND_MSG_CODE:
            msg_id = client_req.send_msg_req()
            print("Message ID: ", msg_id)
            #Return cid and msg_id to client

        elif req_code == const.WAITING_MESSAGES_CODE:
            waiting_msgs = client_req.waiting_msgs_req()
            if waiting_msgs == const.ERROR_NO_WAITING_MSGS:
                print("NO MESSAGES FOR YOU NIGGER")
            else:
                #Return all messages to client
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