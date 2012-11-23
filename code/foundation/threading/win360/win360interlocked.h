#pragma once
//------------------------------------------------------------------------------
/**
    @class Win360::Win360Interlocked
    
    Provides simple atomic operations on shared variables.
    
    (C) 2006 Radon Labs GmbH
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Win360
{
class Win360Interlocked
{
public:
    /// interlocked increment, return result
    static int Increment(int volatile& var);
    /// interlocked decrement, return result
    static int Decrement(int volatile& var);
    /// interlocked add
    static int Add(int volatile& var, int add);
    /// interlocked exchange
    static int Exchange(int volatile* dest, int value);
    /// interlocked compare-exchange
    static int CompareExchange(int volatile* dest, int exchange, int comparand);
};

//------------------------------------------------------------------------------
/**
*/
__forceinline int
Win360Interlocked::Increment(int volatile& var)
{
    return _InterlockedIncrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
Win360Interlocked::Decrement(int volatile& var)
{
    return _InterlockedDecrement((volatile LONG*)&var);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
Win360Interlocked::Add(int volatile& var, int add)
{
    return _InterlockedExchangeAdd((volatile LONG*)&var, add);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
Win360Interlocked::Exchange(int volatile* dest, int value)
{
    return _InterlockedExchange((volatile LONG*)dest, value);
}

//------------------------------------------------------------------------------
/**
*/
__forceinline int
Win360Interlocked::CompareExchange(int volatile* dest, int exchange, int comparand)
{
    return _InterlockedCompareExchange((volatile LONG*)dest, exchange, comparand);
}

} // namespace Win360
//------------------------------------------------------------------------------
