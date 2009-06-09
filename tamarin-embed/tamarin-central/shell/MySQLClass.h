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

#ifndef MYSQLCLASS_H_
#define MYSQLCLASS_H_


#include "avmshell.h"

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string>

namespace avmshell
{
	class MySQLClass : public ClassClosure
	{
    public:
    	/**
    	 * Simple mysql connection utility
    	 * we keep C++ part at a minimum, for flexibility.
    	 * the API presented to the programmers is written in the AS library
    	 */
    	MySQLClass(VTable* cvtable);

    	~MySQLClass();


    	/**
    	 * Create database connection
    	 * TODO: we should use a connection pool.
    	 * at this stage, none is used, although mysqlwrapped offers a solution
    	 *
    	 * we always use utf-8 as charset
    	 */
    	bool connect(Stringp host, Stringp db, Stringp user, Stringp pass);

    	/**
    	 * fetch the result of a query into an array.
    	 * first row contains the column names
    	 */
    	ArrayObject* fetch(Stringp sql);

    	/**
    	 * execute a query that doesn't return data (insert, update, etc)
    	 * we only return affected rows
    	 */
    	int exec(Stringp sql);

    	/**
    	 * escape the input to be safely concatenated into the query
    	 */
    	Stringp escape(Stringp s);

    	/**
    	 * For simplicity, we throw errors from AS library.
    	 * check if this returns something to see if there was any error
    	 */
    	Stringp getError();

    	/**
    	 * Check if we are still connected
    	 * if disconnected, ping tries to reconnect
    	 * if connected or reconnected, returns true
    	 */
    	bool ping();

    private:

    	MYSQL *mysql;
    	const char *strConv(Stringp str);
    };
}


#endif /* MYSQLCLASS_H_ */
