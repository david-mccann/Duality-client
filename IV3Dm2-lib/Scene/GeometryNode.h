#pragma once

#include "G3D.h"
#include "SceneNode.h"

class DataProvider;
class AbstractDispatcher;

class GeometryNode : public SceneNode {
public:
    GeometryNode(std::unique_ptr<DataProvider> provider, SceneNode::MatrixTriple transforms = SceneNode::MatrixTriple{});

    // SceneNode interface
    void applyTransform(const IVDA::Mat4f& matrix) override;
    void accept(AbstractDispatcher& renderer) override;
    void readDataset(std::shared_ptr<std::vector<uint8_t>> data) override;

    const G3D::GeometryInfo& geometryInfo() const noexcept;
    const uint32_t* getIndices() const noexcept;
    const float* getPositions() const noexcept;
    const float* getNormals() const noexcept;
    const float* getTangents() const noexcept;
    const float* getColors() const noexcept;
    const float* getTexCoords() const noexcept;
    const float* getAlphas() const noexcept;

private:
    void assignShortcutPointers();

private:
    std::unique_ptr<G3D::GeometrySoA> m_geometry;

    // vertex attributes
    float* m_positions;
    float* m_normals;
    float* m_tangents;
    float* m_colors;
    float* m_texcoords;
    float* m_alphas;
};