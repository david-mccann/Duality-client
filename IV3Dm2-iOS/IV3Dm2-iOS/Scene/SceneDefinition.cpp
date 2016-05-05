//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#include "SceneDefinition.h"

SceneDefinition::SceneDefinition(const std::string& name)
    : m_name(name) {}

SceneDefinition SceneDefinition::fromJson(const JsonCpp::Value& root) {
    std::string name = root["name"].asString();
    return SceneDefinition(name);
}

std::string SceneDefinition::name() const {
    return m_name;
}