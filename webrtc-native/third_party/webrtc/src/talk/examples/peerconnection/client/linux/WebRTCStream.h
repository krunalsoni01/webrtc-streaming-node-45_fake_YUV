#ifndef CLIENT_LINUX_WEBRTCSTREAM_H
#define CLIENT_LINUX_WEBRTCSTREAM_H

#include <stdio.h>
#include <iostream>
#include "talk/app/webrtc/mediastream.h"
#include "webrtc/base/refcount.h"
//#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
//#include "talk/base/scoped_ptr.h"

using namespace std;
using namespace webrtc;

namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket

class WebRTCStream
{
public:
	WebRTCStream();
    WebRTCStream(string camera, int minVideoHeight, int maxVideoHeight, int minVideoWidth, int maxVideoWidth);
    int start();
    int stop();
    bool IsStarted();
    MediaStream* getMediaStream();
    cricket::VideoCapturer* getCapturerDevice();
private:
    void localTrack();
    string camera;
    int minHeight;
    int minWidth;
    int maxHeight;
    int maxWidth;
    bool isStarted;
};

#endif // WEBRTCSTREAM_H
