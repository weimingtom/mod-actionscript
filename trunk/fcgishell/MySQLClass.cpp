/*
 * MySQLClass.cpp
 *
 *  Created on: Dec 19, 2008
 *      Author: victor
 */

#include "MySQLClass.h"

#include "fcgishell/avmshell.h"
#include <iostream>


#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string>
#include <libmysqlwrapped.h>



using namespace std;

namespace avmshell
{
	MySQLClass::MySQLClass(VTable *cvtable)	: ClassClosure(cvtable)
    {
		createVanillaPrototype();
	}


	string MySQLClass::_s(Stringp s)
	{
		return string(s->toUTF8String()->c_str());
	}

	bool MySQLClass::connect(Stringp host, Stringp dbName, Stringp user, Stringp pass)
	{
		db = new Database(_s(host), _s(user), _s(pass), _s(dbName));
		//mysql_set_character_set()
		return true;
	}



	ArrayObject* MySQLClass::fetch(Stringp sql)
	{
		AvmCore* core = this->core();

		//cout << _s(sql) << "<hr>";

		//Database db("localhost","root","telita","tamarin");
		Query q(*db);


		//MYSQL_RES *res = q.get_result("select * from tbl1");
		MYSQL_RES *res = q.get_result(_s(sql));

		//output array
		ArrayObject *out = this->toplevel()->arrayClass->newArray(q.num_rows());

		uint numFields = mysql_num_fields(res);
		MYSQL_FIELD* fields = mysql_fetch_fields(res);

		//get field names
		if (res)
		{
			ArrayObject *oRow = this->toplevel()->arrayClass->newArray();
			out->setIntProperty(0, oRow->atom());

			for (uint i=0; i<numFields; i++)
			{
				Stringp s = fields[i].name ? core->newString(fields[i].name) : (Stringp) core->kEmptyString;
				oRow->setIntProperty(i, s->atom());
			}


			/*MYSQL_FIELD *f;
			mysql_field_seek(res, 0);
			uint fld = 0;
			while ((f = mysql_fetch_field(res)))
			{
				Stringp s = f->name ? core->newString(f->name) : core->newString("");
				oRow->setIntProperty(fld++, s->atom());
			}*/
		}

		int k = 1;
		while (q.fetch_row())
		{
			//long id = q.getval();

			//ScriptObject* oRow = this->toplevel()->objectClass->construct();
			ArrayObject *oRow = this->toplevel()->arrayClass->newArray();

			for (uint i=0; i<numFields; i++)
			{
				/*if (IS_NUM(fields[i].type))
				{
					oRow->setIntProperty(i, core->doubleToAtom(atof(q.getstr())));
				}
				else*/
				{
					//for unknown reason, can't assign a value in a string key
					//oRow->setStringProperty(n, v->atom());
					oRow->setIntProperty(i, core->newString(q.getstr())->atom());
				}
			}

			out->setIntProperty(k++, oRow->atom());
		}
		q.free_result();



		//Example array
		//ArrayObject *out = this->toplevel()->arrayClass->newArray(30);
		//for (int i=0; i<30; i++)
		//	out->setIntProperty(i, core->newString("aaa")->atom());

		return out;
	}
}

