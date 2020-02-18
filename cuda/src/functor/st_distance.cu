//
// Created by mike on 2/10/20.
//
#include <cuda_runtime.h>
#include <cuda_device_runtime_api.h>
#include <cmath>
#include "functor/st_distance.h"
#include "common/gpu_memory.h"

namespace zilliz {
namespace gis {
namespace cuda {
namespace {
inline DEVICE_RUNNABLE double
Point2PointDistance(const GeoContext& left, const GeoContext& right, int index) {
    auto lv = left.get_value_ptr(index);
    auto rv = right.get_value_ptr(index);
    auto dx = (lv[0] - rv[0]);
    auto dy = (lv[1] - rv[1]);
    return sqrt(dx * dx + dy * dy);
}

__global__ void
ST_DistanceKernel(GeoContext left, GeoContext right, double* result) {
    auto tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid < left.size) {
        auto left_tag = left.get_tag(tid);
        auto right_tag = right.get_tag(tid);
        // handle 2d case only for now
        assert(left_tag.get_group() == WkbGroup::None);
        assert(right_tag.get_group() == WkbGroup::None);
        if (left_tag.get_category() == WkbCategory::Point &&
            right_tag.get_category() == WkbCategory::Point) {
            result[tid] = Point2PointDistance(left, right, tid);
        } else {
            result[tid] = NAN;
        }
    }
}
}    // namespace

void
ST_Distance(const GeometryVector& left,
            const GeometryVector& right,
            double* host_results) {
    assert(left.size() == right.size());
    auto left_ctx = left.CreateReadGeoContext();
    auto right_ctx = right.CreateReadGeoContext();
    auto dev_result = GpuMakeUniqueArray<double>(left.size());
    {
        auto config = GetKernelExecConfig(left.size());
        ST_DistanceKernel<<<config.grid_dim, config.block_dim>>>(
            *left_ctx, *right_ctx, dev_result.get());
    }
    GpuMemcpy(host_results, dev_result.get(), left.size());
}

}    // namespace cuda
}    // namespace gis
}    // namespace zilliz
