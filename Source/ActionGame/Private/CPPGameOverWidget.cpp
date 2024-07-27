#include "CPPGameOverWidget.h"

#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UCPPGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Restart->OnClicked.AddUniqueDynamic(this, &UCPPGameOverWidget::OnRestartClicked);
    Title->OnClicked.AddUniqueDynamic(this, &UCPPGameOverWidget::OnTitleClicked);
}

void UCPPGameOverWidget::OnRestartClicked()
{
    if (ReloadLevel.IsNull()) return;
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, ReloadLevel);
}

void UCPPGameOverWidget::OnTitleClicked()
{
    if (MainManu.IsNull()) return;
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainManu);
}