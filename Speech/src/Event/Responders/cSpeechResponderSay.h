#pragma once

#include <Speech/Event/Responders/ISpeechResponder.h>
#include <stdio.h>
#include <string.h>

BEGIN_NAMESPACE( Speech )

class cSpeechResponderSay : public ISpeechResponder<cSpeechResponderSay>
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
			if (vData) {
				text = (wchar_t *)vData;
			//	size = strlen(vData) + 1;
			//	text = new char[size];
			//	memcpy(text, vData, size);
			}
			else {
			//	text = NULL;
			//	size = 0;
			}
		}
		~cParam() {
		//	delete [] text;
		//	text = NULL;
		//	size = 0; 
		}
	};

public:
	cSpeechResponderSay() : ISpeechResponder( _T("Speech::Say") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &ISpeechEvent::OnSay, vParam.text );
	}
};

END_NAMESPACE()