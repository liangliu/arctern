#include "gtest/gtest.h"
#include "arrow/render_api.h"
#include "render/2d/heatmap.h"

using namespace zilliz::render;

TEST(TWOD_TEST, HEATMAP_TEST) {
    HeatMap<uint32_t > heat_map;

    auto bit_map = new uint8_t{0xff};

    auto data_type = arrow::uint32();

    auto buff_data1 = (uint32_t* )malloc(5 * sizeof(uint32_t));
    for (int i = 0; i < 5; ++i) {
        buff_data1[i] = i + 50;
    }
    auto buffer0 = std::make_shared<arrow::Buffer>(bit_map, 1 * sizeof(uint8_t));
    auto buffer1 = std::make_shared<arrow::Buffer>((uint8_t *)buff_data1, 5 * sizeof(uint32_t));
    std::vector<std::shared_ptr<arrow::Buffer>> buffers1;
    buffers1.emplace_back(buffer0);
    buffers1.emplace_back(buffer1);
    auto array_data1 = arrow::ArrayData::Make(data_type, 5, buffers1);
    auto array1 = arrow::MakeArray(array_data1);

    auto bit_map2 = new uint8_t{0xff};

    auto buff_data2 = (uint32_t* )malloc(5 * sizeof(uint32_t));
    for (int i = 0; i < 5; ++i) {
        buff_data2[i] = i + 50;
    }
    auto buffer20 = std::make_shared<arrow::Buffer>(bit_map2, 1 * sizeof(uint8_t));
    auto buffer21 = std::make_shared<arrow::Buffer>((uint8_t *)buff_data2, 5 * sizeof(uint32_t));
    std::vector<std::shared_ptr<arrow::Buffer>> buffers2;
    buffers2.emplace_back(buffer20);
    buffers2.emplace_back(buffer21);
    auto array_data2 = arrow::ArrayData::Make(arrow::uint32(), 5, buffers2);
    auto array2 = arrow::MakeArray(array_data2);

    auto bit_map3 = new uint8_t{0xff};

    auto buff_data3 = (uint32_t* )malloc(5 * sizeof(uint32_t));
    for (int i = 0; i < 5; ++i) {
        buff_data3[i] = i + 50;
    }
    auto buffer30 = std::make_shared<arrow::Buffer>(bit_map3, 1 * sizeof(uint8_t));
    auto buffer31 = std::make_shared<arrow::Buffer>((uint8_t *)buff_data3, 5 * sizeof(uint32_t));
    std::vector<std::shared_ptr<arrow::Buffer>> buffers3;
    buffers3.emplace_back(buffer30);
    buffers3.emplace_back(buffer31);
    auto array_data3 = arrow::ArrayData::Make(arrow::uint32(), 5, buffers3);
    auto array3 = arrow::MakeArray(array_data3);

    get_heatmap(array1, array2, array3);
}

