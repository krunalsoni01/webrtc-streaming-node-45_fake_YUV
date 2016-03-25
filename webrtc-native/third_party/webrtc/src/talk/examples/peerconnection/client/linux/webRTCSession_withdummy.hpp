#ifndef CLIENT_LINUX_WEBRTCSESSION_HPP
#define CLIENT_LINUX_WEBRTCSESSION_HPP

#include "talk/examples/peerconnection/client/clientpp/src/socket_io_client.hpp"
#include "talk/examples/peerconnection/client/linux/WebRTCFactory.h"
#include "talk/examples/peerconnection/client/linux/WebRTCStream.h"
#include "talk/app/webrtc/mediaconstraintsinterface.h"
#include "talk/examples/peerconnection/client/defaults.h"

#include <deque>
#include <map>
#include <set>
#include <string>

#include "talk/examples/peerconnection/client/peer_connection_client.h"
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "webRTCStack.hpp"

class webRTCStack;

namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket

using namespace std;
using namespace socketio;
using namespace webrtc;

class VideoTrackInterface;
class WebRTCFactory;

enum EventTypes {
    rtc_server_message,
    reg_server_message,
    auth_server_message
  };

enum RTCEventTypes {
    offer,
    answer,
    reoffer,
    reanswer,
    candidate,
    candidates,
    message,
    bye,
    cancel,
    notification,
    ping,
    end,
    start,
    requestnewoffer,
    sessionmessage,
    iceservers
  };


//Observer class has to inherited in class which creates instance of webRTCSession
class SessionObserver{
public:
    virtual void onSessionEnd(){};

    virtual void onSessionConnecting(){};

    virtual void onSessionRenegotiation(){};

    virtual void onSessionConnect(){};

    virtual void onLocalStreamAdded(const long stream, long localVideoTrack, string streamid){};

    virtual void onRemoteStreamAdded(const long stream, string streamId){};

    virtual void onRemoteStreamRemoved(const long stream, string streamId){};

//    virtual ~sessionObserver();
};

/*class SessionCallback{
public:
	virtual void sendRTCMessage(Json::Value message){}
};*/


class mediaconstraints: public MediaConstraintsInterface{
	Constraints mandatory_;
	Constraints optional_;
	const Constraints& GetMandatory() const { //OVERRIDE{
		return mandatory_;
	}

	const Constraints& GetOptional() const{
		return optional_;
	}
};

class webRTCSession: public webrtc::CreateSessionDescriptionObserver,
		public webrtc::PeerConnectionObserver,
		public PeerConnectionClientObserver{
//class webRTCSession: public webrtc::CreateSessionDescriptionObserver{

public:
	webRTCSession(){};
    webRTCSession(string clientsessionId, webRTCStack *stack);
    webRTCSession(string clientsessionId, webRTCStack *stack, string targetid, string fromId, Json::Value options);   // add streamopions (call options) from user
    ~webRTCSession();
    void sendToStack(Json::Value message);   // add fromid, toId and clientsessionId in the outgoing messages
    void on(SessionObserver *lSessionObserver);
    void start();   // connect to stun server and update iceservers from server; start the session
    void requestIceServers();   // send request to server for iceservers
    void startSession(webrtc::PeerConnectionInterface::IceServers ice_servers);    // create peerconnection instance and set local media description
    void getVideoConstraints();             // get user options for video settings and pass it to the Webrtc layer.
//    void setLocalVideoStream(rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc, rtc::scoped_refptr<PeerConnectionFactoryInterface> factory);      // set local audio and video sources/ and send to peerconnection class
    void setLocalVideoStream();//Shayo
    void resetLocalVideoStream();           // arguments yet to be decided
    void createOfferAndSend(rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc, string pcId);  // getting and adding sdp options to the signaling message
    void updateIceServers(sdpData message);

    void setAnswer(sdpData message);  // get the remote midea description from receiver end and send to Peerconnection &
    //implement SDPObserver class

    void end(string reason);        // send bye message to signal handler
    void onSignalingMessage(sdpData message);      // parse message from webRTCStack class and call respective APIs.


    void DummyOnSuccess(webrtc::SessionDescriptionInterface* desc);

    // CreateSessionDescriptionObserver implementation.
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc){}
    virtual void OnFailure(const std::string& error);
    virtual int AddRef(){};
    virtual int Release(){};

    //peerconnectionobserver methods
    //Shayo virtual void OnIceCandidate(const IceCandidateInterface* candidate);
    virtual void OnIceCandidate(const IceCandidateInterface* candidate); //grandpa
    void DummyOnIceCandidate(const IceCandidateInterface* candidate);
    virtual void OnError();
//    virtual void OnSignalingChange(PeerConnectionInterface::SignalingState new_state);
 //Shayo   virtual void OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state);
    void DummyOnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state);
    //virtual
    void DummyOnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state);//Shayo
    virtual void OnAddStream(MediaStreamInterface* stream);
    virtual void OnRemoveStream(MediaStreamInterface* stream);
    virtual void OnDataChannel(DataChannelInterface* channel);
    virtual void OnRenegotiationNeeded();

	  //
	  // PeerConnectionClientObserver implementation.
	  //

	  virtual void OnSignedIn();

	  virtual void OnDisconnected();

	  virtual void OnPeerConnected(int id, const std::string& name);

	  virtual void OnPeerDisconnected(int id);

	  virtual void OnMessageFromPeer(int peer_id, const std::string& message);

	  virtual void OnMessageSent(int err);

	  virtual void OnServerConnectionFailure();


//Shayo private:
//protected://Shayo
   public:
    string target, alias;
    Json::Value options;
    Json::Value dStreamOptions, dStreamTypes;
    string peerconnectionId_;
    string clientSessionId;
//    boost::asio::io_service io_service_;
    SessionObserver *lSessionObserver;
    sdpData sdpMsg;
//    SessionState state;

    const mediaconstraints *pcConstraints;// = new mediaconstraints();
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> pc_factory;//Shayo
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> m_pc_;
//    webrtc::PeerConnectionInterface::IceServers ice_servers;
//    webrtc::PeerConnectionInterface::IceServers ice_servers1;
//    webrtc::PeerConnectionInterface::IceServer server_;

//    PCObserver *pco;// = new PCObserver();
    std::map<std::string, rtc::scoped_refptr<webrtc::MediaStreamInterface> >
          active_streams_;
    webRTCStack *sCb_;

    void setDefaultOptions();
    void onOfferMessage(sdpData message);
    void onAnswerMessage(sdpData message);
    void onCandidateMessage(sdpData message);
    void onPingMessage(sdpData message);//Shayo
    void onIceServers(sdpData message);
    void onByeMessage(sdpData message);

};

#endif // WEBRTCSESSION_HPP
