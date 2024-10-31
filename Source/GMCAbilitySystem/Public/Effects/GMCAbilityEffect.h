// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GMCAbilitySystem.h"
#include "Attributes/GMCAttributeModifier.h"
#include "GMCAbilityEffect.generated.h"

class UGMC_AbilitySystemComponent;

UENUM(BlueprintType)
enum class EGMASEffectType : uint8
{
	Instant,  // Applies Instantly
	Duration, // Lasts for X time
	Infinite  // Lasts forever
};

UENUM(BlueprintType)
enum class EGMASEffectState : uint8
{
	Initialized,  // Applies Instantly
	Started, // Lasts for X time
	Ended  // Lasts forever
};

UENUM(BlueprintType)
enum class EGMASEffectForceType : uint8
{
	None,  // No force to apply.
	KnockDown, // Apply a knock down effect.
	PushBack  // Apply a push back effect.
};

// Container for exposing the attribute modifier to blueprints
UCLASS()
class GMCABILITYSYSTEM_API UGMCAttributeModifierContainer : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGMCAttributeModifier AttributeModifier;
};

USTRUCT(BlueprintType)
struct FGMCAbilityEffectData
{
	GENERATED_BODY()

	FGMCAbilityEffectData():SourceAbilityComponent(nullptr),
							OwnerAbilityComponent(nullptr),
							EffectID(0),
	                         StartTime(0),
	                         EndTime(0)
	{
	}

	UPROPERTY(BlueprintReadWrite, Category = "GMCAbilitySystem")
	UGMC_AbilitySystemComponent* SourceAbilityComponent;

	UPROPERTY()
	UGMC_AbilitySystemComponent* OwnerAbilityComponent;

	UPROPERTY()
	int EffectID;

	UPROPERTY(BlueprintReadOnly, Category = "GMCAbilitySystem")
	double StartTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "GMCAbilitySystem")
	double EndTime;

	UPROPERTY(BlueprintReadOnly, Category = "GMCAbilitySystem")
	double CurrentDuration{0.f};

	// Instantly applies effect then exits. Will not tick.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GMCAbilitySystem")
	bool bIsInstant = true;

	// Apply an inversed version of the modifiers at effect end
	UPROPERTY()
	bool bNegateEffectAtEnd = false;

	// Delay before the effect starts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	double Delay = 0;

	// How long the effect lasts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	double Duration = 0;

	// How often the periodic effect ticks
	// Suggest keeping this above .01
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GMCAbilitySystem")
	double Period = 0;

	// For Period effects, whether first tick should happen immediately
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GMCAbilitySystem")
	bool bPeriodTickAtStart = false;

	// Time in seconds that the client has to apply itself an external effect before the server will force it. If this time is reach, a rollback is likely to happen.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GMCAbilitySystem")
	float ClientGraceTime = 1.f;
	
	UPROPERTY()
	int LateApplicationID = -1;

	// Tag to identify this effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTag EffectTag;

	/**
	* If this tag is set, the effect will be able to stack and will affect the provided atribute (if the attribute is properly setup).
	* Note that 2 effects with a different EffectTag can share the same EffectStackAttributeTag.
	* Effect stacking works as follows:
	* - whenever a new effect with an EffectStackAttributeTag starts, the corresponding attribute is incremented by 1
	* - whenever an effect with an EffectStackAttributeTag ends and is the last one with the EffectStackAttributeTag, the attribute value is set back to 0
	* - whenever an effect with an EffectStackAttributeTag ends but is not the last one with the EffectStackAttributeTag, nothing happens
	* Note that effect stacking only works if the effect has a duration (ie it's not instant).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTag EffectStackAttributeTag;

	/**
	* Contains any relevant metadata for this effect.
	* This includes data effect persistance, effect type (buff, debuff), and dispelling properties.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCABilitySystem")
	FGameplayTagContainer EffectMetaData;

	/**
	* Used when dispelling effects.
	* If there's at least one tag in here, only events which have at least one of those tags in their GrantedTags will be dispelled.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCABilitySystem")
	FGameplayTagContainer FilterDispelledEffectsWithGrantedTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer GrantedTags;

	// Tags that the owner must have to apply this effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer ApplicationMustHaveTags;

	// Tags that the owner must not have to apply this effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer ApplicationMustNotHaveTags;

	// Tags that the owner must have to apply and maintain this effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer MustHaveTags;

	// Tags that the owner must not have to apply and maintain this effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer MustNotHaveTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer RemovedAbilities;

	// If tag is present, periodic effect will not tick. Duration is not affected.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer PausePeriodicEffect;

	// On activation, will end ability present in this container
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FGameplayTagContainer CancelAbilityOnActivation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	TArray<FGMCAttributeModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	EGMASEffectForceType ForceType = EGMASEffectForceType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	FVector ForceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	float ForceAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GMCAbilitySystem")
	float ThreatAmount;
	
	inline bool operator==(const FGMCAbilityEffectData& Other) const
	{
		//Todo: Fix this
		return StartTime == Other.StartTime && EndTime == Other.EndTime;
	};

	bool IsValid() const
	{
		return GrantedTags != FGameplayTagContainer() || GrantedAbilities != FGameplayTagContainer()
				|| RemovedAbilities != FGameplayTagContainer() || Modifiers.Num() > 0
				|| MustHaveTags != FGameplayTagContainer() || MustNotHaveTags != FGameplayTagContainer()
				|| !EffectMetaData.IsEmpty() || !FilterDispelledEffectsWithGrantedTag.IsEmpty()
				|| ForceType != EGMASEffectForceType::None || ThreatAmount != 0.f;
	}

	FString ToString() const{
		return FString::Printf(TEXT("[id: %d] [Tag: %s] (Duration: %.3lf) (CurrentDuration: %.3lf)"), EffectID, *EffectTag.ToString(), Duration, CurrentDuration);
	}
};

/**
 * 
 */
