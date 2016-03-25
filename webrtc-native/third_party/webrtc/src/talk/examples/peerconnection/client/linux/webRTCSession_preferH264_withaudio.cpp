
#include "webRTCSession.hpp"
#include "talk/examples/peerconnection/client/linux/sample.h"
//#include "talk/examples/peerconnection/client/linux/peerconnection_fake.h"
#include "talk/examples/peerconnection/client/linux/androidmediaencoder_fake.h"
//#include "DummyConductor.h"
#include <boost/algorithm/string.hpp>
#include <utility>
#include <iostream>
//gk
#include "webrtc/system_wrappers/interface/trace.h"
//gk
#include "talk/app/webrtc/videosourceinterface.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "talk/examples/peerconnection/client/defaults.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include <unistd.h>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <string>
#include <stdio.h>
static Json::Value optionsTemp;//Shayo
static string targetTemp,aliasTemp,clientSessionIdTemp;//Shayo
//static webRTCStack *sCb_Temp;//Shayo
//const std::string peerConnectionId;//Shayo
std::string peerConnectionId;//Shayo
class Sample;
class DummyConductor;

class mediaconstraints;

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "id";
const char kCandidateSdpMlineIndexName[] = "label";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";
//infy quanta 
//rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track_temp;
//infy quanta
class DummySetSessionDescriptionObserver: public webrtc::SetSessionDescriptionObserver {
public:
	static DummySetSessionDescriptionObserver* Create() {
		return new rtc::RefCountedObject<
				DummySetSessionDescriptionObserver>();
	}
	virtual void OnSuccess() {
		printf(" OnSuccess() \n");
		LOG(INFO) << "DummySetSessionDescriptionObserver  " << __FUNCTION__;
	}
	virtual void OnFailure(const std::string& error) {
		LOG(INFO) << __FUNCTION__ << " " << error;
	}

protected:
	DummySetSessionDescriptionObserver() {
	}
	~DummySetSessionDescriptionObserver() {
	}
};

enum SessionState {
	active, connecting, inactive, disconnected
} state;

webRTCSession::webRTCSession(string clientsessionId, webRTCStack *stack) {
	pcConstraints = new mediaconstraints();
	peerconnectionId_ = "";
//Shayo	pc_ = NULL;
//	pco = new PCObserver();
	printf("webRTCSession::webRTCSession()  ctor\n");
	this->clientSessionId = clientsessionId;
	sCb_ = stack;
	state = inactive;
}

webRTCSession::webRTCSession(string clientsessionId, webRTCStack *stack,
		string targetid, string fromId, Json::Value options) {
	pcConstraints = new mediaconstraints();
	peerconnectionId_ = "";
//Shayo	pc_ = NULL;
//	pco = new PCObserver();
	this->clientSessionId = clientsessionId;
	sCb_ = stack;
	this->alias = fromId;
	target = targetid;
	cout << "clientsessionId==========================>" << this->clientSessionId << endl;
	cout << "target==========================>" << target << endl;
	cout << "alias===========================>" << this->alias << endl;
	this->options = options;
	sCb_Temp = sCb_;//Shayo
	targetTemp = target;//Shayo
	aliasTemp = this->alias;//Shayo
	clientSessionIdTemp = this->clientSessionId;
	state = inactive;
	LOG(INFO) << "webRTCSession::webRTCSession" << endl;
       cout<< " webRTCSession::webRTCSession this"<< this;
	setDefaultOptions();
}

webRTCSession::~webRTCSession() {

}

void webRTCSession::setDefaultOptions() {
	cout << "webRTCSession::setDefaultOptions---" << endl;
//    if (options.isMember("streamOptions")) {
	Json::Value localStreamOptions;
	Json::Value lSO = new Json::Value();
	localStreamOptions = options.get("streamOptions", new Json::Value());
//    }else {
//      streamOptions = new Json::Value();
//    }
	if (options["streamOptions"].isObject()) {
		std::cout << "StreamOptions::   " << options.toStyledString()
				<< std::endl;
//		localStreamOptions = options;
		dStreamOptions = options["streamOptions"];

//      dStreamOptions = localStreamOptions.get("one_way", true);
//      dStreamOptions = localStreamOptions.get("video_resolution", "hd");
//      dStreamOptions = localStreamOptions.get("broadcast", true);
	} else {
		std::cout << "No streamOptions chosen; defaulting" << std::endl;
		dStreamOptions["one_way"] = true;
		dStreamOptions["video_resolution"] = "hd";
		dStreamOptions["broadcast"] = true;
	}

	options["streamOptions"] = dStreamOptions;
	std::cout << "StreamOptions =>\n" << dStreamOptions.toStyledString()
			<< std::endl;

	// Set the defaults for streamTypes
//    if (options.isMember("streamTypes")) {
	Json::Value localStreamTypes;
	localStreamTypes = options.get("streamTypes", new Json::Value());

//    dStreamTypes = localStreamTypes.get("audio", "sendonly");
	dStreamTypes["audio"] = "sendonly";
//    dStreamTypes = localStreamTypes.get("video", "sendonly");
	dStreamTypes["video"] = "sendonly";
//    dStreamTypes = localStreamTypes.get("data", "sendonly");
	dStreamTypes["data"] = "sendonly";

	options["streamTypes"] = dStreamTypes;
	optionsTemp = options;//Shayo
	std::cout << "optionsTemp ===>\n" << optionsTemp.toStyledString()
				<< std::endl;
}

