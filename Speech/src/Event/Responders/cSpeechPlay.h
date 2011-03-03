#pragma once

#include <Speech/Event/Responders/ISpeechPlayResponder.h>

BEGIN_NAMESPACE( Speech )

class cSpeechPlay : public ISpeechPlayResponder<cSpeechPlay>
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
	cSpeechPlay() : ISpeechPlayResponder( _T("Speech::Play") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &ISpeechPlayEvent::OnSpeechPlay );
	}
};

END_NAMESPACE()