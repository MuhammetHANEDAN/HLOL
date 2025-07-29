// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaCameraActor.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMobaCameraActor::AMobaCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	ArrowForward = CreateDefaultSubobject<UArrowComponent>("ArrowForwardComp");
	ArrowForward->SetupAttachment(GetRootComponent());

	ArrowRight = CreateDefaultSubobject<UArrowComponent>("ArrowRightComp");
	ArrowRight->SetupAttachment(GetRootComponent());

	bFindCameraComponentWhenViewTarget = true;
	
}


void AMobaCameraActor::BeginPlay()
{
	Super::BeginPlay();

	BecomeViewTarget(PC);
	
}


void AMobaCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

