import sqlite3
from datetime import datetime as dt
import constants as const
import struct
import utils

class DefensiveDb:
    def __init__(self):
        self._db = sqlite3.connect(const.DATABASE_NAME)
        self._cur = self._db.cursor()
        self._usr_count = 0
        self._clients_list = b""
        #self.create_tables()
        self.reset_db()

    def create_tables(self):
        self._cur.execute("CREATE TABLE IF NOT EXISTS clients(ID, UserName, PublicKey, LastSeen)")
        self._cur.execute("CREATE TABLE IF NOT EXISTS messages(ID, ToClient, FromClient, Type, Content)")

    def get_top_msg_by_cid(self,to_cid):
        res = self._cur.execute(f"SELECT FromClient,ID,Type,Content FROM message WHERE FromClient={to_cid}")
        if res.fetchone() is None:
            return const.ERROR_NO_WAITING_MSGS
        else:
            return res.fetchone()

    def is_username_in_table(self, user_name):
        res = self._cur.execute(f"SELECT UserName FROM clients WHERE UserName='{user_name}'")
        return not(res.fetchone() is None)

    def is_cid_in_table(self, cid):
        res = self._cur.execute(f"SELECT ID FROM clients WHERE ID={cid}")
        return not(res.fetchone() is None)

    def insert_new_user(self,user_name,public_key):
        cid = utils.generate_user_id(user_name)
        user_name_packed = struct.pack(f"!{const.USERNAME_LENGTH}s", bytes(user_name,'utf-8')) + b'\x00'
        cid_unpacked_tup = struct.unpack("!16s",cid)
        cid_unpacked = ""
        for i in cid_unpacked_tup:
            cid_unpacked += bytes.hex(i)

        if self._usr_count == 0:
            self._clients_list = cid + user_name_packed
        else:
            self._clients_list += cid + user_name_packed

        print("usr count: ", self._usr_count)
        print("usr name ", user_name)
        print("pub key: ", public_key)
        print("pub key: ", cid_unpacked)
        print("time: ", dt.now())

        self._cur.execute("""
            INSERT INTO clients (ID, UserName, PublicKey, LastSeen)
            VALUES (?, ?, ?, ?)
        """, (cid_unpacked, user_name, public_key, dt.now()))
        self._db.commit()

        self._usr_count += 1
        return cid

    def get_clients_list(self): return self._clients_list

    def add_new_message(self,cid,other_cid,msg_type,content):
        #create new unique msg_id in length 4 bytes
        generated_id = 0
        self._cur.execute(f"""
        INSERT INTO messages
        VALUES({generated_id},{other_cid},{cid},{msg_type},{content})
        """)
        return generated_id

    def reset_db(self):
        self._cur.execute("DROP TABLE clients")
        self._cur.execute("DROP TABLE messages")
        self.create_tables()

    def get_cid_of_user(self,user_name):
        res = self._cur.execute(f"SELECT ID FROM clients WHERE UserName={user_name}")
        return res.fetchone()[0]
    def get_pub_key_of_cid(self, cid):
        res = self._cur.execute(f"SELECT PublicKey FROM clients WHERE ID={cid}")
        return res.fetchone()[0]



if __name__ == "__main__":
    pass