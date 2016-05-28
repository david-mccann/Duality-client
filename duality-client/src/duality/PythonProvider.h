#pragma once

#include "duality/VariableInfo.h"

#include "src/duality/Communication.h"
#include "src/duality/DataProvider.h"
#include "src/duality/InputVariable.h"

#include <memory>
#include <vector>

class PythonProvider : public DataProvider {
public:
    PythonProvider(const std::string& sceneName, const std::string& fileName, std::vector<FloatVariableInfo> floatInfos,
                   std::vector<EnumVariableInfo> enumUnfos, std::shared_ptr<LazyRpcClient> rpc);

    // DataProvider interface
    std::shared_ptr<std::vector<uint8_t>> fetch() override;
    std::vector<FloatVariableInfo> floatVariableInfos() const override;
    std::vector<EnumVariableInfo> enumVariableInfos() const override;
    void setVariable(const std::string& variable, float value) override;
    void setVariable(const std::string& variable, const std::string& value) override;

    std::string fileName() const;

private:
    std::string m_sceneName;
    std::string m_fileName;
    std::vector<InputVariableFloat> m_floatVariables;
    std::vector<InputVariableEnum> m_enumVariables;
    std::shared_ptr<LazyRpcClient> m_rpc;
    bool m_dirty;
};