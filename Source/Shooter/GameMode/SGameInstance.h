// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API USGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

protected:
	//~ Begin UCommonGameInstance
	virtual void HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;
	//~ End UCommonGameInstance
};
