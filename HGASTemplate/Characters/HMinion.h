// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAI_Character.h"
#include "HMinion.generated.h"

UCLASS()
class HGASTEMPLATE_API AHMinion : public AHAI_Character
{
	GENERATED_BODY()

public:

	AHMinion();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	
	
};
