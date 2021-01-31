// Copyright kevin791129.

#pragma once

#include "ColorFormat.generated.h"

UENUM(BlueprintType, meta = (DisplayName = "Color Format"))
enum class EColorFormat : uint8
{
	CF_HEX UMETA(DisplayName = "Hex"),
	CF_RBG UMETA(DisplayName = "RGB"),
	CF_HSV UMETA(DisplayName = "HSV (HSB)"),
	CF_CMYK UMETA(DisplayName = "CMYK"),
	CF_HSL UMETA(DisplayName = "HSL")
};


