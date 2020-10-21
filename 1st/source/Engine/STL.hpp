#pragma once

#include "Memory/MemoryConfig.hpp"
#include "Memory/STL/HeapContainer.hpp"
#include "Memory/STL/HeapMemory.hpp"
#include "Memory/STL/HeapString.hpp"

namespace NE
{

// String
typedef HeapString String;
typedef HeapStringStream StringStream;
typedef HeapIStringStream IStringStream;
typedef HeapOStringStream OStringStream;

// Vector
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using Vector = HeapVector<T, Alloc>;

// List
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using List = HeapList<T, Alloc>;

// ForwardList
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using ForwardList = HeapForwardList<T, Alloc>;

// Map
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using Map = HeapMap<Key, T, Compare, Alloc>;

// MultiMap
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using MultiMap = HeapMultiMap<Key, T, Compare, Alloc>;

// UnorderedMap
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using UnorderedMap = HeapUnorderedMap<Key, T, Hash, Pred, Alloc>;

// UnorderedMultiMap
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using UnorderedMultiMap = HeapUnorderedMultiMap<Key, T, Hash, Pred, Alloc>;

// Set
template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<Key>>
using Set = HeapSet<Key, Compare, Alloc>;

// MultiSet
template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<Key>>
using MultiSet = HeapMultiSet<Key, Compare, Alloc>;

// UnorderedSet
template <typename Value, typename Hash = std::hash<Value>,
          typename Pred = std::less<Value>,
          typename Alloc = STL::SHeapAllocator<Value>>
using UnorderedSet = HeapUnorderedSet<Value, Hash, Pred, Alloc>;

// UnorderedMultiSet
template <typename Value, typename Hash = std::hash<Value>,
          typename Pred = std::less<Value>,
          typename Alloc = STL::SHeapAllocator<Value>>
using HeapUnorderedMultiSet = HeapUnorderedMultiSet<Value, Hash, Pred, Alloc>;

// Deque
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using Deque = HeapDeque<T, Alloc>;

// Queue
template <typename T, typename Sequence = HeapDeque<T>>
using Queue = HeapQueue<T, Sequence>;

// Stack
template <typename T, typename Sequence = HeapDeque<T>>
using Stack = HeapStack<T, Sequence>;

} // namespace NE


