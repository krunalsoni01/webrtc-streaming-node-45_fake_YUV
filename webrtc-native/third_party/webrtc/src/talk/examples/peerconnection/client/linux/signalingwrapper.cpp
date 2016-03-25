#include "signalingwrapper.h"
using namespace std;

SignalingWrapper::SignalingWrapper()
{
//	wcb_ = new WrapperCallback();
}

void SignalingWrapper::connectAndLogin(string serverAddr, string emailAddr){

    try {
    	char buff[100];
    	 if((serverAddr == "staging.rtcwith.me") || (serverAddr == "connect.rtcwith.me"))
    	    {
    	    	sprintf(buff, "ws://%s/", serverAddr.c_str());
    	    }else{
    	        sprintf(buff, "ws://%s:3000", serverAddr.c_str());
    	    }
          // Create and link handler to websocket++ callbacks.
    	// handler=new socketio_client_handler();
          client::connection_ptr con;
         // client endpoint(handler);
          websocketpp::client *endpoint = new client(static_cast<socketio::socketio_client_handler_ptr>(this));

          // Set log level. Leave these unset for no logging, or only set a few for selective logging.
          endpoint->elog().set_level(websocketpp::log::elevel::RERROR);
          endpoint->elog().set_level(websocketpp::log::elevel::FATAL);
          endpoint->elog().set_level(websocketpp::log::elevel::WARN);
          endpoint->alog().set_level(websocketpp::log::alevel::DEVEL);
          serverAddr = buff;

          temp = 1;

          std::string socket_io_uri = this->perform_handshake(serverAddr);
          con = endpoint->get_connection(socket_io_uri);
          std::cout << "$$$$$$$$$$$$$$$$ value of temp at first pos:" << temp << std::endl;
          // The previous two lines can be combined:
          // con = endpoint.get_connection(handler->perform_handshake(uri));

          endpoint->connect(con);

//          boost::thread t(boost::bind(&client::run, &endpoint, false));
          boost::thread t(boost::bind(&client::run, endpoint, false));

          // Wait for a sec before sending stuff
          while (!this->connected()) {
             sleep(1);
          }

        //  this->bind_event("reg_server_message", &eventcb);
          this->bind_event("reg_server_message", &socketio::socketio_events::serverMsg);
          this->bind_event("rtc_server_message", &socketio::socketio_events::rtcMsg);

          // After connecting, send a connect message if using an endpoint
          //handler->connect_endpoint("{\"address\":\"2@2.com\"}");

          //std::getchar();

          // Then to connect to another endpoint with the same socket, we call connect again.
          //handler->connect_endpoint("/news");

          //std::getchar();
          //handler->emit("regevent","{ type: 'regsuccess', alias: data.address, contact: data.contact}");
          rapidjson::Document doc1;
          doc1.SetObject();
          rapidjson::Value objVal(kObjectType),objVal2(kObjectType);
          rapidjson::Value objArgs;
          objVal.AddMember("address", emailAddr.c_str(), doc1.GetAllocator());
          objVal.AddMember("uid", emailAddr.c_str(), doc1.GetAllocator());

          objArgs.SetString("args");
          doc1.AddMember(objArgs, objVal, doc1.GetAllocator());
          this->emitt("reg_client_message",doc1);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

}

void SignalingWrapper::sendOffer(Json::Value message){
	rapidjson::Document doc2,doc3;
	rapidjson::Value MemberArr(kArrayType);
	doc2.SetObject();
	doc3.SetObject();
	rapidjson::Value objVal1(kObjectType);
	rapidjson::Value objArgs1;
	Json::StyledWriter writer;
	std::string msg = writer.write(message);

	printf("********* created obj to send offer ***************\n");
	std::cout << "$$$$$$$$$$$$$$$$ value of temp:" << temp << std::endl;
	//MemberArr.PushBack(objVal1,doc2.GetAllocator());
	objArgs1.SetString("args");
	doc2.AddMember(objArgs1, objVal1, doc2.GetAllocator());
	printf("********* emitting rtc_client_message to send offer ***************\n");
	this->emitt("rtc_client_message",msg);
}

void SignalingWrapper::appEventCb(sdpData sdp)
{
   // string type = sdp.msgName;
	cout << "SignalingWrapper::appEventCb" << endl;
	onMessage(sdp);
}
