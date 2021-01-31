// Copyright kevin791129

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorPickerBPLibrary.generated.h"

UCLASS()
class UColorPickerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

#pragma region Color Conversion
	/**
	 * Converts linear color to hex color.
	 *
	 * @param Color : Convert color.
	 * @param[out] OutHex : Hex color, format #RRGGBB. 
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Linear Color to Hex", Keywords = "Color Conversion LinearColor Hex"), Category = "Color Picker|Conversion")
		static void LinearColorToHex(const FLinearColor& Color, FString& OutHex);
		
	/**
	 * Converts linear color to RGB color.
	 *
	 * @param Color : Convert color.
	 * @param[out] OutR : Red, range: [0, 255].
	 * @param[out] OutG : Green, range: [0, 255].
	 * @param[out] OutB : Blue, range: [0, 255].
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Linear Color to RGB", Keywords = "Color Conversion LinearColor RGB"), Category = "Color Picker|Conversion")
		static void LinearColorToRGB(const FLinearColor& Color, int& OutR, int& OutG, int& OutB);

	/**
	 * Converts linear color to HSV (HSB) color.
	 *
	 * @param Color : Convert color.
	 * @param[out] OutH : Hue, range: [0.0f, 360.0f).
	 * @param[out] OutS : Saturation, range: [0.0f, 1.0f].
	 * @param[out] OutV : Value, range: [0.0f, 1.0f].
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Linear Color to HSV", Keywords = "Color Conversion LinearColor HSV"), Category = "Color Picker|Conversion")
		static void LinearColorToHSV(const FLinearColor& Color, float& OutH, float& OutS, float& OutV);

	/**
	 * Converts linear color to CMYK color.
	 *
	 * @param Color : Convert color.
	 * @param[out] OutC : Cyan, range: [0.0f, 1.0f].
	 * @param[out] OutM : Magenta, range: [0.0f, 1.0f].
	 * @param[out] OutY : Yellow, range: [0.0f, 1.0f].
	 * @param[out] OutK : Black, range: [0.0f, 1.0f].
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Linear Color to CMYK", Keywords = "Color Conversion LinearColor CMYK"), Category = "Color Picker|Conversion")
		static void LinearColorToCMYK(const FLinearColor& Color, float& OutC, float& OutM, float& OutY, float& OutK);

	/**
	 * Converts linear color to HSL color.
	 *
	 * @param Color : Convert color.
	 * @param[out] OutH : Hue, range: [0.0f, 360.0f).
	 * @param[out] OutS : Saturation, range: [0.0f, 1.0f].
	 * @param[out] OutL : Lightness, range: [0.0f, 1.0f].
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Linear Color to HSL", Keywords = "Color Conversion LinearColor HSL"), Category = "Color Picker|Conversion")
		static void LinearColorToHSL(const FLinearColor& Color, float& OutH, float& OutS, float& OutL);

	/**
	 * Converts hex color to linear color.
	 *
	 * @param Hex : Hex color, supported formats: RGB, RRGGBB, RRGGBBAA, #RGB, #RRGGBB, #RRGGBBAA.
	 * @param[out] OutColor : Linear color.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hex to Linear Color", Keywords = "Color Conversion LinearColor Hex"), Category = "Color Picker|Conversion")
		static void HexToLinearColor(const FString& Hex, FLinearColor& OutColor);

	/**
	 * Converts RGB color to linear color. Invalid input values will be clamped.
	 *
	 * @param R : Red, range: [0, 255].
	 * @param G : Green, range: [0, 255].
	 * @param B : Blue, range: [0, 255].
	 * @param[out] OutColor : Linear color with alpha 1.0f.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "RGB to Linear Color", Keywords = "Color Conversion LinearColor RGB"), Category = "Color Picker|Conversion")
		static void RGBToLinearColor(const int R, const int G, const int B, FLinearColor& OutColor);

	/**
	 * Converts HSV (HSB) color to linear color. Invalid input values will be clamped.
	 *
	 * @param H : Hue, range: [0.0f, 360.0f).
	 * @param S : Saturation, range: [0.0f, 1.0f].
	 * @param V : Value, range: [0.0f, 1.0f].
	 * @param[out] OutColor : Linear color with alpha 1.0f.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "HSV to Linear Color", Keywords = "Color Conversion LinearColor HSV"), Category = "Color Picker|Conversion")
		static void HSVToLinearColor(const float H, const float S, const float V, FLinearColor& OutColor);

	/**
	 * Converts CMYK color to linear color.
	 *
	 * @param C : Cyan, range: [0.0f, 1.0f].
	 * @param M : Magenta, range: [0.0f, 1.0f].
	 * @param Y : Yellow, range: [0.0f, 1.0f].
	 * @param K : Black, range: [0.0f, 1.0f].
	 * @param[out] OutColor : Linear color with alpha 1.0f.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CMYK to Linear Color", Keywords = "Color Conversion LinearColor CMYK"), Category = "Color Picker|Conversion")
		static void CMYKToLinearColor(const float C, const float M, const float Y, const float K, FLinearColor& OutColor);

	/**
	 * Converts HSL color to linear color. Invalid input values will be clamped.
	 *
	 * @param H : Hue, range: [0.0f, 360.0f).
	 * @param S : Saturation, range: [0.0f, 1.0f].
	 * @param L : Lightness, range: [0.0f, 1.0f].
	 * @param[out] OutColor : Linear color with alpha 1.0f.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "HSL to Linear Color", Keywords = "Color Conversion LinearColor HSL"), Category = "Color Picker|Conversion")
		static void HSLToLinearColor(const float H, const float S, const float L, FLinearColor& OutColor);
#pragma endregion
};
