

#include "talk/examples/peerconnection/client/flagdefs.h"
#include "talk/examples/peerconnection/client/peer_connection_client.h"
#include "talk/examples/peerconnection/client/linux/sample.h"
#include "talk/examples/peerconnection/client/linux/webRTCStack.hpp"
#include "webrtc/base/ssladapter.h"
#include "webrtc/base/thread.h"


//#include "talk/base/ssladapter.h"
//#include "talk/base/thread.h"
#include <iostream>

//Sample sample;
//Sample *sample;
using namespace std;

int main(int argc, char* argv[]) {

	char* serverip = new char[30];
	char* FromId = new char[30];
	char* ToId = new char[30];
	char* useroption = new char[30];
	Sample *sample;
	webRTCStack *stack;
	SessionObserver *sessionObserver;
	cout << "No of args" << argc << endl;
	cout << "argv[1]" << argv[1] << endl;

	if (argc >= 2)
	{
		/* server ip is provided copy the same */
		strcpy(serverip, argv[1]);
		cout << "server will be started with ip: " << serverip;
	}

	if (argc >= 3)
	{
		cout << "argv[2]" << argv[2] << endl;
		strcpy(FromId, argv[2]);
	}else
	{
	//	strcpy(FromId, "taher@comcast.net");
	strcpy(FromId, "stb1@comcast.net"); // Modify the User name as per the server
	}

	cout << "Using email address for login: " << FromId;

	if (argc >= 4)
	{
		strcpy((char*)useroption, argv[3]);
	}
	else
	{
	//	strcpy(ToId, "muthu@comcast.net");
		strcpy(ToId, "stb2@comcast.net"); // Modify the User name as per the server
		strcpy(useroption, "two_way");
	}

	if (argc >= 5)
	{
		strcpy((char*)ToId, argv[4]);
	}

	//talk_base::InitializeSSL();
	rtc::InitializeSSL();

	sample = new Sample((string)serverip, (string)FromId, (string)ToId, (string)useroption);
	stack = new webRTCStack((string)serverip, (string)FromId, sample);
	sessionObserver = sample;
	//sample->createStack(*stack);
	sample->createStack(stack);

	getchar();

	//talk_base::CleanupSSL();
	rtc::CleanupSSL();
	return 0;
}

