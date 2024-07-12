#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8 {
    Left UMETA(DisplayName = "Turning Left"),
    Right UMETA(DisplayName = "Turning Right"),
    NotTurning UMETA(DisplayName = "Not Turning"),

    Max UMETA(DisplayName = "DefaultMax"),
};
