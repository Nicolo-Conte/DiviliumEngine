#pragma once
#include <unordered_set>

struct EntityHandle
{
    union
    {
        struct
        {
            uint32_t index;
            uint32_t generation;
        };

        uint64_t handle;
    };
};

using HandlePair = std::pair<EntityHandle, EntityHandle>;
template <>
struct std::hash<HandlePair>
{
    std::size_t operator()(HandlePair const& h) const noexcept
    {
        std::size_t h1 = std::hash<uint64_t>{}(h.first.handle);
        std::size_t h2 = std::hash<uint64_t>{}(h.second.handle);
        return h1 ^ (h2 << 1);
    }
};

inline bool operator==(const HandlePair& lhs, const HandlePair& rhs) { if (lhs.first.handle == rhs.first.handle && lhs.second.handle == rhs.second.handle) { return true; } return false; }

