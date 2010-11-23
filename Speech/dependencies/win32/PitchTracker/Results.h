#ifndef SPIRESULTS_H_
#define SPIRESULTS_H_

#include <vector>

using namespace std;

// Result for a single word
struct ResSpeechPitchWord
{
	string Word;
	vector<double> PitchData;
	int PitchGist;
	long long StartTimeStamp;
	long long EndTimeStamp;
	long long StartTimeRelative;
	long long EndTimeRelative;
	int NumberOfSyllables;
	int HypoId;
	int UtteranceId;
	int Final;
	bool Expired;
	int SeenBefore;
	double ConfidenceReco;
	int UtterancePosition;
};

/*
// Result for a complete utterances
struct ResSpeechPitchResult
{
	string SpeechRecoResult;
	double ConfidenceReco;
	double ConfidencePitch;
	int NumberOfWords;
	long long StartTimeStamp;
	long long EndTimeStamp;
	vector<ResSpeechPitchWord> SubResults;
	bool IsFinal;
};
*/

#endif