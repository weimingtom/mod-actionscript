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
#include <libmysqlwrapped.h>


namespace avmshell
{
	class MySQLClass : public ClassClosure
	{
    public:
    	MySQLClass(VTable* cvtable);

    	//bool connect(Stringp host, Stringp db, Stringp user, Stringp pass);


    	bool connect(Stringp host, Stringp db, Stringp user, Stringp pass);
    	ArrayObject* fetch(Stringp sql);

    private:
    	std::string _s(Stringp s);
    	Database* db;
    };
}


#endif /* MYSQLCLASS_H_ */
