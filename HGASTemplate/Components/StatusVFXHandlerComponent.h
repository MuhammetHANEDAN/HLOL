// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusVFXHandlerComponent.generated.h"


class AHBaseCharacter;
class UAbilitySystemComponent;
class UHAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class HGASTEMPLATE_API UStatusVFXHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusVFXHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
	TObjectPtr<UHAbilitySystemComponent> HASC;
	void SetHASC(UAbilitySystemComponent* InHASC);

	void BindTagDelegates();

	UPROPERTY()
	TWeakObjectPtr<AHBaseCharacter> OwnerHBaseCharacter;

	void InitStatusVFXHandler(UAbilitySystemComponent* ASC);

	// Kuleler için devre dışı olacak
	bool bIsEffectAble = true;
		
};
