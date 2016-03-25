
#include "talk/examples/peerconnection/client/conductor.h"
#include <utility>
#include <vector>

#include "WebRTCStream.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "talk/examples/peerconnection/client/defaults.h"
#include "talk/media/devices/devicemanager.h"
#include "talk/media/base/fakevideocapturer.h"

WebRTCStream::WebRTCStream()
{

}

WebRTCStream::WebRTCStream(string camera, int minVideoHeight, int maxVideoHeight, int minVideoWidth, int maxVideoWidth)
{
    this->camera = camera;
    minHeight = minVideoHeight;
    minWidth = minVideoWidth;
    maxHeight = maxVideoHeight;
    maxWidth = maxVideoWidth;
}

void WebRTCStream::localTrack()
{

}

cricket::VideoCapturer* WebRTCStream::getCapturerDevice(){
	cout << "webRTCSession::getCapturerDevice" << endl;
	  rtc::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
			   cricket::DeviceManagerFactory::Create());
	  if (!dev_manager->Init()) {
		  cout << "webRTCSession::getCapturerDevice error" << endl;
		  LOG(LS_ERROR) << "Can't create device manager" << std::endl;
	    return NULL;
	  }
#if 0
	  std::vector<cricket::Device> devs;
	  if (!dev_manager->GetVideoCaptureDevices(&devs)) {
		  cout << "webRTCSession::getCapturerDevice error2 " << endl;
	    LOG(LS_ERROR) << "Can't enumerate video devices";
	    return NULL;
	  }

	  std::vector<cricket::Device>::iterator dev_it = devs.begin();
#endif 
	  cricket::VideoCapturer* capturer = NULL;
/*	  for (; dev_it != devs.end(); ++dev_it) {
	    capturer = dev_manager->CreateVideoCapturer(*dev_it);
	    cout << "webRTCSession::getCapturerDevice after createCapturer" << endl;
	    if (capturer != NULL)
	      break;
	  }
*/
	 printf(" webRTCSession::getCapturerDevice call yuvFramesGenerator\n");
	const cricket::Device  device_temp("YuvFramesGenerator", "YuvFramesGenerator_id");
        capturer = dev_manager->CreateVideoCapturer(device_temp);

	   printf(" webRTCSession::getCapturerDevice create fakevideocapturer and return +++\n");
    
//        capturer = new cricket::FakeVideoCapturer();	
	  return capturer;

}

int WebRTCStream::start()
{
	return 0;
}

int WebRTCStream::stop()
{
	return 0;
}

bool WebRTCStream::IsStarted()
{
	return false;
}

MediaStream* WebRTCStream::getMediaStream()
{
	return NULL;
}
