// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ALEXTMap.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "AlexMap.h"

#define LOCTEXT_NAMESPACE "FALEXTMapModule"

#define ALEX_BENCHMARK 0

void FALEXTMapModule::StartupModule()
{
#if ALEX_BENCHMARK
	{
		const int32 Max = 10000000;
		
		TMap<int32, int32> Map;
		for (int32 Index = 0; Index < Max; Index++) {
			Map.Add(Index, Index);
		}

		auto TMapStartTime = std::chrono::high_resolution_clock::now();
		int32 x = 0;
		for (int32 Index = 0; Index < Max; Index++) {
			x =  Map[Index];
		}
		auto TMapEndTime = std::chrono::high_resolution_clock::now();
		double TMapLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TMapEndTime - TMapStartTime).count();
		UE_LOG(LogTemp, Warning, TEXT("TMap Lookup Time: %f"), TMapLookUpTime);

		TAlexMap<int32, int32> AlexMap;
		for (int32 Index = 0; Index < Max; Index++) {
			AlexMap.Add(Index, Index);
		}

		auto TAlexStartTime = std::chrono::high_resolution_clock::now();
		x = 0;
		for (int32 Index = 0; Index < Max; Index++) {
			x =  AlexMap[Index];
		}
		auto TAlexEndTime = std::chrono::high_resolution_clock::now();
		double TAlexLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TAlexEndTime - TAlexStartTime).count();
		UE_LOG(LogTemp, Warning, TEXT("TAlexMap Lookup Time: %f"), TAlexLookUpTime);
		
	}
#endif
}

void FALEXTMapModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FALEXTMapModule, ALEXTMap)