void webRTCSession::updateIceServers(sdpData message) {
	cout << "webRTCSession::updateIceServers" << endl;
	printf(" webRTCSession::updateIceServers this =%p \n ",this);
	if (state != inactive) {
		LOG(LS_ERROR)
				<< "Timer expired, but startSession already has been started";
		return;
	}

	webrtc::PeerConnectionInterface::IceServers ice_servers;
	webrtc::PeerConnectionInterface::IceServer server_;
	for (int i = 0; i < message.iceServersData.uri.size(); i++) {
		server_.password = message.iceServersData.credential;
		server_.uri = message.iceServersData.uri.toStyledString();
		server_.username = message.iceServersData.username;
		ice_servers.push_back(server_);
	}
	if (ice_servers.empty()) {
		server_.uri = GetPeerConnectionString();
		ice_servers.push_back(server_);
	}
	if(ice_servers.size()!=0){
		cout << "call startSession" << endl;
		this->startSession(ice_servers);
	}

}

void webRTCSession::on(SessionObserver *lSessionObserver_var) {
	cout << " webRTCSession::on =========================="<< endl;
	this->lSessionObserver = lSessionObserver_var;
}

void webRTCSession::start() {
	cout << "webRTCSession::start" << endl;
	cout << "###### Trying to see target value===>" << target << endl;
	requestIceServers();
	//boost::asio::deadline_timer timer_(io_service_, boost::posix_time::seconds(1));
	//timer_.async_wait(webRTCSession::updateIceServers);
	//timer_.wait();
//	this->updateIceServers();
}

void webRTCSession::startSession(webrtc::PeerConnectionInterface::IceServers ice_servers) {
	cout << "webRTCSession::startSession" << endl;
	rtc::scoped_ptr<cricket::WebRtcVideoEncoderFactory> encoder_factory;
	state = connecting;
	boost::uuids::uuid tPeerConnectionId = boost::uuids::random_generator()();
	//const std::string
	peerConnectionId = boost::lexical_cast<std::string>(
			tPeerConnectionId);
	cout << "create factory value!!!i11111"<< endl;
	 printf(" webRTCSession::startSession pc_factory this= %p \n",this);
	pc_factory.release();
cout << "create factory 2 !!!" << endl;

	 //rtc::scoped_refptr<PeerConnectionFactoryInterface> factory_temp = webrtc::CreatePeerConnectionFactory();//Shayo
#if 0
	  pc_factory = webrtc::CreatePeerConnectionFactory();//Shayo
#else
	 rtc::ThreadManager::Instance()->WrapCurrentThread();
	  webrtc::Trace::CreateTrace();
	  rtc::Thread* worker_thread = new rtc::Thread();
	  worker_thread->SetName("worker_thread", NULL);
	  rtc::Thread* signaling_thread = new rtc::Thread();
	  signaling_thread->SetName("signaling_thread", NULL);
	  CHECK(worker_thread->Start() && signaling_thread->Start())
	      << "Failed to start threads";
	      encoder_factory.reset(new webrtc_jni::MediaCodecVideoEncoderFactory());
	 rtc::scoped_refptr<PeerConnectionFactoryInterface> factory_temp(
      webrtc::CreatePeerConnectionFactory(worker_thread,
                                          signaling_thread,
                                          NULL,
                                        //encoder_factory.release(),
					  NULL,
                                          NULL));
	

	 pc_factory = factory_temp;

#endif 

cout << "create factory 3!!!11111" << endl;

#if 0
	WebRTCFactory *wfactory_ = new WebRTCFactory(this);
//	factory = wfactory_->getPeerConnectionFactory();
	pc_ = factory->CreatePeerConnection(ice_servers, NULL, NULL, this);
	pc_ = wfactory_->getPeerConnectionInterface(ice_servers);
#else
	//talk_base::scoped_refptr<PeerConnectionFactoryInterface> factory = webrtc::CreatePeerConnectionFactory();
//	factory = webrtc::CreatePeerConnectionFactory();//Shayo
//		pc_factory =factory_temp;
	cout << "create factory 3.1                      !!!11111" << endl;	
	if(!pc_factory.get()){
		std::cout << "Factory couldn't be created"<<std::endl;
		pc_factory = NULL;
		m_pc_ = NULL;
		return;
	}
	cout<<"...create factory 6,,"<<endl;
//	DummyConductor *dummyconductor = new DummyConductor(this);
//	dummyconductor = new DummyConductor(this);
//	dummyconductor.init(this);
        //    printf(" dummyconductor   =%p \n",dummyconductor);	
//dummyconductor = new DummyConductor(this);
	//pc_ = factory->
	//m_pc_.release();
//	m_pc_=NULL;
 webrtc::PeerConnectionInterface::IceServers servers1;
  webrtc::PeerConnectionInterface::IceServer server1;
  server1.uri = GetPeerConnectionString();
  servers1.push_back(server1);

cout<<"create factory 4"<<endl;
printf("DTLS is true");
//rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc_;
    webrtc::FakeConstraints fake_constraints;
    fake_constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                            "false");
                            //"true");
 /* }
  else
  {
    constraints.AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
                            "false");
  }
*/
//	 &constraints,

	//pc_ = pc_factory->CreatePeerConnection(ice_servers,NULL, NULL, NULL, dummyconductor);
	m_pc_ = pc_factory->CreatePeerConnection(servers1,&fake_constraints, NULL, NULL, this);
	//pc_ = pc_factory->CreatePeerConnection(ice_servers,&constraints, NULL, NULL, dummyconductor);
