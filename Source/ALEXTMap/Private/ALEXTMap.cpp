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
		const int32 LookupSize = 1000000;
		int32 x = 0;

		// TMap read and write lookup
		TMap<int32, int32> Map;
		for (int32 Index = 0; Index < LookupSize; Index++) {
			Map.Add(Index, Index);
		}

		auto TMapStartTime = std::chrono::high_resolution_clock::now();
		x = 0;
		for (int32 Index = 0; Index < LookupSize; Index++) {
			x =  Map[Index];
		}
		auto TMapEndTime = std::chrono::high_resolution_clock::now();
		double TMapLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TMapEndTime - TMapStartTime).count();
		UE_LOG(LogTemp, Warning, TEXT("Lookup size: %i, TMap(Read+Write) Lookup Time: %fns (%f ms)"), LookupSize, TMapLookUpTime, TMapLookUpTime / 1000000.0f);

		TAlexMap<int32, int32> AlexMap;
		for (int32 Index = 0; Index < LookupSize; Index++) {
			AlexMap.Add(Index, Index);
		}

		// TAlexMap read and write lookup
		auto TAlexRWStartTime = std::chrono::high_resolution_clock::now();
		x = 0;
		for (int32 Index = 0; Index < LookupSize; Index++) {
			x =  AlexMap[Index];
		}
		auto TAlexRWEndTime = std::chrono::high_resolution_clock::now();
		double TAlexRWLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TAlexRWEndTime - TAlexRWStartTime).count();
		UE_LOG(LogTemp, Warning, TEXT("Lookup size: %i, TAlexMap(Read+Write) Lookup Time: %fns (%f ms))"), LookupSize, TAlexRWLookUpTime, TAlexRWLookUpTime / 1000000.0f);

		// TAlexMap read lookup
		auto TAlexRStartTime = std::chrono::high_resolution_clock::now();
		x = 0;
		for (int32 Index = 0; Index < LookupSize; Index++) {
			x =  AlexMap.At(Index);
		}
		auto TAlexREndTime = std::chrono::high_resolution_clock::now();
		double TAlexRLookUpTime =
            std::chrono::duration_cast<std::chrono::nanoseconds>(TAlexREndTime - TAlexRStartTime).count();
		UE_LOG(LogTemp, Warning, TEXT("Lookup size: %i, TAlexMap(Read) Lookup Time: %fns (%f ms)"), LookupSize, TAlexRLookUpTime, TAlexRLookUpTime / 1000000.0f);
		
	}
#endif
}

void FALEXTMapModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FALEXTMapModule, ALEXTMap)
