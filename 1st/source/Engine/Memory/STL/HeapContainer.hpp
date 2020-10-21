
#pragma once

#include "SHeapAllocator.hpp"
#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <deque>
#include <stack>

namespace NE
{

// HeapVector
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using HeapVector = std::vector<T, Alloc>;

// HeapList
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using HeapList = std::list<T, Alloc>;

// HeapForwardList
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using HeapForwardList = std::forward_list<T, Alloc>;

// HeapMap
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using HeapMap = std::map<Key, T, Compare, Alloc>;

// HeapMultiMap
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using HeapMultiMap = std::multimap<Key, T, Compare, Alloc>;

// HeapUnorderedMap
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using HeapUnorderedMap = std::unordered_map<Key, T, Hash, Pred, Alloc>;

// HeapUnorderedMultiMap
template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename Pred = std::equal_to<Key>,
          typename Alloc = STL::SHeapAllocator<std::pair<const Key, T>>>
using HeapUnorderedMultiMap =
  std::unordered_multimap<Key, T, Hash, Pred, Alloc>;

// HeapSet
template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<Key>>
using HeapSet = std::set<Key, Compare, Alloc>;

// HeapMultiSet
template <typename Key, typename Compare = std::less<Key>,
          typename Alloc = STL::SHeapAllocator<Key>>
using HeapMultiSet = std::multiset<Key, Compare, Alloc>;

// HeapUnorderedSet
template <typename Value, typename Hash = std::hash<Value>,
          typename Pred = std::less<Value>,
          typename Alloc = STL::SHeapAllocator<Value>>
using HeapUnorderedSet = std::unordered_set<Value, Hash, Pred, Alloc>;

// HeapUnorderedMultiSet
template <typename Value, typename Hash = std::hash<Value>,
          typename Pred = std::less<Value>,
          typename Alloc = STL::SHeapAllocator<Value>>
using HeapUnorderedMultiSet = std::unordered_multiset<Value, Hash, Pred, Alloc>;

// HeapDeque
template <typename T, typename Alloc = STL::SHeapAllocator<T>>
using HeapDeque = std::deque<T, Alloc>;

// HeapQueue
template <typename T, typename Sequence = HeapDeque<T>>
using HeapQueue = std::queue<T, Sequence>;

// HeapStack
template <typename T, typename Sequence = HeapDeque<T>>
using HeapStack = std::stack<T, Sequence>;

} // namespace NE
