// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "SlateNeuronMenuWidgetStyle.h"


FSlateNeuronMenuStyle::FSlateNeuronMenuStyle()
{
}

FSlateNeuronMenuStyle::~FSlateNeuronMenuStyle()
{
}

const FName FSlateNeuronMenuStyle::TypeName(TEXT("FSlateNeuronMenuStyle"));

const FSlateNeuronMenuStyle& FSlateNeuronMenuStyle::GetDefault()
{
	static FSlateNeuronMenuStyle Default;
	return Default;
}

void FSlateNeuronMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

