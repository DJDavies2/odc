#!/usr/bin/env python 

import sys
sys.path.append('/tmp/build/bundle/debug/odb_api/src/python/odb')

import unittest
from odb import *


TEST_DDL = """
            CREATE TYPE bf AS (f1 bit1, f2 bit2); 
            
            CREATE TABLE foo AS (
                x INTEGER, 
                y DOUBLE, 
                v STRING, 
                status bf
            ) ON 'new_api_example_python.odb';
"""

TEST_INSERT = """INSERT INTO foo (x,y,v,status) VALUES (?,?,?,?);"""
TEST_SELECT = """SELECT * FROM foo;"""

class TestODBQL(unittest.TestCase):

    def setUp(self): 
        pass

    def tearDown(self):
        pass

    def test_insert_data(self):
        db, stmt, tail = c_voidp(), c_voidp(), c_char_p()

        rc = odbql_open(TEST_DDL, byref(db))
        self.assertEqual(rc, ODBQL_OK)

        rc = odbql_prepare_v2(db, TEST_INSERT, -1, byref(stmt), byref(tail))
        self.assertEqual(rc, ODBQL_OK)
        
        for i in range(4): 
            rc = odbql_bind_null(stmt, i)
            self.assertEqual(rc, ODBQL_OK)

        rc = odbql_step(stmt)
        self.assertEqual(rc, ODBQL_ROW)

        for i in range(4): 
            rc = odbql_bind_int(stmt, 0, 1 * i)
            self.assertEqual(rc, ODBQL_OK)
            rc = odbql_bind_double(stmt, 1, c_double(0.1 * i))
            self.assertEqual(rc, ODBQL_OK)
            rc = odbql_bind_text(stmt, 2, "hello", len("hello"), ODBQL_STATIC)
            self.assertEqual(rc, ODBQL_OK)
            rc = odbql_bind_int(stmt, 3, 3 * i)
            self.assertEqual(rc, ODBQL_OK)
            rc = odbql_step(stmt)
            self.assertEqual(rc, ODBQL_ROW)
        
        rc = odbql_finalize(stmt)
        self.assertEqual(rc, ODBQL_OK)
        rc = odbql_close(db)
        self.assertEqual(rc, ODBQL_OK)


    def test_select_data(self):
        db, stmt, tail = c_voidp(), c_voidp(), c_char_p()

        rc = odbql_open("CREATE TABLE foo ON 'new_api_example_python.odb';", byref(db))
        self.assertEqual(rc, ODBQL_OK)

        rc = odbql_prepare_v2(db, "SELECT ALL * FROM foo;", -1, byref(stmt), byref(tail))
        self.assertEqual(rc, ODBQL_OK)

        number_of_rows = 0 
        rc = None
        while rc <> ODBQL_DONE:

            if number_of_rows == 0 or rc == ODBQL_METADATA_CHANGED:
                number_of_columns = odbql_column_count(stmt)
                print 'number_of_columns=', number_of_columns
                for i in range(0,number_of_columns):
                    print i, odbql_column_name(stmt, i) + ':' + type_name(odbql_column_type(stmt, i))
            else:
                self.assertEqual(rc, ODBQL_ROW)

                def value(column):
                    v = odbql_column_value(stmt, column)
                    if not v: return None
                    else: 
                        t = odbql_column_type(stmt, column)
                        if t == ODBQL_FLOAT: return odbql_value_double(v)
                        if t == ODBQL_INTEGER: return odbql_value_int(v)
                        if t == ODBQL_NULL: return None
                        #ODBQL_BLOB     = 4
                        #ODBQL_NULL     = 5
                        #ODBQL_TEXT     = 3
                        return odbql_column_text(stmt, column)

                print ','.join([str(value(column)) for column in range(number_of_columns)])
            number_of_rows += 1
            rc = odbql_step(stmt)


        rc = odbql_finalize(stmt)
        self.assertEqual(rc, ODBQL_OK)
        rc = odbql_close(db)
        self.assertEqual(rc, ODBQL_OK)


    def test_bitfields(self): # ODB-97
        db, stmt, tail = c_voidp(), c_voidp(), c_char_p()
        rc = odbql_open( """create table atovs on 'ATOVS.trimmed.odb';""", byref(db))
        self.assertEqual(rc, ODBQL_OK)
        rc = odbql_prepare_v2(db, '''select distinct qcinfo_1dvar, from atovs order by 1;''', -1, byref(stmt), byref(tail))
        self.assertEqual(rc, ODBQL_OK)
        
        rc = None
        values = []
        while rc <> ODBQL_DONE:
            v = odbql_column_value(stmt, 0)
            values.append(odbql_value_int(v))
            rc = odbql_step(stmt)

        rc = odbql_finalize(stmt)
        self.assertEqual(rc, ODBQL_OK)
        rc = odbql_close(db)
        self.assertEqual(rc, ODBQL_OK)

        expected = [0,1,4,5,8,9,12,13,130,131,134,135,138,139,142,143,642]
        self.assertEqual(values, expected)

    def test_stored_procedure(self):
        db, stmt, tail = c_voidp(), c_voidp(), c_char_p()

        rc = odbql_open("CREATE TABLE foo ON 'new_api_example_python.odb';", byref(db))
        self.assertEqual(rc, ODBQL_OK)

        e = """ { compare, left = new_api_example_python.odb, right = new_api_example_python.odb } ; """
        rc = odbql_prepare_v2(db, e, -1, byref(stmt), byref(tail))
        self.assertEqual(rc, ODBQL_OK)

        rc = odbql_step(stmt)
        self.assertEqual(rc, ODBQL_DONE)

        print 'test_stored_procedure: OK!'



