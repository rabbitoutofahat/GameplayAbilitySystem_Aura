// Copyright Druid Mechanics


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>


AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

void AAuraEffectActor::BeginPlay()
{

	Super::BeginPlay();

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); // A way to get an ASC of any actor in blueprint
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext(); // Makes effect context and returns a FGameplayEffectContext handle
	EffectContextHandle.AddSourceObject(this); // "What object caused the effect?"
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle); // Gives a Spec handle
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	/** 
	* Take EffectSpecHandle, get the data which is a TSharedPtr storing a FGameplayEffectSpec pointer in its raw form, call Get to get that raw pointer, 
	* then deference everything into the FGameplayEffectSpec that the ApplyGameplayEffectSpecToSelf function wants.
	*/

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // If duration type is infinite and removed on end overlap, then store the active effect handle in a map
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{

	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // Check our map for a key value pair that has an ASC that matches the ASC of our end overlap actor
	{

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{

			if (TargetASC == HandlePair.Value)
			{

				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				/**
				* If we want to remove a gameplay effect, we should also remove the pair from the map, but we are currently looping through the map itself
				* Instead, as we move through gameplay effects, we can accumulate key value pairs we want to remove after we're done with the for loop in a container
				*/
				HandlesToRemove.Add(HandlePair.Key);

			}

		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}

	}

}

