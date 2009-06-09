/*
 * HTTPFormClass.h
 *
 *  Created on: Apr 10, 2009
 *      Author: victor
 */

#ifndef MULTIPART_H_
#define MULTIPART_H_

//CGICC
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"

using namespace std;
using namespace cgicc;


//Link the cgicc library to the fcgi data
//we need cgicc only for file uploads
class FCgiInput : public CgiInput
{
public:
	FCgiInput(FCGX_Request rq) : request(rq) {}
	//copy constructor seems to be not used at all (not the assign operator)
	/*FCgiInput(FCgiInput& input):CgiInput(input), request(input.request) {}*/
	virtual size_t read(char *data,	size_t length)
	{
		cin.read(data, length); return cin.gcount();
	}

	virtual string getenv(const char *varName)
	{
		char* val = FCGX_GetParam(varName, request.envp);
		return (val == 0) ? string("") : val;
	}

private:
	FCGX_Request& request;
};


namespace avmshell
{
	class HTTPFormClass : public ClassClosure
	{
	public:

		HTTPFormClass(VTable* cvtable);


		/**
		 * Unidimensional Array of Strings key,value,key,value...
		 */
		ArrayObject* getFields();

		/**
		 * List uploaded files field names.
		 */
		ArrayObject* listFileKeys();

		/**
		 * Filename of uploaded file, as sent by the client
		 */
		Stringp getFilename(Stringp key);


		/**
		 * Check if file was correctly uploaded
		 */
		bool isUploaded(Stringp key);

		/**
		 * Save uploaded file to disk
		 */
		bool saveFile(Stringp key, Stringp destPath);

		~HTTPFormClass();

	private:
		bool inited;
		void init();
		FCgiInput* input;
		cgicc::Cgicc* cgi;
	};
}


#endif /* MULTIPART_H_ */
