// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameSettingListEntry.h"
#include "SGameSettingListEntry_EditableText.generated.h"

class USGameSettingValue_String;
class UEditableText;

/**
 * 
 */
UCLASS()
class SHOOTER_API USGameSettingListEntry_EditableText : public UGameSettingListEntry_Setting
{
	GENERATED_BODY()

public:
	virtual void SetSetting(UGameSetting* InSetting) override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnEntryReleased() override;
	virtual void OnSettingChanged() override;

	UFUNCTION()
	void HandleTextChanged(const FText& Text);

	UFUNCTION()
	void HandleTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	UPROPERTY(Transient)
	TObjectPtr<USGameSettingValue_String> StringSetting { nullptr };

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, BlueprintProtected=true, AllowPrivateAccess=true))
	TObjectPtr<UEditableText> Input_Text { nullptr };

	void Refresh();
};
