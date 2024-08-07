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
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); 
	/** 
	* Take EffectSpecHandle, get the data which is a TSharedPtr storing a FGameplayEffectSpec pointer in its raw form, call Get to get that raw pointer, 
	* then deference everything into the FGameplayEffectSpec that the ApplyGameplayEffectSpecToSelf function wants.
	*/

}

