/*
Created on: Dec 13, 2008
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


#include "avmshell.h"



#include <iostream>
#include <stdlib.h>

//fcgi headers
#include <fcgio.h>
#include <fcgi_config.h>

#include <string>

#include "HttpClass.h"

using namespace std;


//------------------------------------------------------------
// AVM
static MMgc::FixedMalloc* fm = NULL;

/*void *operator new(size_t size)
{
    return fm->Alloc(size);
}

void *operator new[](size_t size)
{
    return fm->Alloc(size);
}

void operator delete( void *p)
{
	if (fm)
		fm->Free(p);
}

void operator delete[]( void *p )
{
	if (fm)
		fm->Free(p);
}*/
//-------------------------------------------------------------


FCGX_Request httpRequest;

int main (void)
{
	//AVM
	//---------------------------------------
	MMgc::GCHeap::Init();
	MMgc::FixedMalloc::Init();
	fm = MMgc::FixedMalloc::GetInstance();

	MMgc::GCHeap* heap = MMgc::GCHeap::GetGCHeap();

	//---------------------------------------



    //int count = 0;
    //long pid = getpid();

    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    //FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&httpRequest, 0, 0);

    while (FCGX_Accept_r(&httpRequest) == 0)
    {
        // Note that the default bufsize (0) will cause the use of iostream
        // methods that require positioning (such as peek(), seek(),
        // unget() and putback()) to fail (in favour of more efficient IO).
		fcgi_streambuf cin_fcgi_streambuf(httpRequest.in);
		fcgi_streambuf cout_fcgi_streambuf(httpRequest.out);
		fcgi_streambuf cerr_fcgi_streambuf(httpRequest.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        // Although FastCGI supports writing before reading,
        // many http clients (browsers) don't support it (so
        // the connection deadlocks until a timeout expires!).
        /*char * content;
        unsigned long clen = gstdin(&httpRequest, &content);

        cout << "Content-type: text/html; charset=utf-8\r\n"
                "\r\n"
                //"<TITLE>Fcgi Helo</TITLE>\n"
               // "<H1>Tamarin Helo</H1>\n"
                "PID: " << pid << "\n"
				//"<i><span style=\"color:red\">helo here!</span></i>\n<br>"
                "Request Number: " << ++count << "\n";


        cout << "<H4>Request Environment</H4>\n";
        penv(httpRequest.envp);

        cout << "<H4>Process/Initial Environment</H4>\n";
        penv(environ);

       cout << "<H4>Standard Input - " << clen;
        if (clen == STDIN_MAX) cout << " (STDIN_MAX)";
        cout << " bytes</H4>\n";
        if (clen) cout.write(content, clen);

        if (content) delete []content;*/

        //avmshell::HttpClass::request = &request;

        string scriptName = FCGX_GetParam("SCRIPT_FILENAME", httpRequest.envp);

        //current directory is the script (abc) directory
        int res = chdir(scriptName.substr(0, scriptName.find_last_of("/") +1).c_str());

        if (res != 0)
        	cerr << "can't chdir to " << scriptName;

		//MMgc::GCHeap* heap = MMgc::GCHeap::GetGCHeap();

		{
			avmshell::Shell *avm;
			MMgc::GC gc(heap);
			avm = new avmshell::Shell(&gc);
			avm->start(scriptName.c_str());
			delete avm;
		}



        // If the output streambufs had non-zero bufsizes and
        // were constructed outside of the accept loop (i.e.
        // their destructor won't be called here), they would
        // have to be flushed here.
    }


    //AVM
    //--------------------------------
	MMgc::FixedMalloc::Destroy();
	MMgc::GCHeap::Destroy();
	fm = 0;
	//--------------------------------


    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
