// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingsStructs.h"
#include "SettingsParserLib.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSTEMPLATE_API USettingsParserLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Utilities")
	static int FindMatchingBracket(UPARAM(ref) FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket );
	
	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Utilities")
	static FString GetBracketContents(UPARAM(ref) FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket);

	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Utilities")
	static FString GetBracketContentsAndIndex(UPARAM(ref) FString& Input, int FirstBracket, FString OpeningBracket, FString ClosingBracket, int& ReturnClosingBracketIndex);

	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Internal")
	static bool ParseSettings(UPARAM(ref) FString& Input, FSettings& out, int& EndIndex);
	
	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Internal")
	static bool ParseSettingsTab(UPARAM(ref) FString& Input,FSettingsTab& out, int& EndIndex);

	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Internal")
	static bool ParseSettingsSection(UPARAM(ref) FString& Input, FSettingsSection& out, int& EndIndex);

	UFUNCTION(BlueprintCallable, Category = "SettingsParserLib|Internal")
	static bool ParseSettingsParam(UPARAM(ref) FString& Input, FSettingsParam& out, int& EndIndex);
};


