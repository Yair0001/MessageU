import sqlite3
from datetime import datetime as dt
DATABASE_NAME = "defensive.db"

class DefensiveDb:
    def __init__(self):
        db = sqlite3.connect(DATABASE_NAME)
        self._cur = db.cursor()
        self._cid = 0

    def create_tables(self):
        self._cur.execute("CREATE TABLE clients(ID, UserName, PublicKey, LastSeen")
        self._cur.execute("CREATE TABLE messages(ID, ToClient, FromClient, Type, Content")

    def is_username_in_table(self, user_name):
        res = self._cur.execute(f"SELECT UserName FROM clients WHERE UserName={user_name}")
        return not(res.fetchone() is None)

    def insert_new_user(self,user_name,public_key):
        #create new unique id in length 16 bytes
        self._cur.execute(f"""
        INSERT INTO client
        VALUES({str(self._cid)},{user_name},{public_key},{dt.now()})
        """)
    def reset_db(self):
        self._cur.execute("DROP TABLE clients")
        self._cur.execute("DROP TABLE messages")
        self.create_tables()


if __name__ == "__main__":
    pass