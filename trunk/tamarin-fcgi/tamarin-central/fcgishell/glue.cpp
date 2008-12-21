/*
 * glue.cpp
 *
 *  Created on: Dec 13, 2008
 *      Author: victor
 */


#include "fcgishell/avmshell.h"



#include <iostream>
#include <stdlib.h>

//fcgi headers
#include <fcgio.h>
#include <fcgi_config.h>

#include <string>

using namespace std;


static const unsigned long STDIN_MAX = 1000000;
int start(char* filename);


static long gstdin(FCGX_Request * request, char ** content);
static void penv(const char * const * envp);


int main (void)
{
    int count = 0;
    long pid = getpid();

    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0)
    {
        // Note that the default bufsize (0) will cause the use of iostream
        // methods that require positioning (such as peek(), seek(),
        // unget() and putback()) to fail (in favour of more efficient IO).
		fcgi_streambuf cin_fcgi_streambuf(request.in);
		fcgi_streambuf cout_fcgi_streambuf(request.out);
		fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

        // Although FastCGI supports writing before reading,
        // many http clients (browsers) don't support it (so
        // the connection deadlocks until a timeout expires!).
        /*char * content;
        unsigned long clen = gstdin(&request, &content);

        cout << "Content-type: text/html; charset=utf-8\r\n"
                "\r\n"
                //"<TITLE>Fcgi Helo</TITLE>\n"
               // "<H1>Tamarin Helo</H1>\n"
                "PID: " << pid << "\n"
				//"<i><span style=\"color:red\">helo here!</span></i>\n<br>"
                "Request Number: " << ++count << "\n";


        cout << "<H4>Request Environment</H4>\n";
        penv(request.envp);

        cout << "<H4>Process/Initial Environment</H4>\n";
        penv(environ);

       cout << "<H4>Standard Input - " << clen;
        if (clen == STDIN_MAX) cout << " (STDIN_MAX)";
        cout << " bytes</H4>\n";
        if (clen) cout.write(content, clen);

        if (content) delete []content;*/

        //find script-name
        const char * const * envp = request.envp;
        string script;
        string s;
        for ( ; *envp; ++envp)
        {
        	s = *envp;
        	if ( s.substr(0, 16) == "SCRIPT_FILENAME=")
        	{
        		script = s.substr(16);
        		break;
        	}
        }

        //current directory is the script (abc) directory
        chdir(script.substr(0, script.find_last_of("/") +1).c_str());


       //cout << "<hr><b>before</b><hr>";
       //cout << "script:" << script << "<br>";
       start((char*) script.c_str());

        // If the output streambufs had non-zero bufsizes and
        // were constructed outside of the accept loop (i.e.
        // their destructor won't be called here), they would
        // have to be flushed here.
    }

    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}


//read stdin
static long gstdin(FCGX_Request * request, char ** content)
{
    char * clenstr = FCGX_GetParam("CONTENT_LENGTH", request->envp);
    unsigned long clen = STDIN_MAX;

    if (clenstr)
    {
        clen = strtol(clenstr, &clenstr, 10);
        if (*clenstr)
        {
            cerr << "can't parse \"CONTENT_LENGTH="
                 << FCGX_GetParam("CONTENT_LENGTH", request->envp)
                 << "\"\n";
            clen = STDIN_MAX;
        }

        // *always* put a cap on the amount of data that will be read
        if (clen > STDIN_MAX) clen = STDIN_MAX;

        *content = new char[clen];

        cin.read(*content, clen);
        clen = cin.gcount();
    }
    else
    {
        // *never* read stdin when CONTENT_LENGTH is missing or unparsable
        *content = 0;
        clen = 0;
    }

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    return clen;
}

static void penv(const char * const * envp)
{
    cout << "<PRE>\n";
    for ( ; *envp; ++envp)
    {
    	cout << *envp << "\n";
    }
    cout << "</PRE>\n";
}
