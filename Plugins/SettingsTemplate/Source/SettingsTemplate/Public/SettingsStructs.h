// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SettingsStructs.generated.h"


USTRUCT(BlueprintType)
struct FSettingsParam
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ParamName = "None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString,FString> Parameters;
};


USTRUCT(BlueprintType)
struct FSettingsSection
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString SectionName = "None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSettingsParam> Parameters;

};


USTRUCT(BlueprintType)
struct FSettingsTab
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TabName = "None";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSettingsSection> sections;

};

USTRUCT(BlueprintType)
struct FSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSettingsTab> Tabs;
};