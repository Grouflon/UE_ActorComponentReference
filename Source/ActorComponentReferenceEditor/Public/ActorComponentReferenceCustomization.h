// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

struct FActorComponentReference;
class IPropertyUtilities;
class UActorComponent;

#include "CoreMinimal.h"

#include <IPropertyTypeCustomization.h>

/**
 * 
 */

class FActorComponentReferenceCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	FActorComponentReferenceCustomization();
	virtual ~FActorComponentReferenceCustomization();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> _structPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& _structCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> _structPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& _structCustomizationUtils) override;

private:
	void _OnActorChanged();
	void _OnSelectionChanged(UObject* _object);

	AActor* m_selectedActor = nullptr;
	FActorComponentReference* m_currentComponentReference = nullptr;
	TArray<UActorComponent*> m_referenceActorComponents;
	TSharedPtr<IPropertyUtilities> m_propertyUtilities;
};
