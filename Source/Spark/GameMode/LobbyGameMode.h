// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
class SPARK_API ALobbyGameMode : public AGameMode {
    GENERATED_BODY()

public:
    virtual void PostLogin(APlayerController* NewPlayer) override;

private:
    void StartTravel();

private:
    UPROPERTY(EditAnywhere, category = Settings)
    int32 MinPlayerAmountToStart { 2 };
};
