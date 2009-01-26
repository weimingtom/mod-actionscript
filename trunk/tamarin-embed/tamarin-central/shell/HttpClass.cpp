/*
 * Http.cpp
 *
 *  Created on: Jan 18, 2009
 *      Author: victor
 */

#import "avmshell.h"


#include <stdlib.h>
#include <iostream>

//to read post/get data
#include <fcgio.h>
#include <fcgi_config.h>


using namespace std;

extern FCGX_Request httpRequest;

namespace avmshell
{

	HttpClass::HttpClass(VTable *cvtable) : ClassClosure(cvtable)
    {
		createVanillaPrototype();
    }

	void HttpClass::write(Stringp content)
	{
		cout << content->toUTF8String()->c_str();
	}

	void HttpClass::logError(Stringp msg)
	{
		cerr << msg->toUTF8String()->c_str();
	}

	Stringp HttpClass::postData()
	{
		AvmCore* core = this->core();

		char *content;
		unsigned long clen = readPost(&content);

		Stringp s = core->newString(content, clen);
		delete content;

		return s;
	}

	//read stdin
	long HttpClass::readPost(char ** content)
	{
	    char * clenstr = FCGX_GetParam("CONTENT_LENGTH", httpRequest.envp);
	    unsigned long clen = STDIN_MAX;

	    if (clenstr)
	    {
	        clen = strtol(clenstr, &clenstr, 10);
	        if (*clenstr)
	        {
	            cerr << "can't parse \"CONTENT_LENGTH="
	                 << FCGX_GetParam("CONTENT_LENGTH", httpRequest.envp)
	                 << "\"\n";
	            clen = STDIN_MAX;
	        }

	        // *always* put a cap on the amount of data that will be read
	        if (clen > STDIN_MAX) clen = STDIN_MAX;

	        *content = new char[clen];

	        cin.read(*content, clen);
	        clen = cin.gcount();
	    }
	    else
	    {
	        // *never* read stdin when CONTENT_LENGTH is missing or unparsable
	        *content = 0;
	        clen = 0;
	    }

	    // Chew up any remaining stdin - this shouldn't be necessary
	    // but is because mod_fastcgi doesn't handle it correctly.

	    // ignore() doesn't set the eof bit in some versions of glibc++
	    // so use gcount() instead of eof()...
	    do cin.ignore(1024); while (cin.gcount() == 1024);

	    return clen;
	}


	ArrayObject* HttpClass::listEnv()
	{
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();
		uint i = 0;

		const char * const * envp = httpRequest.envp;

	    for ( ; *envp; ++envp)
	    {
	    	out->setUintProperty(i++, core->newString(*envp)->atom());
	    }
	    return out;
	}

	Stringp HttpClass::getEnv(Stringp key)
	{
		char *s = FCGX_GetParam(key->toUTF8String()->c_str(), httpRequest.envp);
		return this->core()->newString(s);
	}
}

/*
static void penv(const char * const * envp)
{
    cout << "<PRE>\n";
    for ( ; *envp; ++envp)
    {
    	cout << *envp << "\n";
    }
    cout << "</PRE>\n";
}
*/
