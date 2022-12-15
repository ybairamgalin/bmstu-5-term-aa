#pragma once

#include <string>

namespace algorithm {

int DamerauLevenshteinDistance(
    const std::string& first, const std::string& second);

int DamerauLevenshteinDistanceRecursive(
    const std::string& first, const std::string& second);

int DamerauLevenshteinDistanceRecursiveCached(
    const std::string& first, const std::string& second);

}  // namespace algorithm
