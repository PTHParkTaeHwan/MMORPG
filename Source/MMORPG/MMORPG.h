// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Net/UnrealNetwork.h"
#include <vector>

DECLARE_LOG_CATEGORY_EXTERN(MMORPG, Log, All);
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__)+TEXT(")"))
#define LOG_S(Verbosty) UE_LOG(MMORPG, Verbosity, TEXT("%s"), *LOG_CALLINFO)
#define LOG(Verbosity, Format, ...) UE_LOG(MMORPG, Verbosity, TEXT("%s%s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define ABCHECK(Expr, ...) { if(!(Expr)) { LOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