//	pc_ = pc_factory->CreatePeerConnection(ice_servers,&constraints, NULL, NULL, this);
	if(!m_pc_.get())
	{
                std::cout << "Factory couldn't be created"<<std::endl;
	}

	cout<<"create factory 5 "<<endl;
	printf(" pc_ %p  this =%p \n",(void *)m_pc_,this);
	/*talk_base::scoped_refptr<webrtc::PeerConnectionInterface> pctemp = //factory->CreatePeerConnection(ice_servers, NULL, NULL, dummyconductor);
	cout << "pc Interface returned" << endl;
	if(pc_!=NULL && pctemp!=NULL)
	{
		cout << "pc Interface returned--cp1" << endl;
	    pc_.release();

	    pc_ = pctemp;
	}*/

#endif

	cout << "In Session, After creating PCFactory & PC"<<endl;
	//   Conductor::ConnectToPeer(pc);
//	setLocalVideoStream(pc_, factory);

	setLocalVideoStream();
	/*
	 * creating a dummy class to check createOffer
	 *///SHAYO

        cout<<"..creating offer webrtcsession.."<<endl;
//        cout<<"..dummyconductor value.."<<&dummyconductor<<endl;
       // sleep(100);
	printf(" pc _ value =%p \n",(void  *)m_pc_);
//	printf("dummyconductor[1] =%p \n",dummyconductor);
		printf("this =%p \n",this);
//	m_pc_->CreateOffer(dummyconductor, NULL);
	m_pc_->CreateOffer(this, NULL);
        cout<<"..created offer.."<<endl;
//	createOfferAndSend(pc_, peerConnectionId);
	std::cout << "calling sessionObserver onSessionConnecting 121212" << std::endl;
//	lSessionObserver->onSessionConnecting();
}


void webRTCSession::createOfferAndSend(
		rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc,
		string pcId) {

//	talk_base::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection =
//			pc;
	//peerconnectionId_.assign(pcId);
//	peerconnectionId_ = pcId;			// SHAYO
	std::cout << "webRTCSession::createOfferAndSend" << std::endl;
	m_pc_->CreateOffer(this, NULL);
}

void webRTCSession::getVideoConstraints() {

}

void webRTCSession::setAnswer(sdpData message) {
	std::string sdp = message.Sdp;

	cout<<"<======= SDP inside webRTCSession::setAnswer = "<< sdp;
	webrtc::SessionDescriptionInterface* session_description(
			webrtc::CreateSessionDescription(kanswerStr, sdp));
	cout << "<======= session_description: " << session_description << "=======>" << endl;
	if (!session_description) {
		LOG(WARNING) << "Can't parse received session description message.";
		return;
	}
//	LOG(INFO) << " Received session description :" << message;
	m_pc_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(),
			session_description);
	//this->lSessionObserver->onSessionConnect();

}

