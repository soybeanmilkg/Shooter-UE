// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingValue.h"
#include "SGameSettingValue_String.generated.h"

class FGameSettingDataSource;

/**
 * 
 */
UCLASS()
class SHOOTER_API USGameSettingValue_String : public UGameSettingValue
{
	GENERATED_BODY()

public:
	//~ Begin UGameSettingValue
	virtual void Startup() override;
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
	virtual FString GetAnalyticsValue() const override;
	//~ End UGameSettingValue

	virtual TOptional<FString> GetDefaultValue() const;
	virtual void SetValue(const FString& Value, EGameSettingChangeReason Reason = EGameSettingChangeReason::Change);
	virtual FString GetValue() const;

	/** UGameSettingValueDiscreteDynamic */
	void SetDynamicGetter(const TSharedRef<FGameSettingDataSource>& InGetter);
	void SetDynamicSetter(const TSharedRef<FGameSettingDataSource>& InSetter);
	void SetDefaultValue(const FString& InValue);

protected:
	virtual void OnInitialized() override;

	void OnDataSourcesReady(const uint8 Flag);

protected:
	TOptional<FString> DefaultValue;
	FString InitialValue { TEXT("") };

	TSharedPtr<FGameSettingDataSource> Getter;
	TSharedPtr<FGameSettingDataSource> Setter;
	uint8 StartupFlags { 0 };
};
