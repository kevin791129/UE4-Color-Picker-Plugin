// Copyright kevin791129

#include "UMG/ColorPickerWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "ColorPickerBPLibrary.h"

#pragma region Initialize
void UColorPickerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bool bResourceFound = false;

	SVMat = FindObject<UMaterialInstance>(ANY_PACKAGE, TEXT("MaterialInstanceConstant'/ColorPicker/Material/MI_SV_Picker.MI_SV_Picker'"));
	if (SVMat)
	{
		SVMatDynamic = UMaterialInstanceDynamic::Create(SVMat, this);
		if (SVMatDynamic)
		{
			ColorPicker_SV->SetBrushFromMaterial(SVMatDynamic);
			bResourceFound = true;
		}
	}

	// Saturation and value material missing.
	if (!bResourceFound)
	{
		UE_LOG(LogColorPickerError, Error, TEXT("Saturation and value material not found in plugin."));
	}

	// Only if using default saturation and value indicator.
	if (!SaturationValueIndicatorResourceObject)
	{
		bResourceFound = false;

		SVIndicatorMat = FindObject<UMaterialInstance>(ANY_PACKAGE, TEXT("MaterialInstanceConstant'/ColorPicker/Material/MI_SV_Indicator.MI_SV_Indicator'"));
		if (SVIndicatorMat)
		{
			SVIndicatorMatDynamic = UMaterialInstanceDynamic::Create(SVIndicatorMat, this);
			if (SVIndicatorMatDynamic)
			{
				Indicator_SV->SetBrushFromMaterial(SVIndicatorMatDynamic);
				bUsingDefaultIndicator = true;
				bResourceFound = true;

				SVIndicatorMatDynamic->SetVectorParameterValue("BorderColor", BorderColor);
				SVIndicatorMatDynamic->SetScalarParameterValue("BorderSize", BorderSize);
				SVIndicatorMatDynamic->SetScalarParameterValue("bUseBorderColor", bUseBorderColor ? 1.f : 0.f);
			}
		}

		// Saturation and value indicator material missing.
		if (!bResourceFound)
		{
			UE_LOG(LogColorPickerError, Error, TEXT("Saturation and value indicator material not found in plugin."));
		}
	}
}

void UColorPickerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UCanvasPanelSlot* HuePickerSlot = Cast<UCanvasPanelSlot>(HuePicker->Slot);
	if (HuePickerSlot)
	{
		HuePickerSlot->SetPosition(FVector2D(H_PositionX, H_PositionY));
		HuePickerSlot->SetSize(FVector2D(H_SizeX, H_SizeY));
	}

	UCanvasPanelSlot* SaturationValuePickerSlot = Cast<UCanvasPanelSlot>(SaturationValuePicker->Slot);
	if (SaturationValuePickerSlot)
	{
		SaturationValuePickerSlot->SetPosition(FVector2D(SV_PositionX, SV_PositionY));
		SaturationValuePickerSlot->SetSize(FVector2D(SV_SizeX, SV_SizeY));
	}

	UCanvasPanelSlot* HueIndicatorSlot = Cast<UCanvasPanelSlot>(Indicator_H->Slot);
	if (HueIndicatorSlot && HueIndicatorSize.X > 0.f && HueIndicatorSize.Y > 0.f)
	{
		HueIndicatorSlot->SetSize(HueIndicatorSize);
	}
	if (HueIndicatorResourceObject)
		Indicator_H->SetBrushResourceObject(HueIndicatorResourceObject);

	UCanvasPanelSlot* SaturationValueIndicatorSlot = Cast<UCanvasPanelSlot>(Indicator_SV->Slot);
	if (SaturationValueIndicatorSlot && SaturationValueIndicatorSize.X > 0.f && SaturationValueIndicatorSize.Y > 0.f)
	{
		SaturationValueIndicatorSlot->SetSize(SaturationValueIndicatorSize);
	}
	if (SaturationValueIndicatorResourceObject)
		Indicator_SV->SetBrushResourceObject(SaturationValueIndicatorResourceObject);
}

void UColorPickerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
#pragma endregion

#pragma region Input
FReply UColorPickerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (ColorPicker_H->IsHovered())
	{
		bIsInteracting = true;
		bHueInteraction = true;
		Reply.CaptureMouse(TakeWidget()).LockMouseToWidget(ColorPicker_H->TakeWidget());
	}
	else if (ColorPicker_SV->IsHovered())
	{
		bIsInteracting = true;
		bHueInteraction = false;
		Reply.CaptureMouse(TakeWidget()).LockMouseToWidget(ColorPicker_SV->TakeWidget());
	}
	
	if (bIsInteracting)
	{
		// Notify listeners that user started changing color.
		if (ColorChangeBeginDelegate.IsBound())
			ColorChangeBeginDelegate.Broadcast();

		// Change current color by using 'NativeOnMouseMove'.
		NativeOnMouseMove(InGeometry, InMouseEvent);
	}

	return Reply;
}

