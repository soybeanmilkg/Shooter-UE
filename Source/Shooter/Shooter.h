// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogShooter, Log, All);

namespace Shooter
{
	FString GetNetModeString(UObject* ContextObject);
	
	FString GetNetRoleString(UObject* ContextObject);
}
