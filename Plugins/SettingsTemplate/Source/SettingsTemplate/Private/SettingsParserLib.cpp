// Fill out your copyright notice in the Description page of Project Settings.
#include "SettingsParserLib.h"

int USettingsParserLib::FindMatchingBracket(FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket)
{
	
	int length = UKismetStringLibrary::Len(Input);
	int EncounteredBrackets = 0;
	auto OB = OpeningBracket[0];
	auto CB = ClosingBracket[0];
	for (int i = FirstBracket; i < length; i++)
	{
		if (Input[i] == OB)
		{
			EncounteredBrackets++;
		}
		else if (Input[i] == CB)
		{
			EncounteredBrackets--;
		}
		if (EncounteredBrackets == 0)
		{
			return(i);
		}
		
	}
	return(-1);
}

FString USettingsParserLib::GetBracketContents(FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket)
{
	return(UKismetStringLibrary::GetSubstring(Input, FirstBracket + 1, FindMatchingBracket(Input, FirstBracket, OpeningBracket, ClosingBracket) - FirstBracket - 1));
}

FString USettingsParserLib::GetBracketContentsAndIndex(FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket, int& ReturnClosingBracketIndex)
{
	int ClosingBracketIndex = FindMatchingBracket(Input, FirstBracket, OpeningBracket, ClosingBracket);
	ReturnClosingBracketIndex = ClosingBracketIndex;
	return(UKismetStringLibrary::GetSubstring(Input, FirstBracket + 1, ClosingBracketIndex - FirstBracket - 1));
}

bool USettingsParserLib::ParseSettings(FString& Input, FSettings& out, int& EndIndex)
{
	out.Tabs.Empty();
	FString Content = Input;
	bool HasUnprocessedTabs = true;
	int ParamEndIndex;
	while (HasUnprocessedTabs)
	{


		if (ParseSettingsTab(Content, out.Tabs.AddDefaulted_GetRef(), ParamEndIndex))
		{
			Content = UKismetStringLibrary::RightChop(Content, ParamEndIndex + 1);
		}
		else
		{
			out.Tabs.Pop();
			HasUnprocessedTabs = false;
		}


	}
	return(true);
}

bool USettingsParserLib::ParseSettingsTab(FString& Input, FSettingsTab& out, int& EndIndex)
{
	bool HasUnprocessedSections = true;


	int TabIdentifierPos = UKismetStringLibrary::FindSubstring(Input, "TAB");
	if (TabIdentifierPos == -1)
	{
		return(false);
	}
	int NameOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "(", false, false, TabIdentifierPos + 2 /*number of letters in IDENTIFIER - 1 ie. (TAB)has 3 letters -1 = 2 */);
	if (NameOpeningBracket == -1)
	{
		return (false);
	}
	int NameClosingBracket;
	out.TabName = GetBracketContentsAndIndex(Input, NameOpeningBracket, "(", ")", NameClosingBracket);
	int ContentOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "{", false, false, NameClosingBracket - 1);
	FString Content = GetBracketContentsAndIndex(Input, ContentOpeningBracket, "{", "}", EndIndex);

	int ParamEndIndex;
	while (HasUnprocessedSections)
	{

		
		if (ParseSettingsSection(Content, out.sections.AddDefaulted_GetRef(), ParamEndIndex))
		{
			Content = UKismetStringLibrary::RightChop(Content, ParamEndIndex + 1);
		}
		else
		{
			out.sections.Pop();
			HasUnprocessedSections = false;
		}


	}
	return(true);
}


bool USettingsParserLib::ParseSettingsSection(FString& Input, FSettingsSection& out, int& EndIndex)
{
	bool HasUnprocessedParams = true;

	int SectionIdentifierPos = UKismetStringLibrary::FindSubstring(Input, "SECTION");
	if (SectionIdentifierPos == -1)
	{
		return(false);
	}
	int NameOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "(", false, false, SectionIdentifierPos + 6);
	if (NameOpeningBracket == -1)
	{
		return (false);
	}
	int NameClosingBracket;
	out.SectionName = GetBracketContentsAndIndex(Input, NameOpeningBracket, "(", ")", NameClosingBracket);
	int ContentOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "{", false, false, NameClosingBracket - 1);
	FString Content = GetBracketContentsAndIndex(Input, ContentOpeningBracket, "{", "}", EndIndex);

	int ParamEndIndex;
	while (HasUnprocessedParams)
	{
		
		
		if (ParseSettingsParam(Content, out.Parameters.AddDefaulted_GetRef(),ParamEndIndex))
		{
			Content = UKismetStringLibrary::RightChop(Content, ParamEndIndex + 1);
		}
		else
		{
			out.Parameters.Pop();
			HasUnprocessedParams = false;
		}
		
		
	}
	return(true);
}


bool USettingsParserLib::ParseSettingsParam(FString& Input, FSettingsParam& out, int& EndIndex)
{
	//find location of "PARAM" identifier
	if (UKismetStringLibrary::FindSubstring(Input, "PARAM") == -1)
	{
		return(false);
	}
	int NameOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "(");
	if (NameOpeningBracket == -1)
	{
		return(false);
	}
	int NameClosingBracket;
	out.ParamName = GetBracketContentsAndIndex(Input, NameOpeningBracket, "(", ")",NameClosingBracket);
	int ContentOpeningBracket = UKismetStringLibrary::FindSubstring(Input, "{", false, false, NameClosingBracket - 1);

	//parse statements into an array containing ie. (type=int, name=test)
	TArray<FString> statements = UKismetStringLibrary::ParseIntoArray(GetBracketContentsAndIndex(Input,ContentOpeningBracket,"{","}",EndIndex), ";", true);

	//declate variables for loop 
	int32 Length = statements.Num();
	FString key;
	FString value;
	FString pair;

	//loop through statements
	for (int32 i = 0; i < Length; i++)
	{
		//set pair to current array elementb
		pair = statements[i];

		//get and trim both key and value
		key = UKismetStringLibrary::Trim(UKismetStringLibrary::TrimTrailing(UKismetStringLibrary::LeftChop(pair, pair.Len() - UKismetStringLibrary::FindSubstring(pair, "="))));
		value = UKismetStringLibrary::Trim(UKismetStringLibrary::TrimTrailing(UKismetStringLibrary::RightChop(pair, UKismetStringLibrary::FindSubstring(pair, "=") + 1)));
		//add to map
		if (!key.IsEmpty())
		{
			out.Parameters.Add(key, value);
		}
		
	}
	return(true);
}