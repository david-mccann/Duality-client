#include "src/duality/PythonProvider.h"

#include "duality/Error.h"

#include <algorithm>
#include <cassert>

PythonProvider::PythonProvider(const std::string& sceneName, const std::string& fileName, std::vector<FloatVariableInfo> floatInfos,
                               std::vector<EnumVariableInfo> enumInfos, std::shared_ptr<LazyRpcClient> rpc)
    : m_sceneName(sceneName)
    , m_fileName(fileName)
    , m_rpc(rpc)
    , m_dirty(true) {
    for (const auto& info : floatInfos) {
        m_floatVariables.emplace_back(info);
    }
    for (const auto& info : enumInfos) {
        m_enumVariables.emplace_back(info);
    }
}

std::shared_ptr<std::vector<uint8_t>> PythonProvider::fetch() {
    if (!m_dirty)
        return nullptr;

    JsonCpp::Value values;
    for (const auto& var : m_floatVariables) {
        values[var.info().name] = var.value();
    }
    for (const auto& var : m_enumVariables) {
        values[var.info().name] = var.value();
    }

    JsonCpp::Value params;
    params["scene"] = m_sceneName;
    params["filename"] = m_fileName;
    params["variables"] = values;
    m_rpc->send("python", params);
    auto reply = m_rpc->receive();
    m_dirty = false;
    return reply.second[0];
}


std::vector<FloatVariableInfo> PythonProvider::floatVariableInfos() const {
    std::vector<FloatVariableInfo> result;
    for (const auto& var : m_floatVariables) {
        result.push_back(var.info());
    }
    return result;
}

std::vector<EnumVariableInfo> PythonProvider::enumVariableInfos() const {
    std::vector<EnumVariableInfo> result;
    for (const auto& var : m_enumVariables) {
        result.push_back(var.info());
    }
    return result;
}

void PythonProvider::setVariable(const std::string& variable, float value) {
    auto it = std::find_if(begin(m_floatVariables), end(m_floatVariables),
                           [&variable](const InputVariableFloat& var) { return var.info().name == variable; });
    assert(it != end(m_floatVariables));
    it->setValue(value);
    m_dirty = true;
}

void PythonProvider::setVariable(const std::string& variable, const std::string& value) {
    auto it = std::find_if(begin(m_enumVariables), end(m_enumVariables),
                           [&variable](const InputVariableEnum& var) { return var.info().name == variable; });
    assert(it != end(m_enumVariables));
    it->setValue(value);
    m_dirty = true;
}

std::string PythonProvider::fileName() const {
    return m_fileName;
}
