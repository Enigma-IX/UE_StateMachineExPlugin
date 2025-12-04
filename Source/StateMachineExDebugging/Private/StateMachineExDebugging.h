// Source/StateMachineExDebugging/Private/StateMachineExDebugging.h
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStateMachineExDebugging, Log, All);

class FStateMachineExDebuggingModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};