class UGMC_AbilitySystemComponent;

UCLASS(Blueprintable, BlueprintType)
class GMCABILITYSYSTEM_API UGMCAbilityEffect : public UObject
{
	GENERATED_BODY()

public:
	EGMASEffectState CurrentState;

	UPROPERTY(EditAnywhere, Category = "GMCAbilitySystem")
	FGMCAbilityEffectData EffectData;

	UFUNCTION(BlueprintCallable, Category = "GMCAbilitySystem")
	virtual void InitializeEffect(FGMCAbilityEffectData InitializationData);
	
	virtual void EndEffect();
	virtual void EndEffect_Implementation() {};

	virtual void BeginDestroy() override;
	
	virtual void Tick(float DeltaTime);

	// Return the current duration of the effect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GMAS|Abilities")
	float GetCurrentDuration() const { return EffectData.CurrentDuration; }

	// Return the current duration of the effect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GMAS|Abilities")
	FGMCAbilityEffectData GetEffectData() const { return EffectData; }

	// Return the current duration of the effect
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GMAS|Abilities")
	float GetEffectTotalDuration() const { return EffectData.Duration; }

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="Effect Tick"), Category="GMCAbilitySystem")
	void TickEvent(float DeltaTime);

	// Dynamic Condition allow you to avoid applying the attribute modifier if a condition is not met, for example, a sprint effect with attribute cost when the player is below a certain speed.
	// However, this is not stopping the effect.
	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="Dynamic Condition"), Category="GMCAbilitySystem")
	bool AttributeDynamicCondition() const;
	
	virtual void PeriodTick();
	virtual void PeriodTick_Implementation() {};
	
	void UpdateState(EGMASEffectState State, bool Force=false);

	void CheckState();

	virtual bool IsPeriodPaused();
	
	bool bCompleted;

	// Time that the client applied this Effect. Used for when a client predicts an effect, if the server has not
	// confirmed this effect within a time range, the effect will be cancelled.
	float ClientEffectApplicationTime;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GMCAbilitySystem")
	UGMC_AbilitySystemComponent* SourceAbilityComponent;

	UPROPERTY(BlueprintReadOnly, Category = "GMCAbilitySystem")
	UGMC_AbilitySystemComponent* OwnerAbilityComponent;

	// Tags
	void AddTagsToOwner();

	// bPreserveOnMultipleInstances: If true, will not remove tags if there are multiple instances of the same effect
	void RemoveTagsFromOwner(bool bPreserveOnMultipleInstances = true);

	void AddAbilitiesToOwner(const FGameplayTagContainer& TagsToAdd);
	void RemoveAbilitiesFromOwner(const FGameplayTagContainer& TagsToRemove);
	void EndActiveAbilitiesFromOwner();

	// Does the owner have any of the tags from the container?
	bool DoesOwnerHaveTagFromContainer(FGameplayTagContainer& TagContainer) const;
	
	bool DuplicateEffectAlreadyApplied();

	// Apply the things that should happen as soon as an effect starts. Tags, instant effects, etc.
	virtual void StartEffect();
	virtual void StartEffect_Implementation() {};

	bool bHasStarted;

private:
	// Used for calculating when to tick Period effects
	float PrevPeriodMod = 0;

public:
	FString ToString() {
		return FString::Printf(TEXT("[name: %s] (State %s) | Started: %d | Period Paused: %d | Data: %s"), *GetName(), *EnumToString(CurrentState), bHasStarted, IsPeriodPaused(), *EffectData.ToString());
	}
};

