#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;

class FGameplayDebuggerCategory_StateMachine : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_StateMachine();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FStateMachineDebugData
	{
		FString StateMachineName;
		FString CurrentStateName;
		FString NextStateName;
		int32 StackSize;
		TArray<FString> StateStack;
		bool bIsActive;
		bool bImmediateStateChange;
		FString ShutdownStateName;
		
		// State details
		uint8 CurrentStateId;
		bool bCurrentStatePaused;
		FString StateClassName;
		float TimeInState;
		
		void Serialize(FArchive& Ar);
	};

	struct FRepData
	{
		TArray<FStateMachineDebugData> StateMachines;
		FString DebugActorName;
		
		void Serialize(FArchive& Ar)
		{
			Ar << DebugActorName;
			
			int32 NumStateMachines = StateMachines.Num();
			Ar << NumStateMachines;
			
			if (Ar.IsLoading())
			{
				StateMachines.SetNum(NumStateMachines);
			}
			
			for (int32 i = 0; i < NumStateMachines; i++)
			{
				StateMachines[i].Serialize(Ar);
			}
		}
	};
	
	FRepData DataPack;
};

#endif // WITH_GAMEPLAY_DEBUGGER