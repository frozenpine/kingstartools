# coding: utf-8
import sybpydb

if __name__ == "__main__":
    try:
        conn = sybpydb.connect(
            user='sa', 
            password='sybase15', 
            servername='SYBASE15'
        )

        cur = conn.cursor()
    except Exception as e:
        print(e)
        exit(1)

    cur.execute('use master')
    cur.execute('SELECT * FROM ijdbc_function_escapes')

    header = (col[0] for col in cur.description)
    print("\t".join(header))

    for row in cur.fetchall():
        print("\t".join(str(col) for col in row))

    conn.close()
