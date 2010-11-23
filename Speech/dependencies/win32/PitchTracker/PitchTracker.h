//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/  CADIA.Module.PitchTracker
//_/_/  Eric Nivel
//_/_/
//_/_/  Copyright(c)2007 Center for Analysis and Design of Intelligent Agents
//_/_/                   Reykjavik University
//_/_/                   All rights reserved
//_/_/
//_/_/                   http://cadia.ru.is/
//_/_/
//_/_/  Redistribution and use in source and binary forms, with or without
//_/_/  modification, is permitted provided that the following conditions 
//_/_/  are met:
//_/_/
//_/_/  - Redistributions of source code must retain the above copyright notice,
//_/_/    this list of conditions and the following disclaimer.
//_/_/
//_/_/  - Redistributions in binary form must reproduce the above copyright 
//_/_/    notice, this list of conditions and the following disclaimer in the 
//_/_/    documentation and/or other materials provided with the distribution.
//_/_/
//_/_/  - Neither the name of its copyright holders nor the names of its 
//_/_/    contributors may be used to endorse or promote products derived from 
//_/_/    this software without specific prior written permission.
//_/_/
//_/_/  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//_/_/  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//_/_/  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
//_/_/  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
//_/_/  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//_/_/  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
//_/_/  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
//_/_/  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//_/_/  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
//_/_/  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef	PITCH_TRACKER_H
#define	PITCH_TRACKER_H

#include	<windows.h>

#include "Core.h"
#include "AudioDevice.h"

class	WaveletPitchProcessor;

//	pitch monitoring
typedef	struct
{
	double			value;
	long	long	time;
	bool			voiced;
} PitchSample;

class	PitchTracker{

	private:

		static	PitchTracker	Singleton;

		static	DWORD	WINAPI	ThreadLogic(LPVOID	args);

		typedef	enum{
					RUNNING=0,
					STOPPED
		}State;

		State	state;

		bool	shutdown;

		AudioDevice				*audioStream;
		WaveletPitchProcessor	*pitchProcessor;

		unsigned	int	deviceID;
		int				bufferSize;
		unsigned	int	samplingFrequency;
		double			amplitudeThreshold;
		double			noiseThreshold;
		int				intermediateBufferSize;
		double			amplificationFactor;

		unsigned int	iBufferIndex;

		// Buffer mutex
		HANDLE hBufferMutex;

private:

		short	*buffer;	//	stream buffer

		// Buffering of pitch data
		PitchSample *pitchBuffer;

		PitchSample *pitchResults;

		PitchTracker();

		long long lastVoicedToSilentTransitionTime;
		long long lastSilentToVoicedTransitionTime;

		void	start();
		void	stop();

	public:

		static	void	Init(	unsigned	int	deviceID,
											int	bufferSize,
								unsigned	int	samplingFrequency,
										 double amplitudeThreshold,
										 double noiseThreshold,
										    int intermediateBufferSize,
										 double amplificationFactor
		);

		static void UpdateParams(	double		pamplitudeThreshold,
									double		    pnoiseThreshold,
									double		pamplifcationFactor );



		static	PitchTracker	*Get();
		static	PitchTracker	*GetSingleton();

		~PitchTracker();

		void	run();

		static PitchSample* CollectSamples(unsigned int &size);

		static long long GetLastVoicedToSilentTime();
		static long long GetLastSilentToVoicedTime();
};


#endif