// Copyright Druid Mechanics


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() // Calls EffectApplied (see below), can be bound to AuraCharacter and AuraEnemy classes
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) // When an effect is applied, call this function
{
	GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied"));
}
