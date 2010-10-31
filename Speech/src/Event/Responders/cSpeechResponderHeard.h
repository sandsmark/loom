#pragma once

#include <Speech/Event/Responders/ISpeechResponder.h>

BEGIN_NAMESPACE( Speech )

class cSpeechResponderHeard : public ISpeechResponder<cSpeechResponderHeard>
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
	cSpeechResponderHeard() : ISpeechResponder( _T("Speech::Heard") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &ISpeechEvent::OnHeard, vParam.text );
	}
};

END_NAMESPACE()