// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MinionBarComponent.generated.h"


class UMinionBarController;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HGASTEMPLATE_API UMinionBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	
	UMinionBarComponent();

protected:
	
	virtual void BeginPlay() override;

public:

	void InitMinionBarComp(UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UPROPERTY(EditDefaultsOnly, Category="AProps")
	TSubclassOf<UMinionBarController> MinionBarControllerClass;

	UPROPERTY()
	TObjectPtr<UMinionBarController> MinionBarWidgetController;
	
};
