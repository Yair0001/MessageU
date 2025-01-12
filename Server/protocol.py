import constants as const
from db import DefensiveDb

class ClientReq:
    def __init__(self, data):
        self._db = DefensiveDb()
        curr_offset = 0
        self._data = data
        self._client_id = self._data[curr_offset:curr_offset+const.CLIENT_ID_SIZE]

        curr_offset += const.CLIENT_ID_SIZE
        self._version = self._data[curr_offset:curr_offset+const.VERSION_SIZE]

        curr_offset += const.VERSION_SIZE
        self._code = self._data[curr_offset:curr_offset+const.CODE_SIZE]

        curr_offset += const.CODE_SIZE
        self._payload_size = self._data[curr_offset:curr_offset+const.PAYLOAD_SZ_SIZE]

        curr_offset += const.PAYLOAD_SZ_SIZE
        self._payload = self._data[curr_offset:]

        self._username = bytes()
        self._public_key = bytes()
        self._msg_type = bytes()
        self._content_size = bytes()
        self._msg_content = bytes()
        self._sym_keys = dict()

    def set_username(self, username): self._username = username
    def set_public_key(self, public_key): self._public_key = public_key
    def set_msg_type(self, msg_type): self._msg_type = msg_type
    def set_content_size(self, content_size): self._content_size = content_size
    def set_msg_content(self, msg_content): self._msg_content = msg_content
    def add_sym_key(self, sym_key, cid): self._sym_keys[cid] = sym_key


    def get_client_id(self): return self._client_id
    def get_version(self): return self._version
    def get_code(self): return self._code
    def get_payload_size(self): return self._payload_size
    def get_payload(self): return self._payload
    def get_username(self): return self._username
    def get_public_key(self): return self._public_key
    def get_msg_type(self): return self._msg_type
    def get_content_size(self): return self._content_size
    def get_msg_content(self): return self._msg_content
    def get_sym_key_by_cid(self, cid): return self._sym_keys[cid]

    def register_req(self):
        usr_name = self._payload[:const.USERNAME_LENGTH] + b'\x00'
        does_exist = self._db.is_username_in_table(usr_name)
        if does_exist:
            return const.ERROR_USERNAME_EXISTS

        self.set_username(usr_name)
        pub_key = self._payload[const.USERNAME_LENGTH:]
        self.set_public_key(pub_key)
        return self._db.insert_new_user(self._username,self._public_key)

    def pub_key_req(self):
        other_cid = self._payload
        does_exist = self._db.is_cid_in_table(other_cid)
        if not does_exist:
            return const.ERROR_CID_NOT_EXISTS

        other_pub_key = self._db.get_pub_key_of_cid(other_cid)
        return other_pub_key

    def send_msg_req(self):
        curr_offset = 0
        other_cid = self._payload[curr_offset:const.CLIENT_ID_SIZE]
        curr_offset += const.CLIENT_ID_SIZE

        self.set_msg_type(self._payload[curr_offset:curr_offset+const.MSG_TYPE_SIZE])
        curr_offset += const.MSG_TYPE_SIZE

        self.set_content_size(self._payload[curr_offset:curr_offset+const.CONTENT_SZ_SIZE])
        curr_offset += const.CONTENT_SZ_SIZE

        self.set_msg_content(self._payload[curr_offset:])

        return self._db.add_new_message(self.get_client_id(),other_cid,self.get_msg_type(),self.get_msg_content())

    def clients_list_req(self):
        return self._db.get_clients_list()

    def waiting_msgs_req(self):
        msgs_to_ret = ""

        res = self._db.get_top_msg_by_cid(self.get_client_id())
        if res == const.ERROR_NO_WAITING_MSGS:
            return const.ERROR_NO_WAITING_MSGS

        while res != const.ERROR_NO_WAITING_MSGS:
            # res is a tuple that contains FromClient cid, Message ID, Message Type, Message Content.
            # it is in this specific order
            res = self._db.get_top_msg_by_cid(self.get_client_id())

            other_cid = res[0]
            msg_id = res[1]
            msg_type = res[2]
            msg_content = res[3]

            msg_size = const.CLIENT_ID_SIZE + const.MSG_ID_SIZE + const.MSG_TYPE_SIZE + len(msg_content)
            curr_msg = other_cid + msg_id + msg_size + msg_type + msg_content
            msgs_to_ret += curr_msg

        return msgs_to_ret

if __name__ == "__main__":
    pass