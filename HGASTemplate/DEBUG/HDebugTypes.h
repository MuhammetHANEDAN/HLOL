#pragma once





#define SCREEN_LOG_DURATION(Text, Duration) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Red, TEXT(Text))

#define HLOG(Text) UE_LOG(LogTemp, Warning, TEXT(Text));
#define HLOGF(Float) UE_LOG(LogTemp,Warning, TEXT(" %f"),Float)


