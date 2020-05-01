#pragma once

template <typename T>
class CustomCompare
{
public:
    bool operator() (const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};