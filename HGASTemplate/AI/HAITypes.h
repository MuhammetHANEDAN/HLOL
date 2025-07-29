#pragma once


#include "CoreMinimal.h"
#include "HAITypes.generated.h"

UENUM(BlueprintType,Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAIStatesEnum : uint8
{
	
	EA_None             UMETA(DisplayName = "None"), //0
	EA_GoingForward     UMETA(DisplayName = "GoingForward"), //1
	EA_Chase			UMETA(DisplayName = "Chase"), //2
	EA_Attack			UMETA(DisplayName = "Attack"),//3
	EA_ReturnToLine		UMETA(DisplayName = "ReturnToLine"),//4
	EA_ReturnToStartLoc	UMETA(DisplayName = "ReturnToStartLoc"),//5
	EA_Idle             UMETA(DisplayName = "Idle"),//6
	EA_MinionTaunt      UMETA(DisplayName = "MinionTaunt")//7

};

/**
 * BB
 
	EndTargetLocation        FName("EndTargetLocation") vector
	MiddleSpline             FName("MiddleSpline") actor
	StateEnum				 FName("StateEnum") enum
	TargetActor				 FName("TargetActor") actor
	AttackRange              FName("AttackRange") float
	ChaseDistance			 FName("ChaseDistance") float
	DistanceToTarget		 FName("DistanceToTarget") float
	AttackTetherDistance	 FName("AttackTetherDistance") float
	DetectEnemyRange		 FName("DetectEnemyRange") float
	StartLocation			 FName("StartLocation") vector
	StartRotation			 FName("StartRotation") rotator
  
 */
