#pragma once

#include <Speech/Event/Responders/ISpeechQueueResponder.h>

BEGIN_NAMESPACE( Speech )

class cSpeechQueue : public ISpeechQueueResponder<cSpeechQueue>
{
public:
	class cParam
	{
	public:
		std::wstring text;
	public:
		cParam() {}
		cParam( const cDispatcherHub::IParam &iParam )
		{
			char *vData = (char*)iParam.GetData();
			if (vData)
				text = (wchar_t *)vData;
		}
	};

public:
	cSpeechQueue() : ISpeechQueueResponder( _T("Speech::Queue") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &ISpeechQueueEvent::OnSpeechQueue, vParam.text );
	}
};

END_NAMESPACE()