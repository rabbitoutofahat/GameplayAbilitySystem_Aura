// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UAttributeSet;


UCLASS(Abstract) // Abstract prevents this class base from being dragged into the level
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	// Pointers for our ability system component and attribute set after implementing AbilitySystemInterface above, helps everything in the system interact with eachother cleanly
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // Taken from AbilitySystemInterface.h
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

    //These 2 properties are constructed on both AuraEnemy.cpp and AuraPlayerState.cpp
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

};
