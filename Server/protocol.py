
CLIENT_ID_SIZE = 16
VERSION_SIZE = 1
CODE_SIZE = 2
PAYLOAD_SZ_SIZE = 4
USERNAME_LENGTH = 254

class ClientReq:
    def __init__(self, data):
        self._data = data
        self._client_id = self._data[:CLIENT_ID_SIZE]
        self._version = self._data[CLIENT_ID_SIZE:CLIENT_ID_SIZE+VERSION_SIZE]
        self._code = self._data[CLIENT_ID_SIZE+VERSION_SIZE:CLIENT_ID_SIZE+VERSION_SIZE+CODE_SIZE]
        self._payload_size = self._data[CLIENT_ID_SIZE+VERSION_SIZE+CODE_SIZE:CLIENT_ID_SIZE + VERSION_SIZE + CODE_SIZE+PAYLOAD_SZ_SIZE]
        self._payload = self._data[CLIENT_ID_SIZE+VERSION_SIZE+CODE_SIZE+PAYLOAD_SZ_SIZE:]

        self._username = bytes()
        self._public_key = bytes()
        self._msg_type = bytes()
        self._content_size = bytes()
        self._msg_content = bytes()

    def set_username(self, username): self._username = username
    def set_public_key(self, public_key): self._public_key = public_key
    def set_msg_type(self, msg_type): self._msg_type = msg_type
    def set_content_size(self, content_size): self._content_size = content_size
    def set_msg_content(self, msg_content): self._msg_content = msg_content

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


    def register_req(self):
        usr_name = self._payload[:USERNAME_LENGTH] + b'\x00'
        self.set_username(usr_name)
        pub_key = self._payload[USERNAME_LENGTH:]
        self.set_public_key(pub_key)

        #check if usrName exists in sql db

if __name__ == "__main__":
    pass