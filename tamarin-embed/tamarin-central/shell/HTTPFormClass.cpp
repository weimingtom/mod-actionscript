/*
 * MultipartClass.cpp
 *
 *  Created on: Apr 10, 2009
 *      Author: victor
 */

#import "avmshell.h"

using namespace std;
using namespace cgicc;

extern FCGX_Request httpRequest;


namespace avmshell
{

	HTTPFormClass::HTTPFormClass(VTable *cvtable) : ClassClosure(cvtable)
    {
		createVanillaPrototype();
    }

	void HTTPFormClass::init()
	{
		input = new FCgiInput(httpRequest);
        cgi = new Cgicc(input);
        inited = true;
	}

	ArrayObject* HTTPFormClass::getFields()
	{
		if (!inited) init();
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();

		const vector<FormEntry> els = cgi->getElements();
		for (uint i=0; i<els.size(); i++)
		{
	    	out->setUintProperty(2*i, core->newString(els[i].getName().c_str())->atom());
	    	out->setUintProperty(2*i+1, core->newString(els[i].getValue().c_str())->atom());
		}

		return out;
	}

	ArrayObject* HTTPFormClass::listFileKeys()
	{
		if (!inited) init();
		AvmCore *core = this->core();
		ArrayObject *out = this->toplevel()->arrayClass->newArray();

		const vector<FormFile> files = cgi->getFiles();
		for (uint i=0; i<files.size(); i++)
		{
	    	out->setUintProperty(i, core->newString(files[i].getName().c_str())->atom());
		}
		return out;
	}

	bool HTTPFormClass::isUploaded(Stringp key)
	{
		if (!inited) init();

		string sKey(key->toUTF8String()->c_str());
		const_file_iterator file;
		file = cgi->getFile(sKey);
		return file != cgi->getFiles().end();
	}


	Stringp HTTPFormClass::getFilename(Stringp key)
	{
		if (!inited) init();
		AvmCore *core = this->core();

		string sKey(key->toUTF8String()->c_str());
		const_file_iterator file;
		file = cgi->getFile(sKey);
		if (file != cgi->getFiles().end())
			return core->newString((*file).getFilename().c_str());
		else
			return NULL;
	}


	bool HTTPFormClass::saveFile(Stringp key, Stringp destPath)
	{
		if (!inited) init();
		string sKey(key->toUTF8String()->c_str());
		const_file_iterator file;
		file = cgi->getFile(sKey);
		if (file == cgi->getFiles().end()) return false;

		ofstream fout(destPath->toUTF8String()->c_str());
		if (!fout) return false;
		(*file).writeToStream(fout);
		fout.close();
		return !fout.failbit;
	}

	HTTPFormClass::~HTTPFormClass()
	{
		if (inited)
		{
			delete input;
			delete cgi;
		}
	}
}
