//  Created by David McCann on 5/5/16.
//  Copyright © 2016 Scientific Computing and Imaging Institute. All rights reserved.
//

#pragma once

#include "Scene/DataProvider.h"
#include "Scene/Dataset.h"
#include "Scene/RenderDispatcher.h"
#include "Scene/BoundingBoxCalculator.h"

#include <memory>
#include <vector>

class SceneNode {
public:
    SceneNode(const std::string& name, std::unique_ptr<DataProvider> provider, std::unique_ptr<Dataset> dataset);

    void updateDataset();
    void dispatch(DatasetDispatcher& dispatcher) const;

    std::string name() const;

    const DataProvider* dataProvider() const;
    DataProvider* dataProvider();
    const Dataset* dataset() const;
    
private:
    std::string m_name;
    std::unique_ptr<DataProvider> m_provider;
    std::unique_ptr<Dataset> m_dataset;
};