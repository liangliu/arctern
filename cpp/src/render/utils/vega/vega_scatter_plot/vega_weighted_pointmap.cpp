/*
 * Copyright (C) 2019-2020 Zilliz. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "render/utils/vega/vega_scatter_plot/vega_weighted_pointmap.h"

namespace arctern {
namespace render {

VegaWeightedPointmap::VegaWeightedPointmap(const std::string& json) { Parse(json); }

void VegaWeightedPointmap::Parse(const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (document.Parse(json.c_str()).HasParseError()) {
    // TODO: add log here
    printf("json format error\n");
    is_valid_ = false;
    return;
  }

  if (!JsonLabelCheck(document, "width") || !JsonLabelCheck(document, "height") ||
      !JsonNullCheck(document["width"]) || !JsonNullCheck(document["height"]) ||
      !JsonTypeCheck(document["width"], rapidjson::Type::kNumberType) ||
      !JsonTypeCheck(document["height"], rapidjson::Type::kNumberType)) {
    return;
  }
  window_params_.mutable_width() = document["width"].GetInt();
  window_params_.mutable_height() = document["height"].GetInt();

  if (!JsonLabelCheck(document, "marks") ||
      !JsonTypeCheck(document["marks"], rapidjson::Type::kArrayType) ||
      !JsonSizeCheck(document["marks"], "marks", 1) ||
      !JsonLabelCheck(document["marks"][0], "encode") ||
      !JsonLabelCheck(document["marks"][0]["encode"], "enter")) {
    return;
  }
  rapidjson::Value mark_enter;
  mark_enter = document["marks"][0]["encode"]["enter"];

  // parse color style
  if (!JsonLabelCheck(mark_enter, "strokeWidth") ||
      !JsonLabelCheck(mark_enter, "opacity") ||
      !JsonLabelCheck(mark_enter["strokeWidth"], "value") ||
      !JsonLabelCheck(mark_enter["opacity"], "value") ||
      !JsonTypeCheck(mark_enter["strokeWidth"]["value"], rapidjson::Type::kNumberType) ||
      !JsonTypeCheck(mark_enter["opacity"]["value"], rapidjson::Type::kNumberType)) {
    return;
  }
  circle_params_.radius = mark_enter["strokeWidth"]["value"].GetInt();
  circle_params_.color.a = mark_enter["opacity"]["value"].GetDouble();

  // parse color style
  if (!JsonLabelCheck(mark_enter, "color_style") ||
      !JsonLabelCheck(mark_enter["color_style"], "value") ||
      !JsonTypeCheck(mark_enter["color_style"]["value"], rapidjson::Type::kStringType)) {
    return;
  }
  auto color_style_string = std::string(mark_enter["color_style"]["value"].GetString());
  if (color_style_string == "blue_to_red") {
    color_style_ = ColorStyle::kBlueToRed;
  } else if (color_style_string == "skyblue_to_white") {
    color_style_ = ColorStyle::kSkyBlueToWhite;
  } else if (color_style_string == "purple_to_yellow") {
    color_style_ = ColorStyle::kPurpleToYellow;
  } else if (color_style_string == "red_transparency") {
    color_style_ = ColorStyle::kRedTransParency;
  } else if (color_style_string == "blue_transparency") {
    color_style_ = ColorStyle::kBlueTransParency;
  } else if (color_style_string == "blue_green_yellow") {
    color_style_ = ColorStyle::kBlueGreenYellow;
  } else if (color_style_string == "white_blue") {
    color_style_ = ColorStyle::kWhiteToBlue;
  } else if (color_style_string == "blue_white_red") {
    color_style_ = ColorStyle::kBlueWhiteRed;
  } else if (color_style_string == "green_yellow_red") {
    color_style_ = ColorStyle::kGreenYellowRed;
  } else {
    std::string msg = "unsupported color style '" + color_style_string + "'.";
    // TODO: add log here
  }

  // parse ruler
  if (!JsonLabelCheck(mark_enter, "ruler") ||
      !JsonLabelCheck(mark_enter["ruler"], "value") ||
      !JsonTypeCheck(mark_enter["ruler"]["value"], rapidjson::Type::kArrayType) ||
      !JsonSizeCheck(mark_enter["ruler"]["value"], "ruler.value", 2)) {
    return;
  }
  for (int i = 0; i < 2; i++) {
    if (!JsonTypeCheck(mark_enter["ruler"]["value"][i], rapidjson::Type::kNumberType)) {
      return;
    }
  }
  ruler_ = std::make_pair(mark_enter["ruler"]["value"][0].GetDouble(),
                          mark_enter["ruler"]["value"][1].GetDouble());
}

}  // namespace render
}  // namespace arctern
