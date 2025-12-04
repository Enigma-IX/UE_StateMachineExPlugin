// GameplayDebuggerCategory_StateMachine.cpp
#include "GameplayDebuggerCategory_StateMachine.h"
#include "StateMachine.h"
#include "State.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

FGameplayDebuggerCategory_StateMachine::FGameplayDebuggerCategory_StateMachine()
{
	bShowOnlyWithDebugActor = false;
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_StateMachine::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_StateMachine());
}

void FGameplayDebuggerCategory_StateMachine::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (!OwnerPC)
	{
		return;
	}

	// Collect actors to inspect
	TArray<UObject*> ObjectsToInspect;
	
	if (DebugActor)
	{
		ObjectsToInspect.Add(DebugActor);
		AddTextLine(FString::Printf(TEXT("{White}=== State Machines Debug for: {Yellow}%s {White}==="), *DebugActor->GetName()));
	}
	
	// Also check GameMode
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(OwnerPC))
	{
		ObjectsToInspect.AddUnique(GameMode);
	}

	bool bFoundAnyStateMachine = false;
	int32 StateMachineIndex = 0;

	// Inspect all objects
	for (UObject* ObjectToInspect : ObjectsToInspect)
	{
		if (!ObjectToInspect)
		{
			continue;
		}

		// Find all StateMachine properties
		for (TFieldIterator<FObjectProperty> PropertyIt(ObjectToInspect->GetClass(), EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			FObjectProperty* ObjectProperty = *PropertyIt;
			
			if (ObjectProperty->PropertyClass && 
			    (ObjectProperty->PropertyClass == UStateMachine::StaticClass() || 
			     ObjectProperty->PropertyClass->IsChildOf(UStateMachine::StaticClass())))
			{
				UStateMachine* StateMachine = Cast<UStateMachine>(ObjectProperty->GetPropertyValue_InContainer(ObjectToInspect));
				
				if (IsValid(StateMachine))
				{
					bFoundAnyStateMachine = true;
					
					// Header
					FString OwnerName = ObjectToInspect->GetName();
					AddTextLine(FString::Printf(TEXT("")));
					FColor HeaderColor = StateMachine->IsActive() ? FColor::Green : FColor::Red;
					AddTextLine(FString::Printf(TEXT("{Green}[%d] {White}State Machine: {Cyan}%s {Silver}(Owner: %s)"), 
						StateMachineIndex++, *ObjectProperty->GetName(), *OwnerName));
					
					// Status
					FString StatusText = StateMachine->IsActive() ? TEXT("{Green}ACTIVE") : TEXT("{Red}INACTIVE");
					AddTextLine(FString::Printf(TEXT("  Status: %s"), *StatusText));
					
					// Mode
					FString ModeText = StateMachine->bImmediateStateChange ? TEXT("Immediate") : TEXT("Deferred");
					AddTextLine(FString::Printf(TEXT("  Mode: {Cyan}%s"), *ModeText));

					// Current State
					if (IsValid(StateMachine->CurrentState))
					{
						AddTextLine(FString::Printf(TEXT("  Current State: {Yellow}%s"), 
							*StateMachine->CurrentState->GetClass()->GetName()));
						AddTextLine(FString::Printf(TEXT("    State ID: {Cyan}%d"), 
							StateMachine->CurrentState->StateId));
						AddTextLine(FString::Printf(TEXT("    Paused: %s"), 
							StateMachine->CurrentState->bPaused ? TEXT("{Red}YES") : TEXT("{Green}NO")));
					}
					else
					{
						AddTextLine(FString::Printf(TEXT("  Current State: {Red}None")));
					}

					// Next State
					if (IsValid(StateMachine->NextState))
					{
						AddTextLine(FString::Printf(TEXT("  Next State: {Orange}%s {White}(Pending)"), 
							*StateMachine->NextState->GetClass()->GetName()));
					}

					// Shutdown State
					FString ShutdownStateName = StateMachine->ShutdownState ? 
						StateMachine->ShutdownState->GetName() : TEXT("None");
					AddTextLine(FString::Printf(TEXT("  Shutdown State: {Cyan}%s"), *ShutdownStateName));

					// State Stack
					int32 StackSize = StateMachine->StateStack.Num();
					if (StackSize > 0)
					{
						AddTextLine(FString::Printf(TEXT("  State Stack ({Magenta}%d {White}items):"), StackSize));
						for (int32 i = StackSize - 1; i >= 0; --i)
						{
							UState* StackedState = StateMachine->StateStack[i];
							if (IsValid(StackedState))
							{
								FString Prefix = (i == StackSize - 1) ? TEXT("Top") : TEXT("   ");
								AddTextLine(FString::Printf(TEXT("    {Silver}[%s] {Cyan}%s"), 
									*Prefix, *StackedState->GetClass()->GetName()));
							}
							else
							{
								AddTextLine(FString::Printf(TEXT("    {Red}[Invalid]")));
							}
						}
					}
					else
					{
						AddTextLine(FString::Printf(TEXT("  State Stack: {Silver}Empty")));
					}
				}
			}
		}
	}

	if (!bFoundAnyStateMachine)
	{
		AddTextLine(FString::Printf(TEXT("{Yellow}No State Machines found")));
		AddTextLine(FString::Printf(TEXT("{Silver}Tip: Make sure your Actor has a StateMachine variable")));
	}
	else
	{
		// Legend
		AddTextLine(FString::Printf(TEXT("")));
		AddTextLine(FString::Printf(TEXT("{Silver}Legend:")));
		AddTextLine(FString::Printf(TEXT("  {Green}Green {White}= Active/Running")));
		AddTextLine(FString::Printf(TEXT("  {Red}Red {White}= Inactive/Paused")));
		AddTextLine(FString::Printf(TEXT("  {Yellow}Yellow {White}= Current State")));
		AddTextLine(FString::Printf(TEXT("  {Orange}Orange {White}= Pending State")));
		AddTextLine(FString::Printf(TEXT("  {Cyan}Cyan {White}= Stack/Info")));
	}
}

void FGameplayDebuggerCategory_StateMachine::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	// Avec AddTextLine dans CollectData, DrawData est géré automatiquement par la classe de base
	// Mais on doit quand même l'implémenter car c'est virtuel pur
}