void webRTCSession::sendToStack(Json::Value message) {

	message["clientSessionId"] = clientSessionIdTemp;
	cout << "clientSessionId ==================>>>>>>> " << clientSessionIdTemp << endl;
	/*message["to"] = target;
	message["from"] = alias;*/
	message["to"] = targetTemp;//Shayo
	message["from"] = aliasTemp;//Shayo
	message["uid"] = aliasTemp;
	message["target"] = targetTemp;//Shayo
	cout << "webRTCSession::sendToStack" << endl;
	if(sCb_Temp!=NULL)
	{
	cout << "$$$$$$$$ sCb_Temp is NOT NULL===>" << sCb_Temp << endl;
	sCb_Temp->sendRTCMessage(message);
	}
	else
		cout << "$$$$$$$$ sCb_ is NULL" << endl;

}

/*void webRTCSession::setLocalVideoStream(
		talk_base::scoped_refptr<webrtc::PeerConnectionInterface> pc,
		talk_base::scoped_refptr<PeerConnectionFactoryInterface> factory) {*/
void webRTCSession::setLocalVideoStream(){
	cout << "webRTCSession::setLocalVideoStream" << endl;

#if 0 //Shayo
	if (active_streams_.find(kStreamLabel) != active_streams_.end())
		return;  // Already added.
#endif
	cout << "webRTCSession::setLocalVideoStream creating stream" << endl;
	WebRTCStream *stream_ = new WebRTCStream();
	cout << "webRTCSession::setLocalVideoStream--cp1" << endl;
	//  factory = webrtc::CreatePeerConnectionFactory();
	printf(" webRTCSession::setLocalVideoStream this =%p \n",this);
	rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
			pc_factory->CreateAudioTrack(kAudioLabel,
					pc_factory->CreateAudioSource(NULL)));
        cout << "webRTCSession::setLocalVideoStream,audio_track==>" << audio_track << endl;
	cout << "webRTCSession::setLocalVideoStream--cp2" << endl;
//	cricket::VideoCapturer *capturer_ = stream_->getCapturerDevice();
//	VideoSourceInterface *source = factory->CreateVideoSource(capturer_, NULL);
//
//	talk_base::scoped_refptr<webrtc::VideoTrackInterface> video_track(factory->CreateVideoTrack(kVideoLabel, source));
	rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
			pc_factory->CreateVideoTrack(kVideoLabel,
					pc_factory->CreateVideoSource(stream_->getCapturerDevice(),
							NULL)));
	cout << "webRTCSession::setLocalVideoStream,video_track==>" << video_track << endl;
	rtc::scoped_refptr<webrtc::MediaStreamInterface> stream =
			pc_factory->CreateLocalMediaStream(kStreamLabel);
	cout << "webRTCSession::setLocalVideoStream,CreateLocalMediaStream successful" << endl;
    cout << "stream==>" << stream << endl;
	stream->AddTrack(audio_track);
	cout<<"..adding track.."<<endl;
	stream->AddTrack(video_track);
//infy quanta starts
	//printf("webRTCSession:: call NumberofCaptureDEvices() \n");
//	cricket::VideoCapturer* video_capturer_quanta_ptr=video_track->GetSource()->GetVideoCapturer();
//	VideoCapturer* video_capturer_quanta_ptr=video_track->GetSource()->GetVideoCapturer();
  //	int test =video_capturer_quanta_ptr->NumberOfCaptureDevices();
	//cricket::VideoCapturer* video_capturer_quanta_ptr = stream_->getcapturerDevice_static();
	//printf(" test =%d \n",test);
//infy quanta  starts
	cout<<"..adding video track.."<<endl;
	//if (!pc_->AddStream(stream, NULL)) {
        if (!m_pc_->AddStream(stream)) {
		LOG(LS_INFO) << "Adding stream to PeerConnection failed";
	}
	typedef std::pair<std::string,
			rtc::scoped_refptr<webrtc::MediaStreamInterface> > MediaStreamPair;
	cout << "stream->label()==>" << stream->label() << endl;
	//Shayo active_streams_.insert(MediaStreamPair(stream->label(), stream));

}

void webRTCSession::requestIceServers() {
	Json::Value iceServers;
	iceServers["type"] = "requestIceServers";
	cout << "webRTCSession::requestIceServers,target" << target << endl;
	sendToStack(iceServers);
}

