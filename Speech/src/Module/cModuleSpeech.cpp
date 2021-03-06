#include <Speech/Module/cModuleSpeech.h>
#include <Core/Debug/Logger/cLogger.h>
#include <Core/Event/cDispatcherHub.h>
#include <Speech/Event/cSpeechResponders.h>
#include <module.h>
#include <string>
#include <list>

//#define PITCHTRACKER

using Loom::Core::cLogger;
using Loom::Core::cDispatcherHub;

using namespace Loom::Speech;

/************************************************************************/
cModuleSpeech::cModuleSpeech()
: IModule( GetName() )
/************************************************************************/
{
	cpVoice = NULL;
	cpGrammar = NULL;
	cpRecoCtxt = NULL;
	mThread = NULL;
}

/************************************************************************/
void cModuleSpeech::Init( void )
/************************************************************************/
{
	mInitialized = true;
#ifdef PITCHTRACKER
	SpeechPitchIntegrator::StartSpeechPitchIntegrator();
#endif

	cLogger &vLogger = cLogger::Get();
	vLogger.Log( cLogger::LOG_INFO, _T( "Global" ), _T( "cModuleSpeech startup" ) );

	cSpeechResponderSay::Get().AddListener( *this );
	cSpeechQueue::Get().AddListener( * this);
	cSpeechPlay::Get().AddListener( *this);
	HRESULT hr = E_FAIL;
	if (FAILED(hr = ::CoInitialize(NULL)))
		return;

	hr = cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);
	if(FAILED(hr))
		return;

	hr = cpRecoCtxt->GetVoice(&cpVoice);

	// For MS Speech Reco alone, comment out for PitchTracker
#ifndef PITCHTRACKER
	if (cpRecoCtxt && cpVoice &&
		SUCCEEDED(hr = cpRecoCtxt->SetNotifyWin32Event()) &&
		SUCCEEDED(hr = cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) &&
		SUCCEEDED(hr = cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)) &&
		SUCCEEDED(hr = cpRecoCtxt->CreateGrammar(0, &cpGrammar)) &&
		SUCCEEDED(hr = cpGrammar->LoadDictation(NULL, SPLO_STATIC)) &&
		SUCCEEDED(hr = cpGrammar->SetDictationState(SPRS_ACTIVE)))
	{}
	else
		return;
#endif
	// *****************************************************

	mThread = CreateThread( NULL, 0, StartThread, this, 0, NULL );
	mInitialized = true;
}


/************************************************************************/
void cModuleSpeech::OnSay( const std::wstring &text )
/************************************************************************/
{
	if (cpVoice)
		cpVoice->Speak( text.c_str(), SPF_ASYNC, NULL );	
}

/************************************************************************/
void cModuleSpeech::OnSpeechPlay()
/************************************************************************/
{
	if (cpVoice)
		cpVoice->Speak( wstrSavedString.c_str(), SPF_ASYNC, NULL );	
}

/************************************************************************/
void cModuleSpeech::OnSpeechQueue( const std::wstring &text )
/************************************************************************/
{
	wstrSavedString = text;	
}
/************************************************************************/
void cModuleSpeech::OnHeard( const std::wstring &text )
/************************************************************************/
{
	text.begin();
}

