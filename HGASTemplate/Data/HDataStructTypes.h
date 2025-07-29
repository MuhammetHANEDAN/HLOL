#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HDataStructTypes.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FDynamicSkillInfo
{
	GENERATED_BODY()

	FDynamicSkillInfo () {}

	//FDynamicSkillInfo (int32 InLevel, float InCost, float InCD) : CurrentSkillLevel(InLevel), ManaCost(InCost), CoolDown(InCD) {}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CurrentSkillLevel = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ManaCost = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CoolDown = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SkillDescription = FString();
	
};
