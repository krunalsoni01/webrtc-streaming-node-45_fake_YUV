#include "WebRTCFactory.h"
#include "webrtc/base/logging.h"

using namespace std;

WebRTCFactory::WebRTCFactory(webRTCSession *session)
{
	tsession = session;
}

//talk_base::scoped_refptr<PeerConnectionFactoryInterface> WebRTCFactory::getPeerConnectionFactory()
rtc::scoped_refptr<webrtc::PeerConnectionInterface> WebRTCFactory::getPeerConnectionInterface(
		webrtc::PeerConnectionInterface::IceServers ice_servers_)
{
    if (factory.get()) {
    	std::cout << "Factory class:: already available"<<std::endl;
       // return factory;
    } else {
    	std::cout << "Factory class:: creating factory"<<std::endl;
    	factory = webrtc::CreatePeerConnectionFactory();
    	std::cout << "After factory creation" << std::endl;
    	if(!factory.get()){
    		std::cout << "Factory class:: couldn't be created"<<std::endl;
    		return NULL;
    	}
       // return factory;
    }
		std::cout<<"Error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111 \n";
		std::cout<<"Error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111 \n";
		std::cout<<"Error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111 \n";
		std::cout<<"Error !!!!!!!!!!!!!!!!!!!!!should not enter here !!!!!!!!!!!!!!!!!!11111 \n";
    peerConnection = factory->CreatePeerConnection(ice_servers_,NULL, NULL, NULL,NULL); //not used 
    return peerConnection;
}

void WebRTCFactory::DestroyPeerConnectionFactory()
{
    if(factory != NULL)
    {
        factory.release();
    }
}

//Implementation of PeerconnectionObserver class
/*void WebRTCFactory::OnIceCandidate(const IceCandidateInterface* candidate) {

	LOG(INFO) << __FUNCTION__;
	//	  Json::StyledWriter writer;
	 Json::Value jmessage;

	 jmessage["sdpMid"] = candidate->sdp_mid();
	 jmessage["sdpMLineIndex"] = candidate->sdp_mline_index();
	 std::string sdp;
	 if (!candidate->ToString(&sdp)) {
	 LOG(LS_ERROR) << "Failed to serialize candidate";
	 return;
	 }
	 jmessage["candidate"] = sdp;
	 tsession->sendToStack(jmessage);
}

void WebRTCFactory::OnError() {
	LOG(INFO) << __FUNCTION__ << " *****";
	cout << "webRTCSession::OnError" << endl;

}*/

