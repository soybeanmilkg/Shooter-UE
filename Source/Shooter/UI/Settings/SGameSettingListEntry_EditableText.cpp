// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingListEntry_EditableText.h"

#include "Components/EditableText.h"
#include "Settings/CustomSettings/SGameSettingValue_String.h"

void USGameSettingListEntry_EditableText::SetSetting(UGameSetting* InSetting)
{
	Super::SetSetting(InSetting);

	StringSetting = Cast<USGameSettingValue_String>(InSetting);

	Refresh();
}

void USGameSettingListEntry_EditableText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Input_Text->OnTextChanged.AddDynamic(this, &ThisClass::HandleTextChanged);
	Input_Text->OnTextCommitted.AddDynamic(this, &ThisClass::HandleTextCommitted);
}

void USGameSettingListEntry_EditableText::NativeOnEntryReleased()
{
	Super::NativeOnEntryReleased();

	StringSetting = nullptr;
}

void USGameSettingListEntry_EditableText::OnSettingChanged()
{
	Refresh();
}

void USGameSettingListEntry_EditableText::HandleTextChanged(const FText& Text)
{
	TGuardValue Suspend(bSuspendChangeUpdates, true);

	if (ensure(StringSetting))
	{
		StringSetting->SetValue(Text.ToString());
	}
}

void USGameSettingListEntry_EditableText::HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (Text.IsEmpty())
	{
		StringSetting->RestoreToInitial();
	}
}

void USGameSettingListEntry_EditableText::Refresh()
{
	if (ensure(StringSetting))
	{
		Input_Text->SetText(FText::FromString(StringSetting->GetValue()));
	}
}
