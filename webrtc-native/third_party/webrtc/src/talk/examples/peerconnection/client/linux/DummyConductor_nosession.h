/*
 * DummyConductor.h
 *
 *  Created on: 28-Jul-2014
 *      Author: taher
 */

#ifndef DUMMYCONDUCTOR_H_
#define DUMMYCONDUCTOR_H_

#include <deque>
#include <map>
#include <set>
#include <string>

#include "talk/examples/peerconnection/client/peer_connection_client.h"
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/scoped_ptr.h"
#include "webRTCSession.hpp"

class webRTCSession;

class DummyConductor: public webrtc::CreateSessionDescriptionObserver,
		public webrtc::PeerConnectionObserver,
		public PeerConnectionClientObserver {
public:
	DummyConductor(webRTCSession *session);
	~DummyConductor();

	 //
	  // PeerConnectionObserver implementation.
	  //
	  virtual void OnError();
	  virtual void OnSignalingChange(PeerConnectionInterface::SignalingState new_state);
	  virtual void OnStateChange(
	      webrtc::PeerConnectionObserver::StateType state_changed) {}
	  virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
	  virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
	  virtual void OnRenegotiationNeeded() {}
	  virtual void OnIceChange() {}
	  virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
	  virtual void OnIceConnectionChange(
	        PeerConnectionInterface::IceConnectionState new_state);//Shayo
	  virtual void OnIceGatheringChange(
	        PeerConnectionInterface::IceGatheringState new_state);//Shayo
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

	virtual void OnDataChannel(DataChannelInterface* channel);
	/* {
        LOG(INFO) << __FUNCTION__ << " *****";
}	*/
	  // CreateSessionDescriptionObserver implementation.
	  virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
	  virtual void OnFailure(const std::string& error);
	    virtual int AddRef(){};
	    virtual int Release(){};

private:
	    webRTCSession *session;
};

#endif /* DUMMYCONDUCTOR_H_ */
