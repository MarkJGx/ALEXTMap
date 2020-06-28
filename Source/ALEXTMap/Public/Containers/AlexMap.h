// Copyright 1998-2019 MarkJGx, Inc. All Rights Reserved.

#pragma once

#include "alex.h"
#include "Containers/Map.h"

// Notes: Originally I wanted to inherit from TMapBase and override everything that touches the Pair set, that's a really hacky solution but it's the best solution to ensure compatability with every other container.
// TODO replace with std::allocator with UE4 TSetAllocator

/*
 * "Keys and payloads (i.e., the mapped type) are stored separately,
 * so dereferencing an iterator returns a copy of the key/payload pair, not a reference."
 * "Our iterators have methods to directly return references to the key or payload individually."
 * "Currently, we only support numerical key types. We do not support arbitrary user-defined key types. As a result, you should not change the default comparison function in the class template."
 * 
 * https://github.com/microsoft/ALEX
 */
template <typename KeyType, typename ValueType, class SetAllocator = std::allocator<std::pair<KeyType, ValueType>>, typename KeyFuncs = TDefaultMapHashableKeyFuncs<KeyType, ValueType, false>, class Compare = alex::AlexCompare>
class TAlexMap
{
    static_assert(TIsArithmetic<KeyType>::Value, "ALEX key type must be numeric.");
    static_assert(TIsSame<Compare, alex::AlexCompare>::Value, "Must use AlexCompare.");

public:
    // Value type, returned by dereferencing an iterator
    typedef TPair<KeyType, ValueType> ElementType; // V

    // ALEX class aliases
    typedef alex::Alex<KeyType, ValueType, Compare, SetAllocator, KeyFuncs::bAllowDuplicateKeys> AlexImpl;
    typedef TAlexMap<KeyType, ValueType, SetAllocator, KeyFuncs, Compare> SelfType;
    typedef typename AlexImpl::Iterator Iterator;
    typedef typename AlexImpl::ConstIterator ConstIterator;
    typedef typename AlexImpl::ReverseIterator ReverseIterator;
    typedef typename AlexImpl::ConstReverseIterator ConstReverseIterator;

public:
    TAlexMap() = default;
    TAlexMap(TAlexMap&&) = default;
    TAlexMap(const TAlexMap&) = default;
    TAlexMap& operator=(TAlexMap&&) = default;
    TAlexMap& operator=(const TAlexMap&) = default;

    /** Constructor for moving elements from a TAlexMap with a different SetAllocator */
    template <typename OtherSetAllocator>
    TAlexMap(TAlexMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other) : Alex(MoveTemp(Other.Alex))
    {
    }

    /** Constructor for copying elements from a TAlexMap with a different SetAllocator */
    template <typename OtherSetAllocator>
    TAlexMap(const TAlexMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other) : Alex(Other.Alex)
    {
    }

    /** Assignment operator for moving elements from a TAlexMap with a different SetAllocator */
    template <typename OtherSetAllocator>
    TAlexMap& operator=(TAlexMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>&& Other)
    {
        Alex = MoveTemp(Other.Alex);
        return *this;
    }

    /** Assignment operator for copying elements from a TAlexMap with a different SetAllocator */
    template <typename OtherSetAllocator>
    TAlexMap& operator=(const TAlexMap<KeyType, ValueType, OtherSetAllocator, KeyFuncs>& Other)
    {
        Alex = Other.Alex;
        return *this;
    }

public:
    FORCEINLINE void Swap(const SelfType& Other)
    {
        Alex.swap(Other.Alex);
    }

    /**
    * Removes all elements from the map.
    *
    * This method potentially leaves space allocated for an expected
    * number of elements about to be added.
    */
    FORCEINLINE void Empty()
    {
        Alex.clear();
    }

    /** @return The number of elements in the map. */
    FORCEINLINE int32 Num() const
    {
        return Alex.size();
    }

    FORCEINLINE ValueType Add(const KeyType& InKey, ValueType&& InValue)
    {
        return Add(InKey, MoveTempIfPossible(InValue));
    }

    FORCEINLINE ValueType Add(const KeyType& InKey, const ValueType& InValue)
    {
        std::pair<Iterator, bool> Pair = Alex.insert(InKey, InValue);
        return Pair.second;
    }

    FORCEINLINE ValueType& operator[](const KeyType& Key) { return Alex.insert(Key, ValueType()).first.payload(); }

    FORCEINLINE ValueType& At(const KeyType& Key)
    {
        ValueType* Payload = Alex.get_payload(Key);
        check(Payload != nullptr, "TAlexMap::At: input does not match any key.");
        return *Payload;
    }

    FORCEINLINE const ValueType& At(const KeyType& Key) const
    {
        ValueType* Payload = Alex.get_payload(Key);
        check(Payload != nullptr, "TAlexMap::At: input does not match any key.");
        return *Payload;
    }

    /**
    * Finds an element with the given key in the set.
    * @param Key - The key to search for.
    * @return A pointer to an element with the given key.  If no element in the set has the given key, this will return NULL.
    */
    FORCEINLINE const ValueType& Find(const KeyType& Key) const
    {
        ValueType* Payload = Alex.get_payload(Key);
        return Payload;
    }

    // When bulk loading, Alex can use provided knowledge of the expected fraction
    // of operations that will be inserts
    // For simplicity, operations are either point lookups ("reads") or inserts
    // ("writes)
    // i.e., 0 means we expect a read-only workload, 1 means write-only
    // This is only useful if you set it before bulk loading
    FORCEINLINE void SetExpectedInsertFrac(double expected_insert_frac)
    {
        Alex.set_expected_insert_frac(expected_insert_frac);
    }

    // Maximum node size, in bytes.
    // Higher values result in better average throughput, but worse tail/max
    // insert latency.
    FORCEINLINE void SetMaxNodeSize(int32 max_node_size)
    {
        Alex.set_max_node_size(max_node_size);
    }

    // Bulk load faster by using sampling to train models.
    // This is only useful if you set it before bulk loading.
    FORCEINLINE void SetApproximateModelComputation(bool approximate_model_computation)
    {
        Alex.set_approximate_model_computation(approximate_model_computation);
    }

    // Bulk load faster by using sampling to compute cost.
    // This is only useful if you set it before bulk loading.
    FORCEINLINE void SetApproximateCostComputation(bool approximate_cost_computation)
    {
        Alex.set_approximate_cost_computation(approximate_cost_computation);
    }

private:
    AlexImpl Alex;
};
