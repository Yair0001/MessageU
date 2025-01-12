import sqlite3
from datetime import datetime as dt
import constants as const

class DefensiveDb:
    def __init__(self):
        db = sqlite3.connect(const.DATABASE_NAME)
        self._cur = db.cursor()
        self._cid = 0
        self.create_tables()

    def create_tables(self):
        self._cur.execute("CREATE TABLE clients(ID, UserName, PublicKey, LastSeen")
        self._cur.execute("CREATE TABLE messages(ID, ToClient, FromClient, Type, Content")

    def is_username_in_table(self, user_name):
        res = self._cur.execute(f"SELECT UserName FROM clients WHERE UserName={user_name}")
        return not(res.fetchone() is None)

    def is_cid_in_table(self, cid):
        res = self._cur.execute(f"SELECT ID FROM clients WHERE ID={cid}")
        return not(res.fetchone() is None)

    def insert_new_user(self,user_name,public_key):
        #create new unique id in length 16 bytes
        self._cur.execute(f"""
        INSERT INTO client
        VALUES({str(self._cid)},{user_name},{public_key},{dt.now()})
        """)
        self._cid += 1
        return self._cid-1

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