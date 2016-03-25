#ifndef CLIENT_LINUX_WEBRTCFACTORY_H
#define CLIENT_LINUX_WEBRTCFACTORY_H

#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/refcount.h"
#include "webRTCSession.hpp"
#include "stdio.h"
#include <iostream>

using namespace webrtc;
class webRTCSession;

class WebRTCFactory/*: public webrtc::PeerConnectionObserver*/
{
public:
	WebRTCFactory(webRTCSession *session);
//    talk_base::scoped_refptr<PeerConnectionFactoryInterface> getPeerConnectionFactory();
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> getPeerConnectionInterface(
    		webrtc::PeerConnectionInterface::IceServers ice_servers_);
    void DestroyPeerConnectionFactory();

    //peerconnectionobserver methods
   /* virtual void OnIceCandidate(const IceCandidateInterface* candidate);
    virtual void OnError();
    virtual void OnSignalingChange(PeerConnectionInterface::SignalingState new_state);
    virtual void OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state);
    virtual void OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state);
    virtual void OnAddStream(MediaStreamInterface* stream);
    virtual void OnRemoveStream(MediaStreamInterface* stream);
    virtual void OnDataChannel(DataChannelInterface* channel);
    virtual void OnRenegotiationNeeded();*/

private:
   // webrtc::PeerConnectionFactory* factory;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
    webrtc::PeerConnectionInterface::IceServers ice_servers;
    webRTCSession *tsession;
};

#endif // WEBRTCFACTORY_H
