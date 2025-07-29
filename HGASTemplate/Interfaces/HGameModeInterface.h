// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HGASTEMPLATE_API IHGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void I_KillEvent(AActor* KillerActor, AActor* KilledActor) = 0;

	virtual float I_GetGameTime() = 0;

	

	
};
