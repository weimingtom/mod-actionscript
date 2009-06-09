/*
Created on: jan 18, 2009
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
	bool HTTPClass::outputStarted = false;

	HTTPClass::HTTPClass(VTable *cvtable) : ClassClosure(cvtable)
    {
		createVanillaPrototype();
    }

	bool HTTPClass::getOutputStarted()
	{
		return HTTPClass::outputStarted;
	}

	void HTTPClass::write(Stringp content)
	{
		StUTF8String str(content);
		HTTPClass::sWrite(str.c_str());
	}

	void HTTPClass::sWrite(const char* str)
	{
		HTTPClass::outputStarted = true;
		cout << str;
	}

	void HTTPClass::logError(Stringp msg)
	{
		core()->console << msg << "\n";
	}


	ArrayObject* HTTPClass::listEnv()
	{
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();
		uint i = 0;

		const char * const * envp = httpRequest.envp;

	    for ( ; *envp; ++envp)
	    {
	    	out->setUintProperty(i++, core->internStringUTF8(*envp)->atom());
	    }
	    return out;
	}

	Stringp HTTPClass::getEnv(Stringp key)
	{
		StUTF8String str(key);
		char *s = FCGX_GetParam(str.c_str(), httpRequest.envp);
		return this->core()->newStringUTF8(s);
	}
}
