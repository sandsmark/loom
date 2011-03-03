#pragma once

#include <Psyclone/Event/Responders/IDialogueStatusResponder.h>

BEGIN_NAMESPACE( Psyclone )

class cDialogueStatus : public IDialogueStatusResponder<cDialogueStatus>
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
	cDialogueStatus() : IDialogueStatusResponder( _T("Dialogue::Status") ) {}

	virtual void Respond( const cDispatcherHub::IParam &iParam )
	{
		cParam vParam( iParam );
		Dispatch( &IDialogueStatusEvent::DialogueStatus, vParam.text );
	}
};

END_NAMESPACE()