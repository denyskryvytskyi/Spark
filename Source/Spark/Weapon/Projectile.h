// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Projectile.generated.h"

UCLASS()
class SPARK_API AProjectile : public AActor {
    GENERATED_BODY()

public:
    AProjectile();

public:
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    class UBoxComponent* CollisionBox;
};
