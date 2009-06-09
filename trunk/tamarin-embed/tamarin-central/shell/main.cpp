#include "avmshell.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>


//fcgi headers
#include <fcgio.h>
#include <fcgi_config.h>


using namespace std;

FCGX_Request httpRequest;

fcgi_streambuf cin_fcgi_streambuf;
fcgi_streambuf cout_fcgi_streambuf;
fcgi_streambuf cerr_fcgi_streambuf;

int rqCount;
int maxRq = 20;
bool acceptOk = true;

int main (int argc, char* argv[])
{
	char* conf = getenv("MODAS_MaxRequests");
	if (conf)
		maxRq = atoi(conf);


    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Init();
    FCGX_InitRequest(&httpRequest, 0, 0);

    cerr << "Inited MODAS_MaxRequests=" << maxRq << "\n\n";


    //AVM endless loop
    while (acceptOk)
    {
    	rqCount = 0;
    	startShell(argc, argv);
    	fprintf(stderr, "AVM graceful restart\n");
    }

    cout.flush();
    cerr.flush();
    FCGX_Finish();

    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}


const char* getRequest()
{
	if (++rqCount > maxRq) return NULL;

	if (FCGX_Accept_r(&httpRequest) != 0)
	{
		acceptOk = false;
		fprintf(stderr, "acceptOk=false\n");
		return NULL;
	}

	cin_fcgi_streambuf.attach(httpRequest.in);
	cout_fcgi_streambuf.attach(httpRequest.out);
	cerr_fcgi_streambuf.attach(httpRequest.err);

    cin.rdbuf(&cin_fcgi_streambuf);
    cout.rdbuf(&cout_fcgi_streambuf);
    cerr.rdbuf(&cerr_fcgi_streambuf);


    string scriptName = FCGX_GetParam("SCRIPT_FILENAME", httpRequest.envp);

    //current directory is the script (abc) directory
    int res = chdir(scriptName.substr(0, scriptName.find_last_of("/") +1).c_str());
    if (res != 0)
    {
    	cerr << "can't chdir to " << scriptName;
    	return NULL;
    }


    //FIXME
    static char buff[300];
    strcpy(buff, scriptName.c_str());

    return buff;
}

