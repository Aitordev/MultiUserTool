// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "SlateNeuronMenuWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct MULTIUSERTOOL_API FSlateNeuronMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSlateNeuronMenuStyle();
	virtual ~FSlateNeuronMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSlateNeuronMenuStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USlateNeuronMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSlateNeuronMenuStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
