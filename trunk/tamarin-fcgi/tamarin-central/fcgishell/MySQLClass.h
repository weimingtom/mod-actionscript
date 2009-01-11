/*
 * MySQLClass.h
 *
 *  Created on: Dec 19, 2008
 *      Author: victor
 */

#ifndef MYSQLCLASS_H_
#define MYSQLCLASS_H_


#include "fcgishell/avmshell.h"

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

    private:

    	MYSQL *mysql;
    	const char *strConv(Stringp str);
    };
}


#endif /* MYSQLCLASS_H_ */
