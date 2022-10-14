'''Initializes the tables used in the quality_assignment.py module.

pylint results:
************* Module initdb
initdb.py:24:0: W0702: No exception type(s) specified (bare-except)
initdb.py:8:9: W1514: Using open without explicitly specifying an encoding (unspecified-encoding)

------------------------------------------------------------------
Your code has been rated at 8.57/10 (previous run: 8.67/10, -0.10)

Not sure what sort of exceptions to catch, or what encoding to use to get these to 10/10'''

import sqlite3

def init_db(database_location):
    try:
        connection = sqlite3.connect(database_location)

        with open('schema.sql') as f:
            connection.executescript(f.read())

        cur = connection.cursor()

        cur.execute("PRAGMA foreign_keys=on")

        cur.execute("INSERT INTO users (username, email, user_pass) VALUES (?, ?, ?)",
                    ('admin', 'admin@msn.com', 'seed')
                    )
        
        cur.execute("INSERT INTO users (username, email, user_pass) VALUES (?, ?, ?)",
            ('bob', 'bob@gmail.com', 'bees')
            )

        cur.execute("INSERT INTO posts (title, author, content) VALUES (?, ?, ?)",
            ('Admin test post', 'admin', 'Admin''s first post!')
            )

        cur.execute("INSERT INTO posts (title, author, content) VALUES (?, ?, ?)",
                    ('Test post', 'bob', 'Bob''s first post!')
                    )

        connection.commit()
    except:
        print("Some error occurred creating sqlite3-database.db")
        if connection is not None:
            connection.close()

init_db('sqlite3-database.db')