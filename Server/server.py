import base64

import utils
import socket
from _thread import *
import threading
from protocol import ClientReq
import constants as const
import struct


def pack_server_prot(client):
    version = struct.pack("!c", client.get_version())
    code = struct.pack("!H", client.get_code())
    print("version len: ", len(version))
    print("code len: ", len(code))
    return version+code

def new_client(client):
    while True:
        data = client.recv(1024)
        if not data:
            print("No DATA")
            break
        print(f"Received data: {data}")
        client_req = ClientReq(data)

        req_code = client_req.get_code()
        print(f"Code: {req_code}")

        if req_code == const.REGISTER_CODE:
            cid = client_req.register_req()
            if cid == const.ERROR_USERNAME_EXISTS:
                print("USERNAME ALREADY EXISTS")
                client_req._code = const.ERROR_SERVER
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", 0)
                client.send(prot+client_req.get_payload_size())
            else:
                print("ADDED USERNAME")
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", len(cid))
                print(''.join('{:02x}'.format(x) for x in cid))
                client.send(prot+client_req.get_payload_size()+cid)

        elif req_code == const.CLIENTS_LIST_CODE:
            clients_list = client_req.clients_list_req()
            if len(clients_list) == 0:
                print("NO CLIENTS YET")
                client_req._code = const.ERROR_SERVER
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", 0)

                print("code: ", client_req.get_code())
                print("version: ",client_req.get_version())
                print("payload size: ",client_req.get_payload_size())

                client.send(prot+client_req.get_payload_size())
            else:
                #Return clients_list to user
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", len(clients_list)*(const.USERNAME_LENGTH+1+const.CLIENT_ID_SIZE))
                print("code: ", client_req.get_code())
                print("version: ", client_req.get_version())
                print("payload size: ", client_req.get_payload_size())
                print("payload len: ", len(client_req.get_payload_size()))

                list_to_send = b""
                for i in clients_list:
                    list_to_send += i

                client.send(prot+client_req.get_payload_size()+list_to_send)

        elif req_code == const.CLIENT_PUB_KEY_CODE:
            other_cid, pub_key = client_req.pub_key_req()
            if pub_key == const.ERROR_CID_NOT_EXISTS:
                print("CID DOES NOT EXIST")
                client_req._code = const.ERROR_SERVER
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", 0)
                print("code: ", client_req.get_code())
                print("version: ", client_req.get_version())
                print("payload size: ", client_req.get_payload_size())
                print("payload len: ", len(client_req.get_payload_size()))

                client.send(prot+client_req.get_payload_size())
            else:
                print("Public key: ", pub_key)
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", const.CLIENT_ID_SIZE+const.PUBLIC_KEY_SIZE)
                pub_key_packed = struct.pack(f"!{const.PUBLIC_KEY_SIZE}s",base64.b64decode(pub_key))
                other_cid_packed = struct.pack(f"!{const.CLIENT_ID_SIZE}s", other_cid)
                client.send(prot+client_req.get_payload_size()+other_cid_packed+pub_key_packed)

        elif req_code == const.SEND_MSG_CODE:
            cid, msg_id = client_req.send_msg_req()
            client_req._payload_size = struct.pack("!I", const.CLIENT_ID_SIZE+const.MSG_ID_SIZE)
            prot = pack_server_prot(client_req)
            cid_packed = struct.pack(f"!{const.CLIENT_ID_SIZE}s", cid)
            msg_id_packed = struct.pack(f"!I", msg_id)
            client.send(prot+client_req.get_payload_size()+cid_packed+msg_id_packed)

        elif req_code == const.WAITING_MESSAGES_CODE:
            waiting_msgs = client_req.waiting_msgs_req()
            if waiting_msgs == const.ERROR_NO_WAITING_MSGS:
                print("NO MESSAGES FOR YOU NIGGER")
                client_req._code = const.ERROR_SERVER
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", 0)
                print("code: ", client_req.get_code())
                print("version: ", client_req.get_version())
                print("payload size: ", client_req.get_payload_size())
                print("payload len: ", len(client_req.get_payload_size()))

                client.send(prot+client_req.get_payload_size())

            else:
                #Return all messages to client
                prot = pack_server_prot(client_req)
                client_req._payload_size = struct.pack("!I", len(waiting_msgs))
                client.send(prot+client_req.get_payload_size()+waiting_msgs)

    client.close()

def main():
    port = utils.read_port(const.FILE_NAME)

    sock = utils.set_up_server(port)

    #Clients Loop
    while True:
        client, addr = sock.accept()
        print('Connected to :', addr[0], ':', addr[1])

        start_new_thread(new_client,(client,))

if __name__ ==  "__main__":
    main()