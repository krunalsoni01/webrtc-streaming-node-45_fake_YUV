#include "webRTCStack.hpp"
#include "webrtc/base/logging.h"

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <iostream>

static std::string clientSessionIdTemp = "";//Shayo

webRTCStack::webRTCStack(){

}

webRTCStack::webRTCStack(string url, string emailAddress, StackObserver *stackObserver)
{

    sHandler = new SignalHandler(url,this);
    sHandler->connectToSignalingServer(emailAddress);
//    sessions = new map<std::string, webRTCSession>();
    stackobserver = stackObserver;
}

webRTCStack::~webRTCStack(){

}

webRTCSession* webRTCStack::createSession(string target, string fromId, Json::Value Options, SessionObserver *sessionobserver)
{
    boost::uuids::uuid t_clientSessionId = boost::uuids::random_generator()();
    clientSessionId = boost::lexical_cast<std::string>(t_clientSessionId);
	//string clientSessionId;
    cout << "webRTCStack::createSession" << endl;
    cout << "clientSessionId in webRTCStack::createSession======>" << clientSessionId << endl;
    clientSessionIdTemp = clientSessionId;//Shayo
    cout << "clientSessionIdTemp======>" << clientSessionIdTemp << endl;
    session = new webRTCSession(clientSessionId, this, target, fromId, Options);
//    sessions[clientSessionId] = session;
//gk 

    sessions.insert(sessions_::value_type(clientSessionId,session));
    for(std::map<std::string, webRTCSession*>::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
    {
    	cout << "copied clientId to sessions map "<< itr->first << endl;
    }
 //grandpa
//    sessions.insert(clientSessionId,session);
    session->on(sessionobserver);
    session->start();
	printf("webRTCStack::createSession  this =%p session=%p\n",this,session);	
    return session;
}

void webRTCStack::sendRTCMessage(Json::Value message){
	cout << "webRTCStack::sendRTCMessage" << endl;
	sHandler->sendClientRTCMessage(message);
}

void webRTCStack::onSignalingMessage(string event, sdpData message)
{
	cout << "webRTCStack::onSignalingMessage   event: " << event.c_str() << endl;
	 printf(" webRTCStack::onSignalingMessage  this =%p \n",this);
    if(event == "reg_server_message")
    {
    	this->onRegServerMessage(message);
    }
    else if(event == "rtc_server_message")
    {
    	this->onRTCServerMessage(message);
    }
    else if(event == "auth_server_message")
    {

    }
    else
    {
        printf("ERROR: Unknown server message\n");
    }

}

void webRTCStack::onRegServerMessage(sdpData message){
	cout << "webRTCStack::onRegServerMessage *** " << message.msgName << endl;

	if(message.MsgType == kLoginType)
	{
		printf("Registration successful\n");
		stackobserver->onReady(message);
	}
	else if(message.MsgType == kLoginFailure)
		printf("Registration failed");
}

void webRTCStack::onRTCServerMessage(sdpData message){
    cout << "webRTCStack::onRTCServerMessage" << endl;
//	if(sessions.count(message.PingData.clientSessionId) >= 1)
    cout << " ************* local clientsessionIdTemp :: " << clientSessionIdTemp << endl;
    clientSessionId = clientSessionIdTemp;//Shayo
    cout << " ************* local clientsessionId :: " << clientSessionId << endl;
    cout << "message.PingData.clientSessionId==>" << message.PingData.clientSessionId << endl;
    message.PingData.clientSessionId = clientSessionId;//Shayo
	 printf(" webRTCStack::onRTCServerMessage  this =%p session =%p\n",this,session);

	if(clientSessionId.compare(message.PingData.clientSessionId)==0)
	{

//		webRTCSession session_ = *sessions[message.PingData.clientSessionId];
		//webRTCSession *session_ = session;
		cout << "webRTCStack::onRTCServerMessage--cp1" << endl;
		session->onSignalingMessage(message);
	}
	else if(message.MsgType == kOfferType)
	{
		webRTCSession session_ = webRTCSession(NULL,this);
		cout << "webRTCStack::onRTCServerMessage--cp2" << endl;
		sessions["Id"]= &session_;
		stackobserver->onSession(&session_);
		session_.onSignalingMessage(message);
	}
	else
		LOG(INFO) << "Unknown clientSessionId - dropping message";

}

void webRTCStack::dispose(){

}
