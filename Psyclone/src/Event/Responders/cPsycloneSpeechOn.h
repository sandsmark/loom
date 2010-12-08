#pragma once

#include <Psyclone/Event/Responders/IPsycloneSpeechOnResponder.h>

BEGIN_NAMESPACE( Psyclone )

class cPsycloneSpeechOn : public IPsycloneSpeechOnResponder<cPsycloneSpeechOn>
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
	cPsycloneSpeechOn() : IPsycloneSpeechOnResponder( _T("Psyclone::SpeechOn") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IPsycloneSpeechOnEvent::OnSpeechOn, vParam.text );
	}
};

END_NAMESPACE()