// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "OverheadWidget.generated.h"

/**
 *
 */
UCLASS()
class SPARK_API UOverheadWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetDisplayText(const FString& Text);

    UFUNCTION(BlueprintCallable)
    void ShowPlayerNetRole(APawn* Pawn);

protected:
    virtual void NativeDestruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DisplayText;
};
