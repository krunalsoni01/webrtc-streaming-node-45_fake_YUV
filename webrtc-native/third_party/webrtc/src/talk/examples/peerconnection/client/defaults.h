/*
 * libjingle
 * Copyright 2011 Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PEERCONNECTION_SAMPLES_CLIENT_DEFAULTS_H_
#define PEERCONNECTION_SAMPLES_CLIENT_DEFAULTS_H_
#pragma once

#include <string>

#include "webrtc/base/basictypes.h"

#if 0
extern const char kAudioLabel[];
extern const char kVideoLabel[];
extern const char kStreamLabel[];
extern const uint16 kDefaultServerPort;
#define JSEP_SIP_GATEWAY_SUPPORT
#define JSEP_UPGRADED_GATEWAY_SUPPORT
#ifdef JSEP_SIP_GATEWAY_SUPPORT      
extern const char    kfromAliasIdStr[];
extern const char    ktoAliasIdStr[];
extern const char    ktargetStr[];
extern const char    kfromStr[];
extern const char    ksdpStr[];
extern const char    kofferStr[];
extern const char    ktypeStr[];
extern const char    kanswerStr[];
extern const char    kcandidateStr[];
extern const char    kregSuccess[];
extern const char    kNotification[];
extern const char    keventStr[];
extern const char    kvideourlStr[];
extern const char    kthumburlStr[];
extern const char    ktitleStr[];
extern const char    kIceServers[];
#endif
#endif

const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamLabel[] = "stream_label";
const uint16 kDefaultServerPort = 8888;

#define JSEP_SIP_GATEWAY_SUPPORT
//#ifndef SIGNALLING_LIB_SUPPORT
#ifdef JSEP_SIP_GATEWAY_SUPPORT
#define JSEP_UPGRADED_GATEWAY_SUPPORT
#ifndef JSEP_UPGRADED_GATEWAY_SUPPORT
const char    kfromAliasIdStr[]="fromAlias";
const char    ktoAliasIdStr[]="toAlias";
const char    ktargetStr[]="target";
const char    kfromStr[]="from";
#else
const char    kfromAliasIdStr[]="from";
const char    ktoAliasIdStr[]="to";
const char    ktargetStr[]="target";
const char    kfromStr[]="clientid";
#endif
const char    ksdpStr[]="sdp";
const char    ktypeStr[]="type";
const char    kofferStr[]="offer";
const char    kanswerStr[]="answer";
const char    kcandidateStr[]="candidate";
const char    kregSuccess[]="regsuccess";
const char    kNotification[]="notification";
const char    kSessionmessage[]="sessionMessage";
const char    kphoto[]="photo";
const char    kchat[]="chat";
const char    keventStr[]="event";
const char    kvideourlStr[]="videoUrl";
const char    kthumburlStr[]="thumbUrl";
const char    ktitleStr[]="title";

const char    kByeStr[]="bye";
const char    kSessionMessage[]="sessionMessage";
const char    kUrl[]="url";
const char    kMsg[]="msg";
const char    kIceServers[]="iceServers";

//#endif
#endif


std::string GetEnvVarOrDefault(const char* env_var_name,
                               const char* default_value);
std::string GetPeerConnectionString();
std::string GetDefaultServerName();
std::string GetPeerName();

#endif  // PEERCONNECTION_SAMPLES_CLIENT_DEFAULTS_H_
