#include "gmock/gmock.h"

#include "Scene/GeometryNode.h"
#include "Mocks/DataProviderMock.h"
#include "Scene/SceneMetadata.h"
#include "Scene/Scene.h"

using namespace ::testing;

class SceneNodeTest : public Test {
protected:
    SceneNodeTest() {}

    virtual ~SceneNodeTest() {}
};

TEST_F(SceneNodeTest, UpdateDatasets) {
    SceneMetadata meta("test", "test");
    Scene scene(meta);

    auto mock1 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*mock1, fetch()).Times(1).WillOnce(Return(nullptr));
    scene.addNode(std::make_unique<GeometryNode>(std::move(mock1)));

    auto mock2 = std::make_unique<DataProviderMock>();
    EXPECT_CALL(*mock2, fetch()).Times(1).WillOnce(Return(nullptr));
    scene.addNode(std::make_unique<GeometryNode>(std::move(mock2)));

    scene.updateDatasets();
}