void webRTCSession::onSignalingMessage(sdpData message) {

	LOG(INFO) << __FUNCTION__ << "__brindha";
	printf(" webRTCSession::onSignalingMessage this =%p \n",this);
	//std::string type;
	//type = message.MsgType;
	switch (message.MsgType) {
	case kOfferType:
		onOfferMessage(message);
		break;
	case kAnswerType:
		onAnswerMessage(message);
		break;
	case kCandidateType:
		onCandidateMessage(message);
		break;
	case kPingMessage:
	        onPingMessage(message);
	        break;
	case kIceServer:
		onIceServers(message);
		break;
	case kByeType:
		onByeMessage(message);
		break;
	}
}

void webRTCSession::onAnswerMessage(sdpData message) {
	this->setAnswer(message);

}

void webRTCSession::onPingMessage(sdpData message)
{
	cout << "Inside webRTCSession::onPingMessage" << endl;
	Json::Value jmessage;
	try {
	jmessage[ktypeStr] = "pong";
	jmessage["peerConnectionId"] = peerConnectionId;
	}
	catch (...)
	{
	  return;
    }
	sendToStack(jmessage);
}

void webRTCSession::onCandidateMessage(sdpData msg) {
	LOG(INFO) << __FUNCTION__;
	 printf(" webRTCSession::onCandidateMessage( \n");
#if 1
	int peerConnectionId;
	string reason = "";
	string candidateTemp;
	string id;
	int label;
	bool last;

	try {
		 if(msg.lastTemp == true) {
	    	cout << "Last candidate message received" << endl;
	        last = true;
	        msg.sdpMid = "";
	        msg.Sdp = "";
	        //label = -1;
	        msg.sdpMlineIndex = -1;
	        reason = "candidatesExhausted";
	        cout << "Reason for candidate msg: " << reason << endl;
	        peerConnectionId = msg.id;
	      } else {
	    	  cout << "Last candidate message NOT received" << endl;
	        last = false;
	        candidateTemp = msg.Sdp;
	        id = msg.sdpMid;
	        label = msg.sdpMlineIndex;
	        cout << "candidateTemp: " << candidateTemp << endl;
	        cout << "id " << id << endl;
	        cout << "label" << label << endl;
	        peerConnectionId = msg.id;
	     /*   candidate = msg.getString("candidate");
	                id = msg.getString("id");
	                label = msg.getInt(("label"));
	                peerConnectionId = msg.getString("peerConnectionId");*/
	      }
	    } catch (...) {
	      cout << "Signaling doesn't have a peerConnectionId... dropping" << endl;
	      return;
	    }
	    if(last)
	    {
	    	cout << "Last candidate signalled!!!" << endl;
	    	return;
	    }
#endif
	//talk_base::scoped_ptr<webrtc::IceCandidateInterface> candidate(
			//webrtc::CreateIceCandidate(msg.sdpMid, msg.sdpMlineIndex, msg.Sdp));
	   rtc::scoped_ptr<webrtc::IceCandidateInterface> candidate(
	    			webrtc::CreateIceCandidate(id, label, candidateTemp));
	cout << "Candidate created succesfully!!!!" << endl;
	if (!candidate.get()) {
		LOG(WARNING) << "Can't parse received candidate message.";
		return;
	}
	if (!m_pc_->AddIceCandidate(candidate.get())) {
		LOG(WARNING) << "Failed to apply the received candidate";
		return;
	}
}

void webRTCSession::onOfferMessage(sdpData message) {
	if (state != inactive) {
		LOG(INFO) << "Got an offer while not in an inactive state";
	}

}

void webRTCSession::onIceServers(sdpData message) {
	//Json::Value iceServers = message.iceServersData;
//	PeerConnectionInterface::IceServer iceServers_;
	cout << "webRTCSession::onIceServers" << endl;
	printf(" webRTCSession::onIceServers =%p \n",this);
//	sdpMsg = message;
	/*for (int i = 0; i < message.iceServersData.uri.size(); i++) {
		server_.password = message.iceServersData.credential;
		server_.uri = message.iceServersData.uri.toStyledString();
		server_.username = message.iceServersData.username;
		ice_servers.push_back(server_);
	}*/

	this->updateIceServers(message);
	// Call this function as an async task
	//this->startSession();
	//io_service_.run();

}

void webRTCSession::onByeMessage(sdpData message) {
	cout << "<******************** Inside webRTCSession::onByeMessage ********************>" << endl;
    if(state != active)
    {
        state = disconnected;
        std::string tokenError = "";
        std::string reason = "";
        std::vector<Json::Value> arrayVehicles;
        /*std::string sdp = message.Msg;
        try{
        	if(!(message.Msg == ""))
        	{
        		reason = message.Msg;
        		Json::Value jmessage;
        		jmessage[] = ""
        	}
        }*/
        cout << "message.Msg :" << message.Msg << endl;
    }
}

void webRTCSession::resetLocalVideoStream() {

}

