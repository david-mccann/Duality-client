#pragma once

#include "IVDA/Vectors.h"
#include "duality/InputVariable.h"
#include "duality/SceneMetadata.h"

#include "src/duality/Color.h"
#include "src/duality/Communication.h"
#include "src/duality/RenderParameters2D.h"
#include "src/duality/RenderParameters3D.h"
#include "src/duality/Scene.h"
#include "src/duality/TransferFunction.h"
#include "src/duality/View.h"

#include "mocca/base/Nullable.h"
#include "mocca/fs/Path.h"

#include "jsoncpp/json.h"

class GeometryDataset;
class VolumeDataset;
class SceneNode;
class DataProvider;
class DataCache;

class SceneParser {
public:
    SceneParser(const JsonCpp::Value& root, std::shared_ptr<LazyRpcClient> rpc, std::shared_ptr<DataCache> dataCache);

    std::unique_ptr<Scene> parseScene();
    static SceneMetadata parseMetadata(const JsonCpp::Value& root);
    mocca::Nullable<RenderParameters3D> initialParameters3D();
    mocca::Nullable<RenderParameters2D> initialParameters2D();

private:
    Visibility parseVisibility(const JsonCpp::Value& node);

    std::unique_ptr<SceneNode> parseGeometryNode(const JsonCpp::Value& node);
    std::unique_ptr<GeometryDataset> parseGeometryDataset(const JsonCpp::Value& node);

    std::unique_ptr<SceneNode> parseVolumeNode(const JsonCpp::Value& node);
    std::unique_ptr<VolumeDataset> parseVolumeDataset(const JsonCpp::Value& node);
    std::unique_ptr<TransferFunction> parseTransferFunction(const JsonCpp::Value& node);

    std::unique_ptr<DataProvider> parseProvider(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parseDownload(const JsonCpp::Value& node);
    std::unique_ptr<DataProvider> parsePython(const JsonCpp::Value& node);

    IVDA::Vec3f parseVector3(const JsonCpp::Value& node);
    IVDA::Vec2f parseVector2(const JsonCpp::Value& node);
    IVDA::Mat4f parseMatrix(const JsonCpp::Value& node);
    IVDA::Mat4f parseTransform(const JsonCpp::Value& node);

    Color parseColor(const JsonCpp::Value& node);
    
    void parseParams(const JsonCpp::Value& node);
    void parseFloatVariable(const JsonCpp::Value& node);
    void parseEnumVariable(const JsonCpp::Value& node);

private:
    JsonCpp::Value m_root;
    std::shared_ptr<LazyRpcClient> m_rpc;
    std::shared_ptr<DataCache> m_dataCache;
    std::string m_sceneName;
    std::map<std::string, IVDA::Mat4f> m_transforms;
    std::string m_nodeName;
    int m_varIndex;
    std::map<std::string, std::shared_ptr<Variables>> m_variables;
};