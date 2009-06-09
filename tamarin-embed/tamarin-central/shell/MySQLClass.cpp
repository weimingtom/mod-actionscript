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

#define CSTR(STRINGP) ((StUTF8String) STRINGP).c_str()

namespace avmshell
{
	MySQLClass::MySQLClass(VTable *cvtable)	: ClassClosure(cvtable)
    {
		createVanillaPrototype();

		mysql = mysql_init(mysql);
		if (!mysql)
			core()->console << "Error initialising MYSQL\n";
	}

	MySQLClass::~MySQLClass()
	{
		if (mysql)
			mysql_close(mysql);
	}

	//convert from UTF16 to UTF8
	/*const char* MySQLClass::CSTR(Stringp s)
	{
		StUTF8String sUTF8(s);
		return sUTF8.c_str();
		//return s->toUTF8String()->c_str();
	}*/

	bool MySQLClass::connect(Stringp host, Stringp dbName, Stringp user, Stringp pass)
	{
		if (!mysql_real_connect(mysql,
				CSTR(host),
				CSTR(user),
				CSTR(pass),
				CSTR(dbName),
				0, NULL, CLIENT_FOUND_ROWS))
		{
			return false;
		}

		if (mysql_set_character_set(mysql, "utf8"))
		{
			core()->console << "Error connecting to MySQL: can't set UTF-8 character set.\n";
			return false;
		}

		return true;
	}

	bool MySQLClass::ping()
	{
		return mysql_ping(mysql) == 0;
	}



	ArrayObject* MySQLClass::fetch(Stringp sql)
	{
		if (!sql)
			toplevel()->throwArgumentError(kNullArgumentError, "sql");

		if (!mysql) return NULL;

		StUTF8String tmp(sql);

		if (mysql_real_query(mysql, tmp.c_str(), tmp.length()))
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
			Stringp s = fields[i].name ? core->internStringUTF8(fields[i].name, fields[i].name_length) : (Stringp) core->kEmptyString;
			oRow->setIntProperty(i, s->atom());
		}


		int k = 1;
		MYSQL_ROW row;

		while ((row = mysql_fetch_row(res)))
		{
			//ScriptObject* oRow = this->toplevel()->objectClass->construct();
			ArrayObject *oRow = this->toplevel()->arrayClass->newArray();

			unsigned long *lengths;
			lengths = mysql_fetch_lengths(res);

			for (uint i=0; i<numFields; i++)
			{
				//if (IS_NUM(fields[i].type))
				//At the moment, the value is always of type String (UTF8 Compatible)
				oRow->setIntProperty(i, row[i] ? core->internStringUTF8(row[i], lengths[i])->atom() : NULL);
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

		StUTF8String tmp(sql);
		if (mysql_real_query(mysql, tmp.c_str(), tmp.length()))
			return -1;

		return (uint) mysql_affected_rows(mysql);
	}

	Stringp MySQLClass::escape(Stringp str)
	{
		if (!str)
			toplevel()->throwArgumentError(kNullArgumentError, "str");
		if (!mysql)
			return NULL;

		StUTF8String tmp(str);
		int len0 = tmp.length();
		char *res = new char[len0];//this is not null-terminated
		int len1 = mysql_real_escape_string(mysql, res, tmp.c_str(), len0);
		return core()->internStringUTF8(res, len1);
	}

	Stringp MySQLClass::getError()
	{

		if (mysql) return this->core()->newStringUTF8(mysql_error(mysql));

		return NULL;
	}
}

