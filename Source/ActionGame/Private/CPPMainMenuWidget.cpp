#include "CPPMainMenuWidget.h"

#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UCPPMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Play->OnClicked.AddUniqueDynamic(this, &UCPPMainMenuWidget::OnPlayClicked);
}

void UCPPMainMenuWidget::OnPlayClicked()
{
    if (LoadLevel.IsNull()) return;
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, LoadLevel);
}