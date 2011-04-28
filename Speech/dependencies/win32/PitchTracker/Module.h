//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/  SpeechPitchIntegrator
//_/_/  Helgi Páll Helgason
//_/_/
//_/_/  Copyright(c)2009 Center for Analysis and Design of Intelligent Agents
//_/_/                   Reykjavik University
//_/_/                   All rights reserved
//_/_/
//_/_/                   http://cadia.ru.is/
//_/_/
//_/_/  Redistribution and use in source and binary forms, with or without
//_/_/  modification, is permitted provided that the following conditions 
//_/_/  are met:
//_/_/
//_/_/  - Redistributions of source code must retain the above copyright notice,
//_/_/    this list of conditions and the following disclaimer.
//_/_/
//_/_/  - Redistributions in binary form must reproduce the above copyright 
//_/_/    notice, this list of conditions and the following disclaimer in the 
//_/_/    documentation and/or other materials provided with the distribution.
//_/_/
//_/_/  - Neither the name of its copyright holders nor the names of its 
//_/_/    contributors may be used to endorse or promote products derived from 
//_/_/    this software without specific prior written permission.
//_/_/
//_/_/  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//_/_/  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//_/_/  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
//_/_/  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
//_/_/  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//_/_/  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
//_/_/  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
//_/_/  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//_/_/  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//_/_/  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef SPIMODULE_H_
#define SPIMODULE_H_

// Define the size of the circular array that keeps track of sound start
// events from the PitchTracker
#define PT_SOUNDSTART_BUFFERSIZE 25
#define PT_SOUNDEND_BUFFERSIZE 25

// Average spoken words per minute (English)
#define AVG_WORDS_PER_MIN 155
// Average syllables per word (English)
#define AVG_SYLLABLES_PER_WORD 1.2

#include <sapi.h>
#include <iostream>
#include <atlcomcli.h>
#include "PitchTracker.h"
//#include "PTListener.h"
#include "Results.h"

using namespace std;

// There are three grammars loaded
typedef enum GRAMMARIDS
{
    GID_DICTATION,      // ID for the dictation grammar
    GID_DICTATIONCC,    // ID for the C&C grammar that's active during dictation
    GID_CC              // ID for the C&C grammar that's active when dictation is not
};

class SpeechPitchIntegrator
{
public:
	static SpeechPitchIntegrator* GetSingleton();
	static void __stdcall SAPI_Callback(WPARAM wParam, LPARAM lParam);
	void ReportPTVoiceStart(long long timestamp);
	void ReportPTVoiceEnd(long long timestamp);
	void Stop();
	__declspec( dllexport ) static void StartSpeechPitchIntegrator();
	__declspec( dllexport ) static int IsResultReady();
	__declspec( dllexport ) static int GetWordDataSize();
	__declspec( dllexport ) static int GetPitchDataSize();
	__declspec( dllexport ) static double* GetWordDataX();
	__declspec( dllexport ) static double* GetWordDataY();
	__declspec( dllexport ) static double* GetPitchDataX();
	__declspec( dllexport ) static double* GetPitchDataY();
	__declspec( dllexport ) static void ClearResultFlag();

	__declspec( dllexport ) static void UpdatePitchBuffer();
	__declspec( dllexport ) static int GetPitchBufferDataSize();
	__declspec( dllexport ) static long long * GetPitchTimestamps();
	__declspec( dllexport ) static double * GetPitchValues();
	__declspec( dllexport ) static bool * GetVoicedValues();
	__declspec( dllexport ) static long long * GetWordBoundries();
	__declspec( dllexport ) static char* GetWords();
	__declspec( dllexport ) static void UpdatePitchTrackerParams(double AmplitudeThreshold, double NoiseThreshold, double AmplificationFactor);

	__declspec( dllexport ) static void ClearWordBuffer();

private:
	SpeechPitchIntegrator();
	~SpeechPitchIntegrator();
	static SpeechPitchIntegrator* PtrSingleton;
	HANDLE hDataMutex;

protected:
	PitchSample* mPitchBuffer;	
	unsigned int mPitchBufferSize;

	bool mIsResultReady;
	bool mStopMessageLoop;
	CComPtr<ISpRecognizer> mRecoEngine;
	CComPtr<ISpRecoContext> mContext;
	CComPtr<ISpRecoGrammar> mDictationGrammar;
	CComPtr<ISpRecoGrammar> mCustomGrammar;
	CComPtr<ISpNotifyTranslator> mNotifier;
	static	DWORD	WINAPI	DoMessageLoop(LPVOID	args);
	bool InitializeSAPI();
	
	void HandleUtteranceStart(long long timestampSAPI);
	void HandleUtteranceEnd(long long timestampSAPI);
	void HandleHypothesisUpdate(long long timestampSAPI, WCHAR* hypothesis, bool IsFinal);
	void HandleRecognition(long long timestampSAPI, WCHAR* hypothesis);

	PitchTracker* mPitchTracker;
	//PTListener * mPitchListener;

	long long mPTSoundStartBuffer[PT_SOUNDSTART_BUFFERSIZE];
	long long mPTSoundEndBuffer[PT_SOUNDSTART_BUFFERSIZE];
	long long mSoundStartDelta;
	long long mSoundStart;

	unsigned int mPTSoundStartBufferIndex;
	unsigned int mPTSoundEndBufferIndex;
	unsigned int mSAPIHypothesisCount;

	bool mStateInUtterance;

	int nArrayWordsLength;
	int nArrayPitchLength;
	//double* mArrayWordsX;
	//double* mArrayWordsY;
	//double* mArrayPitchX;
	//double* mArrayPitchY;

	int nSAPIHypoCount;
	int nSAPIUtteranceCount;

	unsigned int mLastMarkedExpired;
	unsigned int mWordIndexAtUtteranceStart;

	vector<ResSpeechPitchWord>* mWordVector;
};

#endif