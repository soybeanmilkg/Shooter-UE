// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncAction_CommonUserInitialize.h"
#include "ControlFlowNode.h"
#include "LoadingProcessInterface.h"
#include "Components/GameStateComponent.h"
#include "SUserInitializeComponent.generated.h"

class UCommonSessionSubsystem;
class UCommonUserSubsystem;
class FControlFlow;
class UGameExperienceDefinition;

UCLASS(ClassGroup=(Shooter), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USUserInitializeComponent : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	explicit USUserInitializeComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent

	//~ Begin ILoadingProcessInterface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~ End ILoadingProcessInterface

private:
	void OnExperienceLoaded(const UGameExperienceDefinition* Experience);

	TSharedPtr<FControlFlow> InitializeFlow { nullptr };
	UPROPERTY(Transient)
	TObjectPtr<UCommonUserSubsystem> UserSubsystem { nullptr };
	UPROPERTY(Transient)
	TObjectPtr<UCommonSessionSubsystem> SessionSubsystem { nullptr };
	FControlFlowNodePtr InProgressTryToInitializeForLocalPlay { nullptr };

	void FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow);
	void FlowStep_TryToInitializeForLocalPlay(FControlFlowNodeRef SubFlow);

	UFUNCTION()
	void OnUserInitializeComplete(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);
};
