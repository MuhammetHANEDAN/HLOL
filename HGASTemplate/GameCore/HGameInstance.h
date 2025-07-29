// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGameInstance.generated.h"

class UData_Item;
class UData_Hero;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRadiantPlayer = true;

	UPROPERTY()
	FGameplayTag HeroTag = FHGamePlayTags::Get().Heroes_Grux;
	
	void SetIsRadiantPlayer(bool bIsRadiant);

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY(EditDefaultsOnly,Category="AProperty")
	TObjectPtr<UData_Hero> HeroAssetData;

	UPROPERTY(EditDefaultsOnly,Category="AProperty")
	TObjectPtr<UData_Item> ItemAssetData;
	
	
};