FReply UColorPickerWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Notify listeners that user finished changing color.
	if (bIsInteracting && ColorChangeEndDelegate.IsBound())
		ColorChangeEndDelegate.Broadcast();

	bIsInteracting = false;

	return FReply::Handled().ReleaseMouseCapture().ReleaseMouseLock();
}

FReply UColorPickerWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bColorChanged = false;

	if (bIsInteracting)
	{
		FVector2D MouseLocationPosition = FVector2D::ZeroVector;
		if (bHueInteraction /*&& ColorPicker_H->GetCachedGeometry().IsUnderLocation(InMouseEvent.GetScreenSpacePosition())*/)
		{
			MouseLocationPosition = ColorPicker_H->GetCachedGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
			SetHueIndicatorPosition(FVector2D(0.f, MouseLocationPosition.Y));
			bColorChanged = true;
		}
		else if(!bHueInteraction /*&& ColorPicker_SV->GetCachedGeometry().IsUnderLocation(InMouseEvent.GetScreenSpacePosition())*/)
		{
			MouseLocationPosition = ColorPicker_SV->GetCachedGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
			SetSaturationValueIndicatorPosition(MouseLocationPosition);
			bColorChanged = true;
		}
	}

	// Update current color and notify listeners of the change.
	if (bColorChanged)
	{
		UColorPickerBPLibrary::HSVToLinearColor(CurrentHue, CurrentSaturation, CurrentValue, CurrentColor);
		UpdateSaturationValueIndicator();

		if (ColorChangeDelegate.IsBound())
			ColorChangeDelegate.Broadcast(CurrentColor);
	}

	return FReply::Handled();
}

FCursorReply UColorPickerWidget::NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent)
{
	if (bIsInteracting)
		return FCursorReply::Cursor(EMouseCursor::Type::None);
	else
		return Super::NativeOnCursorQuery(InGeometry, InCursorEvent);
}

void UColorPickerWidget::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	// Notify listeners that user finished changing color.
	if (bIsInteracting && ColorChangeEndDelegate.IsBound())
		ColorChangeEndDelegate.Broadcast();

	bIsInteracting = false;
}
#pragma endregion

void UColorPickerWidget::SetPickerColor(FLinearColor NewColor, bool bBroadcastChange)
{
	CurrentColor = NewColor;

	float H, S, V;
	UColorPickerBPLibrary::LinearColorToHSV(CurrentColor, H, S, V);
	SetHueIndicatorPosition(FVector2D(0.f, H / 360.f * H_SizeY));
	SetSaturationValueIndicatorPosition(FVector2D(S * SV_SizeX, (1 - V) * SV_SizeY));
	UpdateSaturationValueIndicator();

	if (bBroadcastChange && ColorChangeDelegate.IsBound())
	{
		ColorChangeDelegate.Broadcast(CurrentColor);
	}
}

#pragma region Helper Function
void UColorPickerWidget::SetHueIndicatorPosition(const FVector2D& Position)
{
	const float X = FMath::Clamp(Position.X, 0.f, H_SizeX);
	const float Y = FMath::Clamp(Position.Y, 0.f, H_SizeY);

	UCanvasPanelSlot* HueIndicatorSlot = Cast<UCanvasPanelSlot>(Indicator_H->Slot);
	if (HueIndicatorSlot)
	{
		HueIndicatorSlot->SetPosition(FVector2D(X, Y));
	}

	CurrentHue = Y / H_SizeY * 360.f;

	// Update hue value for the saturation value picker material.
	if (SVMatDynamic)
	{
		SVMatDynamic->SetScalarParameterValue("Hue", Y / H_SizeY);
	}
}

void UColorPickerWidget::SetSaturationValueIndicatorPosition(const FVector2D& Position)
{
	const float X = FMath::Clamp(Position.X, 0.f, SV_SizeX);
	const float Y = FMath::Clamp(Position.Y, 0.f, SV_SizeY);

	UCanvasPanelSlot* SaturationValueIndicatorSlot = Cast<UCanvasPanelSlot>(Indicator_SV->Slot);
	if (SaturationValueIndicatorSlot)
	{
		SaturationValueIndicatorSlot->SetPosition(FVector2D(X, Y));
	}

	CurrentSaturation = X / SV_SizeX;
	CurrentValue = 1 - Y / SV_SizeY;
}

void UColorPickerWidget::UpdateSaturationValueIndicator()
{
	if (!bUsingDefaultIndicator)
		return;

	// Update color for the saturation value indicator material.
	if (SVIndicatorMatDynamic)
	{
		SVIndicatorMatDynamic->SetVectorParameterValue("IndicatorColor", CurrentColor);
	}
}
#pragma endregion