// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ALEXTMap.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "AlexMap.h"

#define LOCTEXT_NAMESPACE "FALEXTMapModule"

#define BENCHMARK 0

void FALEXTMapModule::StartupModule()
{
#if BENCHMARK
	{
		TMap<int32, int32> Map;
		for (long idx = 0; idx < 10000000; idx++) {
			Map.Add(idx, idx);
		}

		auto TMapStartTime = std::chrono::high_resolution_clock::now();
		long x = 0;
		for (long idx = 0; idx < 10000000; idx++) {
			x =  Map[idx];
		}
		auto TMapEndTime = std::chrono::high_resolution_clock::now();
		double TMapLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TMapStartTime - TMapEndTime).count();
		UE_LOG(LogTemp, Warning, TEXT("TMap Lookup Time: %f"), TMapLookUpTime);

		TAlexMap<int32, int32> AlexMap;
		for (long idx = 0; idx < 10000000; idx++) {
			AlexMap.Add(idx, idx);
		}

		auto TAlexStartTime = std::chrono::high_resolution_clock::now();
		x = 0;
		for (long idx = 0; idx < 10000000; idx++) {
			x =  AlexMap[idx];
		}
		auto TAlexEndTime = std::chrono::high_resolution_clock::now();
		double TAlexLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TAlexStartTime - TAlexEndTime).count();
		UE_LOG(LogTemp, Warning, TEXT("TAlexMap Lookup Time: %f"), TAlexLookUpTime);
		
	}
#endif
}

void FALEXTMapModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FALEXTMapModule, ALEXTMap)