//void webRTCSession::DummyOnSuccess(webrtc::SessionDescriptionInterface* desc) {
void webRTCSession::OnSuccess(webrtc::SessionDescriptionInterface* desc) {

	cout << "webRTCSession::OnSuccess" << endl;
//	state = SessionState::active;
	state = active;
	m_pc_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(),
			desc);
	cout << "webRTCSession::OnSuccess 1" << endl;
	boost::uuids::uuid tPeerConnectionId = boost::uuids::random_generator()();
	peerConnectionId = boost::lexical_cast<std::string>(
			tPeerConnectionId);
	cout << "webRTCSession::OnSuccess 2" << endl;
//  Json::StyledWriter writer;
	Json::Value jmessage;
	jmessage[ktypeStr] = desc->type();
	std::string sdp;
	cout << "webRTCSession::OnSuccess 3" << endl;
	desc->ToString(&sdp);
	cout << "webRTCSession::OnSuccess 4" << endl;
	//jmessage[ktargetStr] = "muthu@comcast.net";
#if 0
	if (options.isMember("broadcast")) {
		boost::replace_all(sdp, "sendrecv", "sendonly");
		cout << "webRTCSession::OnSuccess 5" << endl;
	}
#else
	Json::Value temp;
	string tmpstring;
	cout << "TEMP = " << optionsTemp.toStyledString() << endl;
	//Shayo:GetStringFromJsonObject(options,"streamOptions",&tmpstring);
	cout << "tmpstring : "<< optionsTemp["streamOptions"].toStyledString()<<endl;//  GetString()<<endl;
//	Json::Value temp1 =optionsTemp["streamOptions"];
	bool temp1 =optionsTemp["streamOptions"]["broadcast"].asBool();
	cout << "tmp1:" << temp1 << endl;

	//GetStringFromJsonObject(optionsTemp["streamOptions"],"streamOptions",&tmpstring);
	//cout << "tmpstring:" << tmpstring.c_str() << endl;
	/*if(!tmpstring.empty()){
	//bool _broadcast = temp["broadcast"].asBool();
	cout << "streamoptions is " << tmpstring << endl;
	}
	else
//		cout << "streamoptions is empty" << endl;*/
//	if (temp["broadcast"].asBool()) {
	if(temp1){
		cout << "+++++++replacing sendonly with sendrecv 1" << endl;
		boost::replace_all(sdp, "sendrecv", "sendonly");
	}
	
	std::string sdp1 =preferCodec(sdp, "H264");
//preferCodec(sdp, "H264");
#endif
		//cout << "After sdp1 "<< sdp1 << endl;
//  jmessage[ksdpStr] = sdp;

jmessage[ksdpStr] = sdp1;
	jmessage["peerConnectionId"] = peerConnectionId;

	jmessage["streamOptions"] = optionsTemp["streamOptions"];
	jmessage["streamTypes"] = optionsTemp["streamTypes"];
	//     offerMsg.put("streamTypes", options.getJSONObject("streamTypes"));
	cout << "webRTCSession::OnSuccess 7" << endl;
	this->sendToStack(jmessage);
	cout << "webRTCSession::OnSuccess 6" << endl;
}

void webRTCSession::OnFailure(const std::string& error) {
	LOG(LERROR) << error;
}

void webRTCSession::OnIceCandidate(const IceCandidateInterface* candidate) 
{
	printf(" webRTCSession::OnIceCandidate call DummyOnIceCandidate.... grandpa  \n");
	DummyOnIceCandidate( candidate);
}

//Implementation of PeerconnectionObserver class
void webRTCSession::DummyOnIceCandidate(const IceCandidateInterface* candidate) {
//void webRTCSession::OnIceCandidate(const IceCandidateInterface* candidate) {
	 cout << "webRTCSession::DummyOnIceCandidate" << endl;

	LOG(INFO) << __FUNCTION__;
	//	  Json::StyledWriter writer;
	 Json::Value jmessage;
	 jmessage["peerConnectionId"] = peerConnectionId;
	 cout << "peerConnectionId=============================>" << peerConnectionId << endl;
     jmessage[kSessionDescriptionTypeName] = "candidate";
	 jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
	 jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
	 std::string sdp;
	 if (!candidate->ToString(&sdp)) {
     cout << "webRTCSession::OnIceCandidate,Failed to serialize candidate" << endl;
	 LOG(LS_ERROR) << "Failed to serialize candidate";
	 return;
	 }
	 jmessage[kCandidateSdpName] = sdp;
	 cout << "webRTCSession::OnIceCandidate,calling sendToStack" << endl;
	 this->sendToStack(jmessage);
}

