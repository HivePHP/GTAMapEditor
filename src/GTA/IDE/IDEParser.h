#pragma once
#include "IDETypes.h"
#include <string>
#include <vector>
#include <unordered_map>

class IDEParser {
public:
    IDEParser();
    ~IDEParser();

    bool Load(const std::string& filepath);
    const IDEObject* Find(int id) const;

    size_t GetObjectCount() const { return m_Objects.size(); }
    const std::vector<IDEObject>& GetObjectsList() const { return m_ObjectsList; }

private:
    std::unordered_map<int, IDEObject> m_Objects;
    std::vector<IDEObject> m_ObjectsList;
};