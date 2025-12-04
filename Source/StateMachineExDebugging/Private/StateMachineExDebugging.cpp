// Source/StateMachineExDebugging/Private/StateMachineExDebugging.cpp
#include "StateMachineExDebugging.h"
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategory_StateMachine.h"

#define LOCTEXT_NAMESPACE "FStateMachineExDebuggingModule"

DEFINE_LOG_CATEGORY(LogStateMachineExDebugging);

IMPLEMENT_MODULE(FStateMachineExDebuggingModule, StateMachineExDebugging)

void FStateMachineExDebuggingModule::StartupModule()
{
	IModuleInterface::StartupModule();

	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("StateMachine", 
		IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_StateMachine::MakeInstance),
		EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
		7); // Slot 7 (Numpad 7)
	
	GameplayDebuggerModule.NotifyCategoriesChanged();
	
	UE_LOG(LogStateMachineExDebugging, Log, TEXT("StateMachine Gameplay Debugger Category registered"));
}

void FStateMachineExDebuggingModule::ShutdownModule()
{
	if (IGameplayDebugger::IsAvailable())
	{
		IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
		GameplayDebuggerModule.UnregisterCategory("StateMachine");
		GameplayDebuggerModule.NotifyCategoriesChanged();
	}
	
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE