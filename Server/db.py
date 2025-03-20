import sqlite3
from datetime import datetime as dt
import constants as const
import struct
import utils
from constants import CLIENT_ID_SIZE


class DefensiveDb:
    def __init__(self):
        self._db = sqlite3.connect(const.DATABASE_NAME)
        self._cur = self._db.cursor()
        self._usr_count = 0
        self.create_tables()
        # self.reset_db()
        self._index = self.initialize_index()
        self._clients_list = self.initialize_clients_list()

    def create_tables(self):
        self._cur.execute("CREATE TABLE IF NOT EXISTS clients(ID, UserName, PublicKey, LastSeen)")
        self._cur.execute("CREATE TABLE IF NOT EXISTS messages(ID, ToClient, FromClient, Type, Content)")

    def initialize_index(self):
        res = self._cur.execute("SELECT COUNT(*) FROM messages")
        return res.fetchone()[0]

    def initialize_clients_list(self):
        """Populate the clients list from the database."""
        clients_list = []
        self._usr_count = 0
        for cid, user_name in self._cur.execute("SELECT ID, UserName FROM clients"):
            user_name_packed = struct.pack(f"!{const.USERNAME_LENGTH}s", bytes(user_name, 'utf-8')) + b'\x00'
            cid_unpacked = bytes.fromhex(cid)
            clients_list.append(cid_unpacked + user_name_packed)
            self._usr_count += 1
        return clients_list

    def get_top_msg_by_cid(self,to_cid):
        self._cur.execute(
            "SELECT FromClient, ID, Type, Content FROM messages WHERE ToClient = ?",
            (to_cid.hex(),)
        )

        res = self._cur.fetchone()
        print(f"DEBUG: res = {res} (type: {type(res)})")

        if res is None:
            return const.ERROR_NO_WAITING_MSGS

        self._cur.execute("DELETE FROM messages WHERE ToClient = ?", (to_cid.hex(),))
        self._db.commit()
        return res

    def is_username_in_table(self, user_name):
        res = self._cur.execute(f"SELECT UserName FROM clients WHERE UserName='{user_name}'")
        return not(res.fetchone() is None)

    def is_cid_in_table(self, cid):
        res = self._cur.execute(f"SELECT ID FROM clients WHERE ID=?",(cid.hex(),))
        return not(res.fetchone() is None)

    def insert_new_user(self,user_name,public_key):
        cid = utils.generate_user_id(user_name)
        user_name_packed = struct.pack(f"!{const.USERNAME_LENGTH}s", bytes(user_name,'utf-8')) + b'\x00'
        cid_unpacked_tup = struct.unpack("!16s",cid)
        cid_unpacked = ""
        for i in cid_unpacked_tup:
            cid_unpacked += bytes.hex(i)

        self._clients_list.append(bytes.fromhex(cid_unpacked) + user_name_packed)

        self._cur.execute("""
            INSERT INTO clients (ID, UserName, PublicKey, LastSeen)
            VALUES (?, ?, ?, ?)
        """, (cid_unpacked, user_name, public_key, dt.now()))
        self._db.commit()
        self._usr_count += 1

        return cid

    def get_clients_list(self, cid):
        return self.rmv_client_from_clients_list(cid)

    def rmv_client_from_clients_list(self,cid):
        newClientsList = []
        for client in self._clients_list:
            currListCid = bytes(client[:CLIENT_ID_SIZE])
            if currListCid != cid:
                newClientsList.append(client)
        return newClientsList

    def add_new_message(self,cid,other_cid,msg_type,content):
        # Create new unique msg_id (4 bytes in length)
        self._index += 1

        # Use a parameterized query
        self._cur.execute("""
            INSERT INTO messages (ID, ToClient, FromClient, Type, Content) 
            VALUES (?, ?, ?, ?, ?)
        """, (self._index, other_cid.hex(), cid.hex(), struct.unpack("!B", msg_type)[0], content.hex()))  # Convert `other_cid` to hex string
        self._db.commit()

        return other_cid, self._index

    def reset_db(self):
        self._cur.execute("DROP TABLE clients")
        self._cur.execute("DROP TABLE messages")
        self.create_tables()

    def get_cid_of_user(self,user_name):
        res = self._cur.execute(f"SELECT ID FROM clients WHERE UserName={user_name}")
        return res.fetchone()[0]
    def get_pub_key_of_cid(self, cid):
        res = self._cur.execute(f"SELECT PublicKey FROM clients WHERE ID=?",(cid.hex(),))
        return res.fetchone()[0]



if __name__ == "__main__":
    pass