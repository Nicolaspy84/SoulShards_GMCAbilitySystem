﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attributes.h"
#include "GMCAbility.h"
#include "GMCAbilityEffect.h"
#include "GMCMovementUtilityComponent.h"
#include "Components/ActorComponent.h"
#include "GMCAbilityComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GMCABILITYSYSTEM_API UGMC_AbilityComponent : public UGMC_MovementUtilityCmp
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGMC_AbilityComponent();

	bool TryActivateAbility(FGMCAbilityData AbilityData);

	// Queue an ability to be executed
	void QueueAbility(FGMCAbilityData AbilityData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGMCAttributeSet Attributes;

	UFUNCTION(BlueprintCallable)
	void ApplyAbilityCost(UGMCAbility* Ability);

	UFUNCTION(BlueprintCallable)
	void ApplyAbilityEffect(TSubclassOf<UGMCAbilityEffect> Effect);

	UPROPERTY(BlueprintReadWrite)
	bool bJustTeleported;

protected:
	virtual void BindReplicationData_Implementation() override;
	virtual void GenPredictionTick_Implementation(float DeltaTime) override;
	virtual void GenSimulationTick_Implementation(float DeltaTime) override;
	virtual void PreLocalMoveExecution_Implementation(const FGMC_Move& LocalMove) override;

	bool CanAffordAbilityCost(UGMCAbility* Ability);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	TArray<TSubclassOf<UGMCAbility>> GrantedAbilities;

private:
	TArray<FGMCAbilityData> QueuedAbilities;

	// Current Ability Data being processed
	// Members of this struct are bound over GMC
	FGMCAbilityData AbilityData{};
};