/*
Created on: Dec 19, 2008
Author(s): Victor Dramba

This program is made available under the terms of the GNU GPL.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "MySQLClass.h"

#include "avmshell.h"
#include <iostream>


#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string>


using namespace std;

namespace avmshell
{
	MySQLClass::MySQLClass(VTable *cvtable)	: ClassClosure(cvtable)
    {
		createVanillaPrototype();

		mysql = mysql_init(mysql);
		if (!mysql)
			cerr << "Error initialising MYSQL" << endl;
	}

	MySQLClass::~MySQLClass()
	{
		if (mysql)
			mysql_close(mysql);
	}

	//convert from UTF16 to UTF8
	const char* MySQLClass::strConv(Stringp s)
	{
		return s->toUTF8String()->c_str();
	}

	bool MySQLClass::connect(Stringp host, Stringp dbName, Stringp user, Stringp pass)
	{
		if (!mysql_real_connect(mysql,
				strConv(host),
				strConv(user),
				strConv(pass),
				strConv(dbName),
				0, NULL, CLIENT_FOUND_ROWS))
		{
			return false;
		}

		if (mysql_set_character_set(mysql, "utf8"))
		{
			cerr << "Error connecting to MySQL: can't set UTF-8 character set." << endl;
			return false;
		}

		return true;
	}



	ArrayObject* MySQLClass::fetch(Stringp sql)
	{
		if (!sql)
			toplevel()->throwArgumentError(kNullArgumentError, "sql");

		if (!mysql) return NULL;

		if (mysql_query(mysql, strConv(sql)))
			return NULL;


		//get field names
		MYSQL_RES *res = mysql_store_result(mysql);
		if (!res)
			return NULL;


		AvmCore* core = this->core();

		//output array
		ArrayObject *out = this->toplevel()->arrayClass->newArray(mysql_num_rows(res)+1);

		uint numFields = mysql_num_fields(res);
		MYSQL_FIELD* fields = mysql_fetch_fields(res);

		ArrayObject *oRow = this->toplevel()->arrayClass->newArray();
		out->setIntProperty(0, oRow->atom());

		//output field names in the top row
		for (uint i=0; i<numFields; i++)
		{
			Stringp s = fields[i].name ? core->newString(fields[i].name) : (Stringp) core->kEmptyString;
			oRow->setIntProperty(i, s->atom());
		}


		int k = 1;
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(res)))
		{
			//ScriptObject* oRow = this->toplevel()->objectClass->construct();
			ArrayObject *oRow = this->toplevel()->arrayClass->newArray();

			for (uint i=0; i<numFields; i++)
			{
				//if (IS_NUM(fields[i].type))
				//At the moment, the value is always of type String
				oRow->setIntProperty(i, row[i] ? core->newString(row[i])->atom() : NULL);
			}
			out->setIntProperty(k++, oRow->atom());
		}

		mysql_free_result(res);
		return out;
	}

	int MySQLClass::exec(Stringp sql)
	{
		if (!sql)
			toplevel()->throwArgumentError(kNullArgumentError, "sql");

		if (!mysql)
			return -1;

		const char *q = strConv(sql);

		if (mysql_real_query(mysql, q, strlen(q)))
			return -1;

		return (uint) mysql_affected_rows(mysql);
	}

	Stringp MySQLClass::escape(Stringp str)
	{
		if (!str)
			toplevel()->throwArgumentError(kNullArgumentError, "str");
		if (!mysql)
			return NULL;

		const char *src = strConv(str);
		int len0 = strlen(src);
		char *res = new char[len0 * 2 + 1];

		int len1 = mysql_real_escape_string(mysql, res, src, len0);
		res[len1] = '\0';
		Stringp sret = this->core()->newString(res);
		delete res;
		return sret;
	}

	Stringp MySQLClass::getError()
	{

		if (mysql)
			return this->core()->newString(mysql_error(mysql));

		return NULL;
	}
}