class TestPEP249(unittest.TestCase):

    def setUp(self):
        self.data = [[1,0.1, '  one   ', 1],
                     [2,0.2, '  two   ', 2],
                     [3,0.3, '  three ', 3],
                     [4,0.4, '  four  ', 4],
                    ]
        self.conn = connect(TEST_DDL)

    def test_insert_data(self):
        c = self.conn.cursor()
        print self.data
        c.executemany(TEST_INSERT, self.data)


    def read_with_legacy_api(self, file_name = 'new_api_example_python.odb'):
        return  new_open(file_name)

    def test_select_data_fetchone(self):

        legacy = self.read_with_legacy_api()
        print 'legacy:', legacy

        c = self.conn.cursor()
        c.execute(TEST_SELECT)
        number_of_rows = 0
        while True:
            row = c.fetchone()
            if row is None:
                break

            original = self.data [number_of_rows]
            # Legacy API does not handle bitfields correctly
            #self.assertEqual (original, legacy [number_of_rows])
            self.assertEqual (original, row)

            print row
            number_of_rows += 1

        self.assertEqual ( number_of_rows, 4 )


    def test_select_data_fetchall(self):
        """"""
        c = self.conn.cursor()
        c.execute(TEST_SELECT)
        rows = [r for r in c.fetchall()]
        self.assertEqual ( len(rows), 4 )
        self.assertEqual ( rows, self.data )

    def test_select_data_iterate(self):
        """https://www.python.org/dev/peps/pep-0249/#iter"""
        c = self.conn.cursor()
        c.execute(TEST_SELECT)
        self.assertEqual ( self.data,  [r for r in c] )

    def test_callproc(self):
        """https://www.python.org/dev/peps/pep-0249/#callproc"""
        c = self.conn.cursor()
        rc = c.callproc('compare', left = 'new_api_example_python.odb', right = 'new_api_example_python.odb')
        # TODO: check rc

    def test_bitfields(self): # ODB-97
        # !odb sql select qcflags_info_1dvar    -i ATOVS.trimmed.odb | sort | uniq
        expected = [0,1,4,5,8,9,12,13,130,131,134,135,138,139,142,143,642]

        conn = odb.connect("""create table atovs on 'ATOVS.trimmed.odb';""")
        c = conn.cursor()
        c.execute('''select distinct qcinfo_1dvar, from atovs order by 1''')
        actual = [r[0] for r in c.fetchall()]
        self.assertEqual(actual, expected) 

"""
    def test_select_data_from_mars(self):
        conn = connect(ddl = '''
            CREATE TABLE foo 
            ON "mars://RETRIEVE,
                        DATABASE  = marsod,
                        CLASS     = OD,
                        TYPE      = MFB,
                        STREAM    = OPER,
                        EXPVER    = 0001,
                        DATE      = 20160830,
                        TIME      = 1200,
                        REPORTYPE = 16001 "''')
        c = conn.cursor()
        c.execute('SELECT * from foo;')
        data = c.fetchall()
        self.assertEqual(len(data), 4438) 

        c.execute('SELECT * from foo')
        data = c.fetchall()
        self.assertEqual(len(data), 4438) 
"""


if __name__ == '__main__':
    unittest.main()
