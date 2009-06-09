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

#ifndef HTTP_H_
#define HTTP_H_

#include <fcgio.h>
#include <fcgi_config.h>


namespace avmshell
{
	class HTTPClass : public ClassClosure
	{
	public:

		HTTPClass(VTable* cvtable);

		void write(Stringp content);

		void logError(Stringp msg);

		ArrayObject* listEnv();

		Stringp getEnv(Stringp key);

		bool getOutputStarted();


		//fcgi request
		//static FCGX_Request *request;
		static bool outputStarted;
		static void sWrite(const char* str);

	private:

		//edit this value to accept bigger POST data
		static const unsigned long STDIN_MAX = 1000000; // 1MB

		long int readPost(char ** content);


	};
}

#endif /* HTTP_H_ */
