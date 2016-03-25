#include "SignalHandler.hpp"

using namespace std;
//class webRTCStack;

SignalHandler::SignalHandler(){

}

SignalHandler::SignalHandler(string server_url, webRTCStack *stack){

    hcb_ = new HandlerCallBack();
    GATEWAY_SERVER_URL = server_url;
    this->lStack = stack;
    signaling = new SignalingWrapper();
//    this->connectToSignalingServer(server_url);
}

SignalHandler::~SignalHandler(){

}

void SignalHandler::connectToSignalingServer(string emailId){

    this->connectAndLogin(GATEWAY_SERVER_URL, emailId);
}

void SignalHandler::sendClientRegMessage(string message){

}

void SignalHandler::sendClientRTCMessage(Json::Value message){
	this->sendOffer(message);
}

void SignalHandler::sendClientAuthMessage(string message){

}

void SignalHandler::disconnect(){

}

void SignalHandler::onMessage(sdpData message){
    string type = message.msgName;
	cout << "SignalHandler::onMessage " << type << endl;
	lStack->onSignalingMessage(type, message);

}