/************************************************************************/
DWORD cModuleSpeech::StartThread( LPVOID arg )
/************************************************************************/
{

	cModuleSpeech* _this = (cModuleSpeech*) arg;

	_this->OnSay(_T("Pitch Tracker is running..."));

	int nSize = 0;

	//For Pitch Tracker alone, comment out for MS Speech Reco
#ifdef PITCHTRACKER
	char str[1024];
	int strC = 0;
	while (true) {
		SpeechPitchIntegrator::UpdatePitchBuffer();
		nSize = SpeechPitchIntegrator::GetPitchBufferDataSize();
		if (nSize > 0)
		{
			// Pitch data is available
			__int64* oTimestamps = new __int64[nSize];
			double* oPitchValues = new double[nSize];
			bool* oVoicedValues = new bool[nSize];
			__int64* oWordBoundries = NULL;
			std::list<std::string> oWords;
			// Query C++ DLL for fresh data
			__int64* ptrTS = SpeechPitchIntegrator::GetPitchTimestamps();
			double* ptrPI = SpeechPitchIntegrator::GetPitchValues();
			char* ptrW = SpeechPitchIntegrator::GetWords();
			bool* ptrV = SpeechPitchIntegrator::GetVoicedValues();
			for (int i = 0; i < nSize; i++)
			{
				oTimestamps[i] = ptrTS[i];
				oPitchValues[i] = ptrPI[i];
				oVoicedValues[i] = ptrV[i];
			}
			__int64* ptrWB = SpeechPitchIntegrator::GetWordBoundries();
			if (ptrWB != NULL)
			{
				oWordBoundries = new __int64[ptrWB[0]];
				for (int i = 0; i < ptrWB[0]; i++)
					oWordBoundries[i] = ptrWB[i + 1];
			}
			if (ptrW != NULL)
			{
				//System.Text.StringBuilder oSbWords = new StringBuilder();
				int nIndex = 0;
				char c;
				do
				{
					c = (char)ptrW[nIndex++];
					if ( c != '|' )
					{
						str[strC++] = c;
					// oSbWords.Append(c);
					}
				} while (c != '|');
				str[strC++] = 0;
				size_t vPos = 0;
				size_t vLength = strlen( str );
				for ( size_t vPos=0; vPos < vLength; vPos++ )
				{
					if ( str[vPos] == ' ' ) continue;
					char *vStart = str + vPos;
					for ( /*nothing*/; vPos < vLength && str[vPos] != ' '; vPos++ );
					char *vEnd = str + vPos;
					*vEnd = 0;
					oWords.push_back( vStart );
				}
				strC = 0;
				str[0] = 0;
			}      
			SpeechPitchIntegrator::ClearWordBuffer();
		}

		Sleep(100);
	}
	/*****************************************************/
#else
	// For MS Speech Reco alone, comment out for PitchTracker
	std::wstring text;
	const wchar_t *vReceivedMessageName = _T("Speech::Heard");

	// Do listening and routing here
	// Listen for message name and data and send with Dispatch
	for ( ;; )
	{
		HRESULT hr = S_OK;
		const WCHAR * const pchStop = _this->StopWord();
		CComPtr<ISpRecoResult> cpResult;

		if ((hr = _this->waitForSpeech(_this->cpRecoCtxt, &cpResult, 1000)) == S_OK)
		{
			_this->cpGrammar->SetDictationState( SPRS_INACTIVE );

			CSpDynamicString dstrText;

			if (SUCCEEDED(cpResult->GetText((ULONG)SP_GETWHOLEPHRASE, (ULONG)SP_GETWHOLEPHRASE, TRUE, &dstrText, NULL)))
			{
				printf("I heard:  %s\n", (LPSTR)CW2A(dstrText));
				text = dstrText;
				cDispatcherHub::IParam vReceivedParam( (void*)text.c_str() );

				// Dispatch received message
				cDispatcherHub::Get().Dispatch( vReceivedMessageName, vReceivedParam );

				//if (fUseTTS)
				//{
				//	cpVoice->Speak( L"I heard", SPF_ASYNC, NULL);
				//	cpVoice->Speak( dstrText, SPF_ASYNC, NULL );
				//}

				//if (fReplay)
				//{
				//	if (fUseTTS)
				//		cpVoice->Speak( L"when you said", SPF_ASYNC, NULL);
				//	else
				//		printf ("\twhen you said...\n");
				//	cpResult->SpeakAudio(NULL, 0, NULL, NULL);
				//}

				cpResult.Release();
			}
			if (_wcsicmp(dstrText, pchStop) == 0)
			{
				break;
			}

			_this->cpGrammar->SetDictationState( SPRS_ACTIVE );
		} 
	}
	// *****************************************************
#endif

	return 0;
}

/************************************************************************/
void cModuleSpeech::Destroy( void )
/************************************************************************/
{
	TerminateThread( mThread, 0 );
	::CoUninitialize();

	mInitialized = false;
}




