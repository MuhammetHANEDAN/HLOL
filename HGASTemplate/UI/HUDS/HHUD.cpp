
#include "HHUD.h"

#include "Blueprint/UserWidget.h"
#include "HGASTemplate/UI/Widget/HUserWidget.h"
#include "HGASTemplate/UI/WidgetControllers/OverlayWidgetController.h"


void AHHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

UOverlayWidgetController* AHHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies(); // UAtributesetteki attributelerin changed delegatelerini widget delegatelerine bağlıoruz
	}
	return OverlayWidgetController;
}

void AHHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass uninitialized please fill out BP_HHUD"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass uninitialized please fill out BP_HHUD"));
	
	OverlayWidget = CreateWidget<UHUserWidget>(GetWorld(),OverlayWidgetClass);

	FWidgetControllerParams WidgetControllerParams (PC,PS,ASC,AS);
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(OverlayWidgetController); // Bu fonkun içinde bp de firelanan delegate WidgetControllerSet(); var
	OverlayWidgetController->BroadcastInitialValues(); // Widget Controller içindeki delegateler broadcaste başlıyor
	OverlayWidgetController->InitializeUpgradeButtonDelegateMap();
	
	OverlayWidget->AddToViewport();
}
