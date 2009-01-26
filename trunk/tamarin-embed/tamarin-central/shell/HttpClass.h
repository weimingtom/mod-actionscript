/*
 * Http.h
 *
 *  Created on: Jan 18, 2009
 *      Author: victor
 */

#ifndef HTTP_H_
#define HTTP_H_

#include <fcgio.h>
#include <fcgi_config.h>


namespace avmshell
{
	class HttpClass : public ClassClosure
	{
	public:

		HttpClass(VTable* cvtable);

		void write(Stringp content);

		void logError(Stringp msg);

		Stringp postData();

		ArrayObject* listEnv();

		Stringp getEnv(Stringp key);


		//fcgi request
		static FCGX_Request *request;

	private:

		//edit this value to accept bigger POST data
		static const unsigned long STDIN_MAX = 1000000; // 1MB

		long int readPost(char ** content);
	};
}

#endif /* HTTP_H_ */
