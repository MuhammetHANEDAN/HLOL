// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobaCameraActor.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class HGASTEMPLATE_API AMobaCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMobaCameraActor();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> ArrowForward;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> ArrowRight;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	

};
