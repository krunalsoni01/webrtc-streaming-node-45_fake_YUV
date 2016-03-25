/*
 * DummyConductor.cpp
 *
 *  Created on: 28-Jul-2014
 *      Author: taher
 */

#include "DummyConductor.h"
#include <utility>
#include <iostream>

#include "talk/app/webrtc/videosourceinterface.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "talk/examples/peerconnection/client/defaults.h"
#include "talk/media/devices/devicemanager.h"

DummyConductor::DummyConductor(webRTCSession *session) {
	// TODO Auto-generated constructor stub
	printf(" DummyConductor::DummyConductor session =%p ,this =%p\n",session,this);
	LOG(INFO) << __FUNCTION__;
	this->session = session;
}

DummyConductor::~DummyConductor() {
	// TODO Auto-generated destructor stub
	LOG(INFO) << __FUNCTION__;
}

//
// PeerConnectionObserver implementation.
//

void DummyConductor::OnError() {
  LOG(LS_ERROR) << __FUNCTION__;
}

void DummyConductor::OnSignalingChange(
     PeerConnectionInterface::SignalingState new_state) {
	LOG(INFO) << "DummyConductor " << __FUNCTION__ << " ";
}

// Called when a remote stream is added
void DummyConductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
	cout << "DummyConductor::OnAddStream" << endl;
  LOG(INFO) << __FUNCTION__ << " " << stream->label();
}

void DummyConductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
  LOG(INFO) << __FUNCTION__ << " " << stream->label();
}

void DummyConductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
  cout << "DummyConductor::OnIceCandidate" << endl;
  //session->DummyOnIceCandidate(candidate); //gk
  LOG(INFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
}
//Shayo : Begin
void DummyConductor::OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state)
{
	cout << "DummyConductor::OnIceConnectionChange" << endl;
//    session->DummyOnIceConnectionChange(new_state);//Shayo //gk
}
void DummyConductor::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state)
{
	cout << "DummyConductor::OnIceGatheringChange,new_state" << new_state << endl;
//    session->DummyOnIceGatheringChange(new_state); //gk 
	cout << "session value is: " << session << endl;
}
//Shayo : End
//
// PeerConnectionClientObserver implementation.
//

void DummyConductor::OnSignedIn() {

	printf(" %s \n",__PRETTY_FUNCTION__);
  LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnDisconnected() {

	printf(" %s \n",__PRETTY_FUNCTION__);
  LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnPeerConnected(int id, const std::string& name) {

	printf(" %s \n",__PRETTY_FUNCTION__);
  LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnPeerDisconnected(int id) {

	printf(" %s \n",__PRETTY_FUNCTION__);
  LOG(INFO) << __FUNCTION__;

}

void DummyConductor::OnMessageFromPeer(int peer_id, const std::string& message) {

	printf(" %s \n",__PRETTY_FUNCTION__);
	LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnMessageSent(int err) {
  // Process the next pending message if any.
	LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnServerConnectionFailure() {
	LOG(INFO) << __FUNCTION__;
}

void DummyConductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
	LOG(INFO) << "DummyConductor" << __FUNCTION__;

	printf(" %s \n",__PRETTY_FUNCTION__);
	if(session!=NULL)
	{
		cout << "session is not null :(" << endl;
		cout << "session value is: " << session << endl;
	//	session->DummyOnSuccess(desc);
	}else
		cout << "session is null :(" << endl;
}

void DummyConductor::OnFailure(const std::string& error) {

	printf(" %s \n",__PRETTY_FUNCTION__);
    LOG(LERROR) << error;
}

void DummyConductor::OnDataChannel(DataChannelInterface* channel) {
        LOG(INFO) << __FUNCTION__ << " *****";
}
