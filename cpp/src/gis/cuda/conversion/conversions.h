#pragma once
#include "gis/cuda/common/gis_definitions.h"
#include "gis/cuda/mock/arrow/api.h"
namespace zilliz {
namespace gis {
namespace cuda {

GeometryVector ArrowWkbToGeometryVector(const std::shared_ptr<arrow::Array>& wkb_arrow);
std::shared_ptr<arrow::Array> GeometryVectorToArrowWkb(const GeometryVector&);

}  // namespace cuda
}  // namespace gis
}  // namespace zilliz
#include "conversions.impl.h"