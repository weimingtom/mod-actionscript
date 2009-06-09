/*
 * MultipartClass.cpp
 *
 *  Created on: Apr 10, 2009
 *      Author: victor
 */

#import "avmshell.h"

using namespace std;
using namespace modas;

extern FCGX_Request httpRequest;


namespace avmshell
{

	/*static*/
	modas::Sapi* HTTPFormClass::sapi = 0;

	HTTPFormClass::HTTPFormClass(VTable *cvtable) : ClassClosure(cvtable)
    {
		createVanillaPrototype();
    }

	Stringp HTTPFormClass::rawPostData()
	{
		AvmCore* core = this->core();
		Stringp s = core->newStringUTF8(sapi->postData.data(), sapi->postData.length());
		return s;
	}

	ArrayObject* HTTPFormClass::convertForm(std::vector<FormEntry>& vec)
	{
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();

		for (std::vector<FormEntry>::iterator it=vec.begin(); it!=vec.end(); it++)
		{
	    	ArrayObject* arr;
	    	Stringp key = core->internStringUTF8((*it).name.data(), (*it).name.length());
	    	Stringp value = core->internStringUTF8((*it).value.data(), (*it).value.length());
	    	//we have it, append
	    	if (out->hasStringProperty(key))
	    	{
	    		arr = (ArrayObject*) core->atomToScriptObject(out->getStringProperty(key));
		    	arr->setUintProperty(arr->get_length(), value->atom());
	    	}
	    	//create array
	    	else
	    	{
		    	arr = this->toplevel()->arrayClass->newArray();
		    	arr->setUintProperty(0, value->atom());
		    	out->setStringProperty(key, arr->atom());
	    	}
		}
		return out;
	}

	ArrayObject* HTTPFormClass::getPOST()
	{
		return convertForm(sapi->postFields);
	}

	ArrayObject* HTTPFormClass::getGET()
	{
		return convertForm(sapi->getFields);
	}



	ArrayObject* HTTPFormClass::listFileKeys()
	{
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();

		for (std::vector<FormFile>::iterator it=sapi->files.begin(); it!=sapi->files.end(); it++)
		{
	    	out->setUintProperty(out->get_length(), core->newStringUTF8((*it).name.data(), (*it).name.length())->atom());
		}
		return out;
	}

	bool HTTPFormClass::isUploaded(Stringp key)
	{
		string sKey(((StUTF8String) key).c_str());
		for (std::vector<FormFile>::iterator it=sapi->files.begin(); it!=sapi->files.end(); it++)
			if (sKey == (*it).name) return true;
		return false;
	}


	Stringp HTTPFormClass::getFilename(Stringp key)
	{
		AvmCore *core = this->core();

		string sKey(((StUTF8String) key).c_str());

		for (std::vector<FormFile>::iterator it=sapi->files.begin(); it!=sapi->files.end(); it++)
			if (sKey == (*it).name) return core->newStringUTF8((*it).filename.data(), (*it).filename.length());
		return NULL;
	}

	uint HTTPFormClass::getFileSize(Stringp key)
	{
		//AvmCore *core = this->core();
		string sKey(((StUTF8String) key).c_str());

		for (std::vector<FormFile>::iterator it=sapi->files.begin(); it!=sapi->files.end(); it++)
			if (sKey == (*it).name) return (*it).data.length();
		return 0;
	}

	void HTTPFormClass::saveFile(Stringp key, Stringp destPath)
	{
		string sKey(((StUTF8String) key).c_str());

		for (std::vector<FormFile>::iterator it=sapi->files.begin(); it!=sapi->files.end(); it++)
		{
			if (sKey == (*it).name)
			{
				StUTF8String filenameUTF8(destPath);
				File* fp = Platform::GetInstance()->createFile();
				if (!fp || !fp->open(filenameUTF8.c_str(), File::OPEN_WRITE))
				{
					if(fp)
					{
						Platform::GetInstance()->destroyFile(fp);
					}
					toplevel()->throwError(kFileWriteError, destPath);
				}

				if ((int32_t)fp->write((*it).data.data(), (*it).data.length()) != (int32_t)(*it).data.length())
					toplevel()->throwError(kFileWriteError, destPath);

				fp->close();
				Platform::GetInstance()->destroyFile(fp);
			}
		}
	}

	HTTPFormClass::~HTTPFormClass()
	{
	}
}
