// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponentReferenceCustomization.h"

#include <Engine/Selection.h>
#include <IDetailChildrenBuilder.h>
#include <DetailWidgetRow.h>
#include <IDetailPropertyRow.h>
#include <IPropertyUtilities.h>
#include <Widgets/Input/SComboButton.h>
#include <Widgets/Views/SListView.h>
#include <Widgets/Images/SImage.h>
#include <Styling/SlateIconFinder.h>

#include <ActorComponentReference.h>

#define LOCTEXT_NAMESPACE "ActorComponentReferenceEditorModule"

TSharedRef<IPropertyTypeCustomization> FActorComponentReferenceCustomization::MakeInstance()
{
	return MakeShareable(new FActorComponentReferenceCustomization);
}

FActorComponentReferenceCustomization::FActorComponentReferenceCustomization()
{
	USelection::SelectionChangedEvent.AddRaw(this, &FActorComponentReferenceCustomization::_OnSelectionChanged);
}

FActorComponentReferenceCustomization::~FActorComponentReferenceCustomization()
{
	USelection::SelectionChangedEvent.RemoveAll(this);
}

void FActorComponentReferenceCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> _structPropertyHandle, class FDetailWidgetRow& _headerRow, IPropertyTypeCustomizationUtils& _structCustomizationUtils)
{
	m_selectedActor = nullptr;
	TArray<UObject*> outerObjects;
	_structPropertyHandle->GetOuterObjects(outerObjects);
	for (UObject* object : outerObjects)
	{
		AActor* actor = Cast<AActor>(object);
		if (actor)
		{
			m_selectedActor = actor;
			break;
		}
	}

	if (!m_selectedActor)
		return;

	m_propertyUtilities = _structCustomizationUtils.GetPropertyUtilities();

	_headerRow
	.NameContent()
	[
		_structPropertyHandle->CreatePropertyNameWidget()
	]							
	.ValueContent()
	[
		_structPropertyHandle->CreatePropertyValueWidget()
	];
}

void FActorComponentReferenceCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> _structPropertyHandle, class IDetailChildrenBuilder& _structBuilder, IPropertyTypeCustomizationUtils& _structCustomizationUtils)
{
	if (!m_selectedActor)
		return;

	FSlateFontInfo fontInfo = _structCustomizationUtils.GetRegularFont();

	m_currentComponentReference = _structPropertyHandle->GetProperty()->ContainerPtrToValuePtr<FActorComponentReference>(m_selectedActor);
	check(m_currentComponentReference);

	uint32 NumChildren;
	_structPropertyHandle->GetNumChildren(NumChildren);

	TSharedPtr<IPropertyHandle> actorHandle = _structPropertyHandle->GetChildHandle("Actor");
	check(actorHandle.IsValid());
	actorHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateRaw(this, &FActorComponentReferenceCustomization::_OnActorChanged));

	_structBuilder.AddProperty(actorHandle.ToSharedRef());

	if (m_currentComponentReference->Actor)
	{
		m_referenceActorComponents.Empty();
		m_currentComponentReference->Actor->GetComponents(m_referenceActorComponents);

		FText selectedComponentName = m_currentComponentReference->Component ? FText::FromString(m_currentComponentReference->Component->GetName()) : LOCTEXT("None", "None");

		_structBuilder.AddCustomRow(LOCTEXT("SearchString","ComponentReference"))
		.NameContent()
		[
			SNew(STextBlock)
			.Font(fontInfo)
			.Text(LOCTEXT("Component", "Component"))
		]
		.ValueContent()
		[
			SNew(SComboButton)
			.ContentPadding(FMargin(0, 0, 5, 0))
			//.ToolTipText(TooltipText)
			.ButtonContent()
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("NoBorder"))
				.Padding(FMargin(0, 0, 5, 0))
				[
					SNew(STextBlock)
					.Font(fontInfo)
					.Text(selectedComponentName)
				]
			]
			.MenuContent()
			[
				SNew(SListView<UActorComponent*>)
				.ListItemsSource(&m_referenceActorComponents)
				.ScrollbarVisibility(EVisibility::Collapsed)
				.OnGenerateRow_Lambda([fontInfo](UActorComponent* InItem, const TSharedRef< class STableViewBase >& Owner)
				{
					// Setup a default icon brush.
					const FSlateBrush* ComponentIcon = FEditorStyle::GetBrush("SCS.NativeComponent");
					ComponentIcon = FSlateIconFinder::FindIconBrushForClass(InItem->GetClass(), TEXT("SCS.Component"));
					const FLinearColor InheritedNativeComponentColor(0.7f, 0.9f, 0.7f);

					return
					SNew(STableRow<UActorComponent*>, Owner)
					.ToolTipText(FText::FromString(InItem->GetClass()->GetName()))
					.Padding(FMargin(2, 2, 2, 2))
					[
						SNew(SHorizontalBox)
						+SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SImage)
							.Image(ComponentIcon)
							.ColorAndOpacity(InheritedNativeComponentColor)
						]
						+ SHorizontalBox::Slot()
						.Padding(FMargin(5, 0, 0, 0))
						[
							SNew(STextBlock)
							.Font(fontInfo)
							.Text(FText::FromString(InItem->GetName()))
						]
					];
				})
				.OnSelectionChanged_Lambda([this](UActorComponent* InComponent, ESelectInfo::Type)
				{
					m_currentComponentReference->Component = InComponent;
					m_propertyUtilities->ForceRefresh();
				})
			]
		];
	}
}

void FActorComponentReferenceCustomization::_OnActorChanged()
{
	// NOTE(Remi|2018/06/07): Stupid checks because of stupid Slate design
	if (!m_selectedActor)
		return;

	if (!m_propertyUtilities.IsValid())
		return;

	if (!m_currentComponentReference)
		return;

	m_currentComponentReference->Component = nullptr;
	m_propertyUtilities->ForceRefresh();
}

void FActorComponentReferenceCustomization::_OnSelectionChanged(UObject* _object)
{
	AActor* actor = Cast<AActor>(_object);
	if (actor != m_selectedActor)
	{
		m_selectedActor = nullptr;
		m_currentComponentReference = nullptr;
		m_propertyUtilities.Reset();
		return;
	}

	UActorComponent* actorComponent = Cast<UActorComponent>(_object);
	if (actorComponent && actorComponent->GetOwner() != m_selectedActor)
	{
		m_selectedActor = nullptr;
		m_currentComponentReference = nullptr;
		m_propertyUtilities.Reset();
		return;
	}
}

#undef LOCTEXT_NAMESPACE
