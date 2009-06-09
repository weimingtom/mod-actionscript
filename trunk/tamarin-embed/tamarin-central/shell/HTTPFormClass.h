/*
 * HTTPFormClass.h
 *
 *  Created on: Apr 10, 2009
 *      Author: victor
 */

#ifndef MULTIPART_H_
#define MULTIPART_H_


#include "Sapi.h"

using namespace std;


namespace avmshell
{
	class HTTPFormClass : public ClassClosure
	{
	public:

		HTTPFormClass(VTable* cvtable);


		/**
		 * Map of form POST data (object of strings or arrays of string)
		 */
		ArrayObject* getPOST();

		/**
		 * Map of form GET data (object of strings or arrays of string)
		 */
		ArrayObject* getGET();


		Stringp rawPostData();


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
		 * Get the uploaded file size
		 */
		uint getFileSize(Stringp key);

		/**
		 * Save uploaded file to disk
		 */
		void saveFile(Stringp key, Stringp destPath);

		static modas::Sapi* sapi;

		~HTTPFormClass();

	private:
		ArrayObject* convertForm(std::vector<modas::FormEntry>& vec);
	};
}


#endif /* MULTIPART_H_ */