void webRTCSession::OnError() {
	LOG(INFO) << __FUNCTION__ << " *****";
	cout << "webRTCSession::OnError" << endl;

}

/*void webRTCSession::OnSignalingChange(
		PeerConnectionInterface::SignalingState new_state) {
	LOG(INFO) << __FUNCTION__ << " *****";
}*/
void webRTCSession::OnIceConnectionChange(
//void webRTCSession::DummyOnIceConnectionChange(
		PeerConnectionInterface::IceConnectionState new_state) {
	cout << "webRTCSession::DummyOnIceConnectionChange()" << endl;
	if (new_state == 4)//kIceConnectionFailed
	{
		cout << "Inside webRTCSession::DummyOnIceConnectionChange(),new_state = kIceConnectionFailed" << endl;

	}else if(new_state == 2)//kIceConnectionConnected
	{
		cout << "Inside webRTCSession::DummyOnIceConnectionChange(),new_state = kIceConnectionConnected" << endl;

	}
}

void webRTCSession::OnIceGatheringChange(
		PeerConnectionInterface::IceGatheringState new_state) {
	cout << "webRTCSession::OnIceGatheringChange" << endl;
	if(new_state == 2)//kIceGatheringComplete
	{
		cout << "webRTCSession::OnIceGatheringChange,if new_state IS kIceGatheringComplete" << endl;
		Json::Value json;
		try
		{
		  json["type"] = "candidate";
		  json["last"] = true;
		  json["reason"] = "candidatesExhausted";
		  json["peerConnectionId"] = peerConnectionId;
		  cout << "webRTCSession::OnIceGatheringChange,setting json values" << endl;
		}catch (...) {
	        //e.printStackTrace();
			cout << "webRTCSession::OnIceGatheringChange,inside CATCH" << endl;
	        return;
	      }
		cout << "webRTCSession::OnIceGatheringChange,calling sendToStack" << endl;
		sendToStack(json);
	}
}

void webRTCSession::OnAddStream(MediaStreamInterface* stream) {
	LOG(INFO) << __FUNCTION__ << " *****";
	std::cout << "!!!!Inside OnAddStream" << std::endl;
	stream->AddRef();
}

void webRTCSession::OnRemoveStream(MediaStreamInterface* stream) {
	LOG(INFO) << __FUNCTION__ << " *****";
}

void webRTCSession::OnDataChannel(DataChannelInterface* channel) {
	LOG(INFO) << __FUNCTION__ << " *****";
}

void webRTCSession::OnRenegotiationNeeded() {
	LOG(INFO) << __FUNCTION__ << " *****";
}
//
// PeerConnectionClientObserver implementation.
//

void webRTCSession::OnSignedIn() {
  LOG(INFO) << __FUNCTION__;
}

void webRTCSession::OnDisconnected() {
  LOG(INFO) << __FUNCTION__;
}

void webRTCSession::OnPeerConnected(int id, const std::string& name) {
  LOG(INFO) << __FUNCTION__;
}

void webRTCSession::OnPeerDisconnected(int id) {
  LOG(INFO) << __FUNCTION__;

}

void webRTCSession::OnMessageFromPeer(int peer_id, const std::string& message) {
	LOG(INFO) << __FUNCTION__;
}

void webRTCSession::OnMessageSent(int err) {
  // Process the next pending message if any.
	LOG(INFO) << __FUNCTION__;
}

void webRTCSession::OnServerConnectionFailure() {
	LOG(INFO) << __FUNCTION__;
}

/*std::string webRTCSession::preferCodec(std::string sdp, string codec){  
  std::string *lineSeparator = "\n";
  std::string *mLineSeparator = " ";
  
  return sdp;
}*/


//anand

