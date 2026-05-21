// Fill out your copyright notice in the Description page of Project Settings.


#include "SUserInitializeComponent.h"

#include "CommonSessionSubsystem.h"
#include "CommonUserSubsystem.h"
#include "ControlFlow.h"
#include "ControlFlowManager.h"
#include "GameExperienceManagerComponent.h"
#include "Shooter.h"
#include "GameMode/SGameInstance.h"
#include "GameMode/SGameModeBase.h"
#include "GameMode/SGameStateBase.h"
#include "Kismet/GameplayStatics.h"

USUserInitializeComponent::USUserInitializeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USUserInitializeComponent::BeginPlay()
{
	Super::BeginPlay();

	const auto GameState = GetGameStateChecked<ASGameStateBase>();
	GameState->GetExperienceManager()->CallOrRegister_OnExperienceLoaded(
		FOnGameExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void USUserInitializeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool USUserInitializeComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (InitializeFlow.IsValid())
	{
		OutReason = TEXT("用户正在初始化");

		const TOptional<FString> StepDebugName = InitializeFlow->GetCurrentStepDebugName();
		if (StepDebugName.IsSet())
		{
			OutReason = StepDebugName.GetValue();
		}
	}

	return false;
}

void USUserInitializeComponent::OnExperienceLoaded(const UGameExperienceDefinition* Experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("用户初始化流程"))
	                     .QueueStep(TEXT("等待用户初始化"), this, &ThisClass::FlowStep_WaitForUserInitialization)
	                     .QueueStep(TEXT("尝试初始化用户"), this, &ThisClass::FlowStep_TryToInitializeForLocalPlay);

	Flow.ExecuteFlow();
	InitializeFlow = Flow.AsShared();
}

void USUserInitializeComponent::FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow)
{
	UE_LOG(LogShooter, Log, TEXT("[USUserLoginComponent] 用户初始化流程-初始化前的清理"))

	bool bWasHardDisconnect { false };
	const ASGameModeBase* GameMode = GetGameMode<ASGameModeBase>();
	if (ensure(GameMode) && UGameplayStatics::HasOption(GameMode->OptionsString, "closed"))
	{
		bWasHardDisconnect = true;
	}

	const USGameInstance* GameInstance = GetGameInstanceChecked<USGameInstance>();

	// 重置用户状态
	UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
	if (ensure(UserSubsystem) && bWasHardDisconnect)
	{
		UserSubsystem->ResetUserState();
	}

	// 清空 Session 状态
	SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
	if (ensure(SessionSubsystem))
	{
		SessionSubsystem->CleanUpSessions();
	}

	SubFlow->ContinueFlow();
}

void USUserInitializeComponent::FlowStep_TryToInitializeForLocalPlay(FControlFlowNodeRef SubFlow)
{
	UE_LOG(LogShooter, Log, TEXT("[USUserLoginComponent] 用户初始化流程-用户初始化"))

	InProgressTryToInitializeForLocalPlay = SubFlow;
	UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &ThisClass::OnUserInitializeComplete);
	UserSubsystem->TryToInitializeForLocalPlay(0, {}, false);
}

void USUserInitializeComponent::OnUserInitializeComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	if (InProgressTryToInitializeForLocalPlay.IsValid())
	{
		const FControlFlowNodePtr Flow = InProgressTryToInitializeForLocalPlay;

		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &ThisClass::OnUserInitializeComplete);
		InProgressTryToInitializeForLocalPlay.Reset();

		Flow->ContinueFlow();
	}
}
