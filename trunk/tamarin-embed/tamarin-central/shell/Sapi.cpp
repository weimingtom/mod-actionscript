/*
Sapi.cpp
Created on: May 13, 2009
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. *
*/

#include "avmshell.h"
#include <cstdlib>

namespace modas
{

	Sapi::Sapi(avmplus::PrintWriter &con, FCGX_Request &rq) : console(con), request(rq)
	{
		isError = false;

		console << "construct Sapi\n";
		bool isPost = false;

		char* method = FCGX_GetParam("REQUEST_METHOD", request.envp);
		if (strcasecmp(method, "POST") == 0)
		{
			//we have a post, read posted data directly from request.in
			char* clen = FCGX_GetParam("CONTENT_LENGTH", request.envp);
			int len = std::atol(clen);
			int maxPostLength = 100000000;//~100M
			if (len > maxPostLength)
			{
				console << "Max post data length exceeded\n";
				console << "CONTENT_LENGTH: " << len << "\n";
				return;
			}

			if (len >= 0)
			{
				std::vector<char> data(len);
				int ch;
				for (int i=0; i<len; i++)
				{
					if ((ch = FCGX_GetChar(request.in)) < 0)
					{
						isError = true;
						console << "I/O Error\n";
						return;
					}
					data[i] = (char)ch;
				}
				isPost = true;
				postData = std::string(&data[0], len);
				//console << postData.c_str() << "\n";
			}
		}

		if (isPost)
		{
			char* contentType = FCGX_GetParam("CONTENT_TYPE", request.envp);
			if (contentType == NULL || strcasecmp("application/x-www-form-urlencoded", contentType)==0)
			{
				//console << "url encoded\n";
				parseUrlEncoded(postData, postFields);
			}
			else if (strncasecmp("multipart/form-data", contentType, 19)==0)
			{
				//console << "multipart\n";
				parseMultipart(contentType);
			}
			//console << contentType << "\n";
		}

		std::string query = FCGX_GetParam("QUERY_STRING", request.envp);
		parseUrlEncoded(query, getFields);

		for (std::vector<FormEntry>::iterator it=postFields.begin(); it!=postFields.end(); it++)
			console << "post " << (*it).name.c_str() << "\n";
		for (std::vector<FormEntry>::iterator it=getFields.begin(); it!=getFields.end(); it++)
			console << "get " << (*it).name.c_str() << "\n";

		for (std::vector<FormFile>::iterator it=files.begin(); it!=files.end(); it++)
			console << "file " << (*it).name.c_str() << " " << (*it).data.length() <<"\n";

	}

	void Sapi::parseMultipart(std::string contentType)
	{
		// Find out what the separator is
		std::string 		bType 	= "boundary=";
		std::string::size_type 	pos = contentType.find(bType);

		// generate the separators
		std::string sep1 = contentType.substr(pos + bType.length());
		sep1.append("\r\n");
		sep1.insert(0, "--");

		std::string sep2 = contentType.substr(pos + bType.length());
		sep2.append("--\r\n");
		sep2.insert(0, "--");

		// Find the data between the separators
		std::string::size_type start  = postData.find(sep1);
		std::string::size_type sepLen = sep1.length();
		std::string::size_type oldPos = start + sepLen;

		while(true)
		{
			pos = postData.find(sep1, oldPos);

			// If sep1 wasn't found, the rest of the data is an item
			if(std::string::npos == pos)
				break;

			// parse the data
			parseMime(postData.substr(oldPos, pos - oldPos));

			// update position
			oldPos = pos + sepLen;
		}

		// The data is terminated by sep2
		pos = postData.find(sep2, oldPos);
		// parse the data, if found
		if(std::string::npos != pos)
			parseMime(postData.substr(oldPos, pos - oldPos));
	}

	void Sapi::parseMime(std::string data)
	{
		// Find the header
		std::string end = "\r\n\r\n";
		std::string::size_type headLimit = data.find(end, 0);

		// Detect error
		if(std::string::npos == headLimit)
		{
			console << "Malformed input\n";
			isError = true;
			return;
		}

		// Extract the value - there is still a trailing CR/LF to be subtracted off
		std::string::size_type valueStart = headLimit + end.length();
		std::string value = data.substr(valueStart, data.length() - valueStart - 2);

		std::string disposition;
		disposition = extractBetween(data, "Content-Disposition: ", ";");

		std::string name;
		name = extractBetween(data, "name=\"", "\"");

		std::string filename;
		filename = extractBetween(data, "filename=\"", "\"");
		filename = urlDecode(filename);

		std::string type;
		type = extractBetween(data, "Content-Type: ", "\r\n\r\n");

		if(filename.empty())
		{
			FormEntry* entry = new FormEntry;
			entry->name = name;
			entry->value = value;
			postFields.push_back(*entry);
		}
		else
		{
			FormFile* file = new FormFile;
			file->name = name;
			file->dataType = type;
			file->filename = filename;
			file->data = value;
			files.push_back(*file);
		}
	}


	/*static*/
	void Sapi::parseUrlEncoded(std::string &data, std::vector<FormEntry> &vec)
	{
		std::string::size_type pos;
		std::string::size_type oldPos = 0;

		while(true)
		{
			pos = data.find_first_of('=', oldPos);
			if(std::string::npos == pos) break;

			FormEntry* entry = new FormEntry;
			entry->name = Sapi::urlDecode(data.substr(oldPos, pos - oldPos));
			oldPos = ++pos;

			pos = data.find_first_of('&', oldPos);

			// Even if an '&' wasn't found the rest of the string is a value
			entry->value = Sapi::urlDecode(data.substr(oldPos, pos - oldPos));

			// Store the pair
			vec.push_back(*entry);

			if(std::string::npos == pos)
			break;

			oldPos = ++pos;
		}
	}

	/*static*/
	std::string Sapi::urlDecode(const std::string& src)
	{
		std::string result;
		std::string::const_iterator iter;
		char c;

		for(iter = src.begin(); iter != src.end(); ++iter)
		{
			switch(*iter)
			{
				case '+':
					result.append(1, ' ');
					break;
				case '%':
					// Don't assume well-formed input
					if(std::distance(iter, src.end()) >= 2
							&& std::isxdigit(*(iter + 1)) && std::isxdigit(*(iter + 2)))
					{
						c = *++iter;
						result.append(1, hexToChar(c, *++iter));
					}
					// Just pass the % through untouched
					else
					{
						result.append(1, '%');
					}
				break;
				default:
					result.append(1, *iter);
					break;
			}
		}
		return result;
	}

	// return data between separators
	/*static*/
	std::string Sapi::extractBetween(const std::string& data,
				  const std::string& separator1,
				  const std::string& separator2)
	{
		std::string result;
		std::string::size_type start, limit;

		start = data.find(separator1, 0);
		if(std::string::npos != start)
		{
			start += separator1.length();
			limit = data.find(separator2, start);
			if(std::string::npos != limit)
				result = data.substr(start, limit - start);
		}
		return result;
	}

	/*static*/
	char Sapi::hexToChar(char first, char second)
	{
	  int digit;

	  digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
	  digit *= 16;
	  digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
	  return static_cast<char>(digit);
	}

	Sapi::~Sapi()
	{
		console << "destruct Sapi\n";
	}
}
