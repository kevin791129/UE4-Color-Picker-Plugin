// Copyright kevin791129

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Layout/Margin.h"
#include "ColorPickerWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickerColorChanged, const FLinearColor&, Color);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickerColorChangeBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickerColorChangeEnd);

/**
 * Color picker widget comprised of a hue picker and a saturation and value picker.
 */
UCLASS()
class COLORPICKER_API UColorPickerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Set the current color of color picker and move indicators accordlingly. 
	 * 
	 * @param NewColor : New color to be displayed by color picker.
	 * @param bBroadcastChange : Whether to broadcast 'OnPickerColorChanged' delegate, default not to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Color Picker Widget")
		void SetPickerColor(FLinearColor NewColor, bool bBroadcastChange = false);

	/**
	 * Get the color currently displayed by color picker.
	 *
	 * @return Current color.
	 */
	UFUNCTION(BlueprintCallable, Category = "Color Picker Widget")
		const FLinearColor& GetPickerColor() const { return CurrentColor; }

protected:
	//~ Begin UUserWidget Function Override
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;

	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	//~ End UUserWidget Function Override

private:
#pragma region Helper Function
	/**
	 * Set hue indicator position also updates CurrentHue value.
	 * 
	 * @param Position : New indicator position, clamped to stay within hue picker canvas.
	 */
	UFUNCTION()
		void SetHueIndicatorPosition(const FVector2D& Position);

	/**
	 * Set saturation value indicator position also updates CurrentSaturation and CurrentValue values.
	 *
	 * @param Position : New indicator position, clamped to stay within saturation value picker canvas.
	 */
	UFUNCTION()
		void SetSaturationValueIndicatorPosition(const FVector2D& Position);

	/**
	 * Update the saturation value indicator (if using default) with current color.
	 */
	UFUNCTION()
		void UpdateSaturationValueIndicator();
#pragma endregion

public:
	/** When user interacts with picker and selects new color. */
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Color Changed"))
		FOnPickerColorChanged ColorChangeDelegate;
	/** When user starts interacting with picker. */
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Color Change Start"))
		FOnPickerColorChangeBegin ColorChangeBeginDelegate;
	/** When user finishes interacting with picker. */
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Color Change Finish"))
		FOnPickerColorChangeEnd ColorChangeEndDelegate;

protected:
	/**
	 * UI elements.
	 */
	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* HuePicker;
	UPROPERTY(meta = (BindWidget))
		UCanvasPanel* SaturationValuePicker;
	UPROPERTY(meta = (BindWidget))
		UImage* ColorPicker_H;
	UPROPERTY(meta = (BindWidget))
		UImage* ColorPicker_SV;
	UPROPERTY(meta = (BindWidget))
		UImage* Indicator_H;
	UPROPERTY(meta = (BindWidget))
		UImage* Indicator_SV;

#pragma region Picker Settings
	/** Hue picker position X. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Layout", meta = (DisplayName = "Position X"))
		float H_PositionX = 420.f;
	/** Hue picker position Y. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Layout", meta = (DisplayName = "Position Y"))
		float H_PositionY = 0.f;
	/** Hue picker size X. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Layout", meta = (ClampMin = 0.0f, DisplayName = "Size X"))
		float H_SizeX = 30.f;
	/** Hue picker size Y. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Layout", meta = (ClampMin = 0.0f, DisplayName = "Size Y"))
		float H_SizeY = 300.f;

	/** Saturation and value picker position X. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Layout", meta = (DisplayName = "Position X"))
		float SV_PositionX = 0.f;
	/** Saturation and value picker position Y. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Layout", meta = (DisplayName = "Position Y"))
		float SV_PositionY = 0.f;
	/** Saturation and value picker size X. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Layout", meta = (ClampMin = 0.0f, DisplayName = "Size X"))
		float SV_SizeX = 400.f;
	/** Saturation and value picker size Y. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Layout", meta = (ClampMin = 0.0f, DisplayName = "Size Y"))
		float SV_SizeY = 300.f;

	/** Hue slider indicator icon, can be a UTexture or UMaterialInterface or an object implementing the AtlasedTextureInterface. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Indicator", meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "Inicator Icon", AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
		UObject* HueIndicatorResourceObject;
	/** Hue indicator size if values are valid. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Hue|Indicator", meta = (DisplayName = "Inicator Size"))
		FVector2D HueIndicatorSize = FVector2D(30.f, 20.f);

	/** Saturation and value indicator icon, can be a UTexture or UMaterialInterface or an object implementing the AtlasedTextureInterface. Border settings only works if empty (not set). */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Indicator", meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true", DisplayName = "Inicator Icon", AllowedClasses = "Texture,MaterialInterface,SlateTextureAtlasInterface", DisallowedClasses = "MediaTexture"))
		UObject* SaturationValueIndicatorResourceObject;
	/** Saturation and value indicator size if values are valid. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Indicator", meta = (DisplayName = "Inicator Size"))
		FVector2D SaturationValueIndicatorSize = FVector2D(18.f);
	/** Color indicator border thickness as percentage of indicator radius. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Indicator", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float BorderSize = 0.1f;
	/** Use set indicator border color or allow border color to change dynamically for contrast. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Indicator")
		bool bUseBorderColor = true;
	/** Indicator border color. */
	UPROPERTY(EditInstanceOnly, Category = "Color Picker Widget|Satuation Value|Indicator")
		FLinearColor BorderColor = FLinearColor::White;
#pragma endregion

	/** Color picker current displayed color. */
	UPROPERTY(BlueprintReadOnly, Category = "Color Picker Widget|Color")
		FLinearColor CurrentColor;
	/** Current hue value at indicator position. */
	UPROPERTY(BlueprintReadOnly, Category = "Color Picker Widget|Color")
		float CurrentHue;
	/** Current hue value at indicator position. */
	UPROPERTY(BlueprintReadOnly, Category = "Color Picker Widget|Color")
		float CurrentSaturation;
	/** Current hue value at indicator position. */
	UPROPERTY(BlueprintReadOnly, Category = "Color Picker Widget|Color")
		float CurrentValue;

private:
	/** If user is interacting with color picker. */
	UPROPERTY()
		bool bIsInteracting;
	/** True if user is interacting with hue picker, false with saturation and value picker. */
	UPROPERTY()
		bool bHueInteraction;

	/** Saturation and value image material to create dynamic material. */
	UPROPERTY()
		UMaterialInstance* SVMat;
	/** Saturation and value image dynamic material updated whenever hue changes. */
	UPROPERTY()
		UMaterialInstanceDynamic* SVMatDynamic;

	/** Saturation and value indicator material to create dynamic material. */
	UPROPERTY()
		UMaterialInstance* SVIndicatorMat;
	/** Saturation and value indicator dynamic material updated whenever saturation and value changes. */
	UPROPERTY()
		UMaterialInstanceDynamic* SVIndicatorMatDynamic;
	/** If using default saturation and value indicator. */
	UPROPERTY()
		bool bUsingDefaultIndicator;
};
