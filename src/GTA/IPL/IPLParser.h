#pragma once
#include "IPLTypes.h"
#include <string>
#include <vector>

class IPLParser {
public:
    IPLParser();
    ~IPLParser();

    bool Load(const std::string& filepath);
    const std::vector<IPLPlacement>& GetPlacements() const { return m_Placements; }
    size_t GetPlacementCount() const { return m_Placements.size(); }

private:
    std::vector<IPLPlacement> m_Placements;
};