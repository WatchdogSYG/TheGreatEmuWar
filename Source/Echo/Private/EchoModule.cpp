// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"


class FEchoModule : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE( FEchoModule, Echo);
