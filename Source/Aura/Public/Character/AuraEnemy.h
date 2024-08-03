// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

// As soon as we added the pure virtual functions Highlight/UnhighlightActor, this became an abstract class which cannot be instantiated without overriding
public:
	virtual void HighlightActor() override; 
	virtual void UnHighlightActor() override;
};
