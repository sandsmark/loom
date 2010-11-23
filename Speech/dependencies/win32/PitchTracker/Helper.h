#ifndef SPIHELPER_H_
#define SPIHELPER_H_

#include <vector>
#include <math.h>
using namespace std;

class SPIHelper
{
public:
	// Tradional delimeter based string split
	static vector<string> StringSplit(string str, string delim)
	{
		vector<string> oResults;
		int nCutAt;
		while ( (nCutAt = str.find_first_of(delim)) != str.npos )
		{
			if ( nCutAt > 0 )
				oResults.push_back(str.substr(0, nCutAt));
			str = str.substr(nCutAt+1);			
		}
		if ( str.length() > 0 ) 
			oResults.push_back(str);
		return oResults;
	}

	// Convert a string to lower case
	static string StringToLower(string Input)
	{
		for ( unsigned int i = 0; i < Input.length(); i++ )
			if ( Input[i] >= 0x41 && Input[i] <= 0x5A )
				Input[i] = Input[i] + 0x20;
		return Input;
	}

	// Check if character is a vowel
	static bool IsCharVowel(char letter)
	{
		return ( letter == 'a' || letter == 'e' || letter == 'y' || letter == 'u' || letter == 'i' || letter == 'o' );  
	}

	// Simple algorithm for counting number of syllables in a single word
	static int CountSyllablesInWord(string Word) 
	{
		if ( Word.length() <= 3 ) 
			return 1;

		string WordEnd = Word.substr(Word.length()-2, 2);
		if ( WordEnd.compare("es") == 0 || WordEnd.compare("ed") == 0 )
			Word = Word.substr(0, Word.length() - 2);
		else
		{
			if ( WordEnd.compare("le") != 0 && WordEnd[1] == 'e' ) 
				Word = Word.substr(0, Word.length() - 1);
		}
		bool bLastWasVowel = false;
		int nSyllableCount = 0;
		for ( unsigned int i = 0; i < Word.length(); i++ )
		{
			bool bVowel = IsCharVowel(Word[i]);
			if ( bVowel && !bLastWasVowel )
				nSyllableCount++;
			bLastWasVowel = bVowel;
		}
		if ( Word.substr(0, 2).compare("mc") == 0 )
			nSyllableCount++;
		return nSyllableCount;
	}

	static int ExtractGistOfPitch(vector<double>* pitchData)
	{
		if ( pitchData->size() < 3 ) 
			return 0;
		int nMaxUp = 0;
		int nMaxDown = 0;
		int nCurrUp = 0;
		int nCurrDown = 0;
		for ( unsigned int i = 1; i < pitchData->size(); i++ )
		{
			if ( pitchData->at(i) > pitchData->at(i-1) )
			{
				nCurrUp++;
				if ( nCurrDown > nMaxDown )
					nMaxDown = nCurrDown;
				nCurrDown = 0;
			}
			else
			if ( pitchData->at(i) < pitchData->at(i-1) )
			{
				nCurrDown++;
				if ( nCurrUp > nMaxUp )
					nMaxUp = nCurrUp;
				nCurrUp = 0;
			}
			else
			{
				if ( nCurrUp > nMaxUp )
					nMaxUp = nCurrUp;
				if ( nCurrDown > nMaxDown )
					nMaxDown = nCurrDown;
				nCurrUp = 0;
				nCurrDown = 0;
			}
		}
		if ( nMaxDown > nMaxUp )
			return -nMaxDown;
		if ( nMaxUp > nMaxDown )
			return nMaxUp;
		return 0;
	}

	static double CalculateWordConfidence(unsigned int HypoId, unsigned int SeenBefore)
	{
		return (((double)HypoId * (double)SeenBefore) + 1.0) / pow((double)HypoId + (double)SeenBefore + 1.0, 1.5);
	}

};



#endif