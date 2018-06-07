// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UActorComponent;

#include "ActorComponentReference.generated.h"


USTRUCT(BlueprintType)
struct FActorComponentReference
{
	GENERATED_BODY();

	// TODO: It may be possible to remove the actor pointer from the struct and make it exit only inside the customization class, in order to make it error proof.
	// NOTE(Remi|2018/06/07): Read only so far cause I'm afraid of wrong setting from blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly) AActor* Actor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) UActorComponent* Component = nullptr;
};
