#pragma once
#include "Core/Hash.h"

using tileInt = int16_t;
using tilePos = std::pair<tileInt, tileInt>;

template <>
inline uint32_t divil::Hash::KeyHash(const tilePos& p)
{
    auto h1 = std::hash<tileInt>{}(p.first);
    auto h2 = std::hash<tileInt>{}(p.second);

    // Mainly for demonstration purposes, i.e. works but is overly simple
    // In the real world, use sth. like boost.hash_combine
    return uint32_t(h1 ^ h2);
}

struct TilePositions
{
    //tilePos positions[4];
    glm::vec2 minXY;
    glm::vec2 maxXY;
};
