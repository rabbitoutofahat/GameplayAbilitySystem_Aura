// Copyright Druid Mechanics


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() // Calls EffectApplied (see below), can be bound to AuraCharacter and AuraEnemy classes
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) // When an effect is applied, call this function (we want to show stuff in the HUD)
{
	
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer); // Fill the tag container with our asset tags

	EffectAssetTags.Broadcast(TagContainer); // Any class that binds to our EffectAssetTag's delegate will receive a tag container filled with the asset tags

}
