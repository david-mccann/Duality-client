#include "src/duality/BoundingBox.h"

#include "src/duality/GeometryDataset.h"
#include "src/duality/VolumeDataset.h"

#include <limits>
#include <cassert>

BoundingBoxCalculator::BoundingBoxCalculator() {
    m_min = IVDA::Vec3f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_max = IVDA::Vec3f(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
}

void BoundingBoxCalculator::dispatch(GeometryDataset& dataset) {
    if (dataset.geometryInfo() == nullptr) return;
    assert(dataset.getPositions() != nullptr);
    for (size_t i = 0; i < dataset.geometryInfo()->numberIndices; ++i) {
        auto offset = 3 * dataset.getIndices()[i];
        const auto positions = dataset.getPositions();
        IVDA::Vec3f pos(positions[offset + 0], positions[offset + 1], positions[offset + 2]);
        m_min.StoreMin(pos);
        m_max.StoreMax(pos);
    }
}

void BoundingBoxCalculator::dispatch(VolumeDataset& dataset) {
    BoundingBox bb = dataset.boundingBox();
    m_min.StoreMin(bb.min);
    m_max.StoreMax(bb.max);
}

BoundingBox BoundingBoxCalculator::boundingBox() const {
    return BoundingBox{m_min, m_max};
}
