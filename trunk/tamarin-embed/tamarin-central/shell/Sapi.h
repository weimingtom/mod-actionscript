/*
Sapi.h
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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef SAPI_H_
#define SAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcgio.h>
#include <fcgi_config.h>
#include <vector>

namespace modas
{

	struct FormEntry
	{
	public:
		std::string name;
		std::string value;
	};

	struct FormFile
	{
	public:
		std::string name;
		std::string filename;
		std::string dataType;
		std::string data;
		void writeToStream(std::ostream& out) const
		{
			out.write(data.data(), data.length());
		}
	};



	class Sapi
	{
	public:
		Sapi(avmplus::PrintWriter &con, FCGX_Request &rq);
		virtual ~Sapi();

		std::vector<FormEntry> getFields;
		std::vector<FormEntry> postFields;
		std::vector<FormFile> files;
		std::string postData;

		bool isError;

	private:

		static void parseUrlEncoded(std::string &data, std::vector<FormEntry> &vec);
		void parseMultipart(std::string contentType);
		void parseMime(std::string data);

		avmplus::PrintWriter& console;
		FCGX_Request& request;

		//utilities
		static std::string urlDecode(const std::string& src);
		static std::string extractBetween(const std::string& data,
				      const std::string& separator1,
				      const std::string& separator2);
		static char hexToChar(char first, char second);
	};

}

#endif /* SAPI_H_ */
