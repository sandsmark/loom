#pragma once

#include <Core/Module/IModule.h>
#include <Speech/Event/ISpeechEvent.h>
#include <Speech/Event/ISpeechListenerEvent.h>
using Loom::Core::IModule;

#include <windows.h>
#include <sapi.h>
#include <stdio.h>
#include <string.h>
#include <atlbase.h>
#include "sphelper.h"

BEGIN_NAMESPACE( Speech )

class SPEECH_API cModuleSpeech : public IModule, public ISpeechEvent, public ISpeechListenerEvent
{
protected:
	HANDLE mThread;
	CComPtr<ISpRecoContext> cpRecoCtxt;
	CComPtr<ISpRecoGrammar> cpGrammar;
	CComPtr<ISpVoice> cpVoice;

public:
	cModuleSpeech();

	virtual void Init( void );
	virtual void Destroy( void );

	static DWORD WINAPI StartThread( LPVOID iParam );
	
	virtual void OnSay( const std::wstring &text );
	virtual void OnHeard( const std::wstring &text );

	// Automatic inline
	HRESULT waitForSpeech(ISpRecoContext * pRecoCtxt, ISpRecoResult ** ppResult, unsigned int timeout)
	{
		HRESULT hr = S_OK;
		CSpEvent event;

		if ((hr = pRecoCtxt->WaitForNotifyEvent(timeout)) == S_FALSE)
			return S_FALSE;

		if FAILED(hr = event.GetFrom(pRecoCtxt))
			return S_FALSE;

		*ppResult = event.RecoResult();
		if (*ppResult)
			(*ppResult)->AddRef();

		return hr;
	}

	// Automatic inline
	HRESULT BlockForResult(ISpRecoContext * pRecoCtxt, ISpRecoResult ** ppResult)
	{
		HRESULT hr = S_OK;
		CSpEvent event;

		while (SUCCEEDED(hr) &&
			   SUCCEEDED(hr = event.GetFrom(pRecoCtxt)) &&
			   hr == S_FALSE)
		{
			hr = pRecoCtxt->WaitForNotifyEvent(INFINITE);
		}

		*ppResult = event.RecoResult();
		if (*ppResult)
		{
			(*ppResult)->AddRef();
		}

		return hr;
	}

	// Automatic inline
	const WCHAR * StopWord()
	{
		const WCHAR * pchStop;
	    
		LANGID LangId = ::SpGetUserDefaultUILanguage();

		switch (LangId)
		{
			case MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT):
				pchStop = L"\x7d42\x4e86\\\x30b7\x30e5\x30fc\x30ea\x30e7\x30fc/\x3057\x3085\x3046\x308a\x3087\x3046";;
				break;

			default:
				pchStop = L"Stop";
				break;
		}

		return pchStop;
	}

	static const cString &GetName( void )
	{
		static const cString vName = _T("Speech");
		return vName;
	}


};

END_NAMESPACE()
