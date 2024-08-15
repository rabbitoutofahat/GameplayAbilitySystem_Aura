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
	for (const FGameplayTag& Tag : TagContainer)
	{

		// TO DO: Broadcast the tag to the widget controller
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

	}

}
