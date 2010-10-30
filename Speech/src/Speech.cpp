#include <Speech/Speech.h>
#include <Speech/Module/cModuleSpeech.h>

using namespace Loom::Speech;

extern "C"
{

SPEECH_API IModule *CreateModule( void )
{
	return new cModuleSpeech();
}

}
