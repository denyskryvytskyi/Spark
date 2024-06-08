// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EMatchType : uint8 {
    FFA
};

inline FString MatchTypeToString(const EMatchType MatchType)
{
    switch (MatchType) {
    case EMatchType::FFA:
        return FString("FFA");
    }

    return "NONE";
}
