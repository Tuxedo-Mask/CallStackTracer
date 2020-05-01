#pragma once

#include "Comarator.h"
#include "CustomAllocator/CustomAllocator.h"

#include <string>
#include <vector>
#include <set>
#include <map>

template<typename T>
using CustomAllocatedVector = std::vector<T, CustomAllocator<T>>;

template<typename T>
using CustomAllocatedSet = std::set<T, CustomCompare<T>, CustomAllocator<T>>;

template<typename T, typename U>
using CustomAllocatedMap = std::map<T, U, CustomCompare<T>, CustomAllocator<T>>;

using CustomAllocatedString = std::basic_string<char, std::char_traits<char>, CustomAllocator<char>>;
