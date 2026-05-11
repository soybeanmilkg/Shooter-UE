// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, Shooter, "Shooter");

DEFINE_LOG_CATEGORY(LogShooter)

namespace Shooter
{
	FString GetNetModeString(UObject* ContextObject)
	{
		ENetMode Mode = NM_MAX;

		if (const AActor* Actor = Cast<AActor>(ContextObject))
		{
			Mode = Actor->GetNetMode();
		}
		else if (const UActorComponent* Component = Cast<UActorComponent>(ContextObject))
		{
			Mode = Component->GetNetMode();
		}

		switch (Mode)
		{
			case NM_Standalone: return "Standalone";
			case NM_DedicatedServer: return "DedicatedServer";
			case NM_ListenServer: return "ListenServer";
			case NM_Client: return "Client";
			case NM_MAX: return "MAX";
		}

		return "Unknown";
	}

	FString GetNetRoleString(UObject* ContextObject)
	{
		ENetRole Role = ROLE_None;

		if (const AActor* Actor = Cast<AActor>(ContextObject))
		{
			Role = Actor->GetLocalRole();
		}
		else if (const UActorComponent* Component = Cast<UActorComponent>(ContextObject))
		{
			Role = Component->GetOwnerRole();
		}

		return StaticEnum<ENetRole>()->GetDisplayNameTextByValue(Role).ToString();
	}
}
