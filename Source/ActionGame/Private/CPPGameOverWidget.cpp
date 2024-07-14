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
    UGameplayStatics::OpenLevel(this, FName(TEXT("MoveUp")));
}

void UCPPGameOverWidget::OnTitleClicked()
{
    UGameplayStatics::OpenLevel(this, FName(TEXT("MainMenu")));
}