std::string webRTCSession::preferCodec(std::string sdpDescription,string codec)
{  cout<<"...sdp befor[1].."<<sdpDescription<<endl;
  sdpDescription.erase(sdpDescription.size() -2);  //last character is null character 
  cout<<"...sdp after erasing.."<<sdpDescription<<endl;
 
 std::vector<std::string> lines;
 boost::split(lines,sdpDescription,boost::is_any_of("\n"));
 cout << "* size of the vector: " << lines.size() << endl;
 cout<<"...splitted lines.."<<lines[0]<<"\n";

  int mLineIndex = -1;
  //string *codecRtpMap = NULL;  
  string mediaDescription = "m=video ";
  std::string codecRtpMap;
  boost::regex exp{"^a=rtpmap:(\\d+) H264(/\\d+)+[\r]?$"};
  //boost::regex exp{"^a=rtpmap:(\\d+) VP8(/\\d+)+[\r]?$"};
                  //a=rtpmap:100 VP8/90000\r\n

  std::string mergedSdpDescription; 
  for (string line : lines) 
     mergedSdpDescription.append(line).append("\n");
  
  //cout<<"...sdp after merging.."<<mergedSdpDescription<<endl;
  //if(sdpDescription.compare(mergedSdpDescription) == 0)
   //printf("\n both merged and original strings are same##########\n");   
  //else
   //printf("\n Strings are different \n"); 
  for(int i = 0; i < lines.size(); i++)
  {
   cout<<"Line**********" <<lines[i]<<endl;
   std::string::size_type found = lines[i].find(mediaDescription);
   if (found != std::string::npos)
   {
      cout << " media found :" <<endl;
      mLineIndex = i;
      continue;   //this is to get H264 value
   }
  
   //boost::cmatch result;
   //boost::match_results result;
   boost::match_results<std::string::const_iterator> result;
   if (boost::regex_match(lines[i], result, exp)) 
   //if (boost::regex_search(lines[i], result, exp)) 
   {
        codecRtpMap = result[1]; //storing codec value
        //codecRtpMap = lines[i];
        cout << "regex match found and the value is  :" << codecRtpMap << endl;
        cout << "result is " << result[1] << endl;
        continue;
   }
     
 }
   if (mLineIndex == -1)
   {
     cout << "no media description found" << endl;
     return sdpDescription;
   }
   if(codecRtpMap.empty()) 
   {
     cout <<  "No rtpmap found " <<endl;
     return sdpDescription;
   }
 
  
   std::vector<std::string> origMLineParts;
   boost::split(origMLineParts,lines[mLineIndex],boost::is_any_of(" "));
  
   cout << "size of origMLineParts is :" <<origMLineParts.size() <<endl; 
   if(origMLineParts.size() > 3)
   {
     std::string newMLine;
     int origPartIndex = 0;
     // Format is: m=<media> <port> <proto> <fmt> ...
     newMLine.append(origMLineParts[origPartIndex++]).append(" ");
     //cout <<"new line  after 1st append is :"<<newMLine <<endl;
     newMLine.append(origMLineParts[origPartIndex++]).append(" ");
     //cout <<"new line after 2nd append is :"<<newMLine <<endl;
     newMLine.append(origMLineParts[origPartIndex++]).append(" ");
     //cout <<"new line is 3rd append is:"<<newMLine <<endl;
     newMLine.append(codecRtpMap);
     cout <<"finally new line is :"<<newMLine <<endl;
 
    for (; origPartIndex < origMLineParts.size(); origPartIndex++) 
    {
      cout << "origPartIndex in for loop is :" << origPartIndex <<endl;
      if (origMLineParts[origPartIndex].compare(codecRtpMap)) 
      {
          newMLine.append(" ").append(origMLineParts[origPartIndex]);
      }
    }

    lines[mLineIndex] = newMLine;
    cout << "Change media description: " <<lines[mLineIndex] <<endl;

   }
   else
  {
    cout << "Wrong SDP media description format: " <<lines[mLineIndex] <<endl;
  }
  
  std::string newSdpDescription;
  std::vector<std::string>::iterator it = lines.end();
  for (std::string line : lines) 
  {
     //if(line.compare(*it) != 0)
      newSdpDescription.append(line).append("\n");
     //else
      //newSdpDescription.append(line);

#if 0
	static int a=0;
       if(a==0)
	{
	
	cout<<"Line++++++++++++++++" <<line<<endl;
     newSdpDescription.append(line).append("\r\n");
	}
	a=!a;
     //newSdpDescription.append(line).append("\r\n");
#endif
  }
  cout<<"...sdp after[1] before erasing..."<<newSdpDescription<<endl;  
  //cout << "VASUDHA size of newdescription 1: " <<newSdpDescription.size() <<endl;
//  newSdpDescription.erase(newSdpDescription.size() -1);
  //cout << "VASUDHA size of newdescription 2: " <<newSdpDescription.size() <<endl;
  //newSdpDescription.erase(newSdpDescription.size() -1);
  //cout << "VASUDHA size of newdescription 3: " <<newSdpDescription.size() <<endl;i
	//newSdpDescription.pop_back();
	//newSdpDescription.resize(newSdpDescription.size() -1);
       //newSdpDescription.replace(newSdpDescription.size() -1, 1, "\0");
  cout<<"...sdp after[1]..."<<newSdpDescription<<endl;  
  if(sdpDescription.compare(newSdpDescription) == 0)
   printf("VASUDHA both strings are same##########\n");   
  else
   printf("Strings are different \n"); 
  return newSdpDescription;
}
