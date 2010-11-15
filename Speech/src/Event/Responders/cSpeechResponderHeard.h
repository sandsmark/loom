#pragma once

#include <Speech/Event/Responders/ISpeechListenerResponder.h>

BEGIN_NAMESPACE( Speech )

class cSpeechResponderHeard : public ISpeechListenerResponder<cSpeechResponderHeard>
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
	cSpeechResponderHeard() : ISpeechListenerResponder( _T("Speech::Heard") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &ISpeechListenerEvent::OnHeard, vParam.text );
	}
};

END_NAMESPACE()