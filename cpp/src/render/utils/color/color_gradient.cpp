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
#include <string>

#include "render/utils/color/color_gradient.h"

#define WHITE 0xFFFFFF;
#define YELLOW 0xFFFF00;    //  :255,255,0
#define ORANGE 0xFF7D00;    //  :255,125,0
#define PURPLE 0xFF00FF;    //  :255,0,25
#define RED 0xFF0000;       //  :255,0,0
#define CYANBLUE 0x00FFFF;  //  :0,255,255
#define GREEN 0x00FF00;     //  :0,255,0
#define BLUE 0x0000FF;      //  :0,0,255
#define SKYBLUE 0xB4E7F5;

#define TRANSPARENCY 0.5f

namespace zilliz {
namespace render {

void ColorGradient::createDefaultHeatMapGradient() {
  color.clear();
  color.push_back(ColorPoint(0, 0, 1, 0.0f));   // Blue.
  color.push_back(ColorPoint(0, 1, 1, 0.25f));  // Cyan.
  color.push_back(ColorPoint(0, 1, 0, 0.5f));   // Green.
  color.push_back(ColorPoint(1, 1, 0, 0.75f));  // Yellow.
  color.push_back(ColorPoint(1, 0, 0, 1.0f));   // Red.
}

void ColorGradient::getColorAtValue(const float value, float& red, float& green,
                                    float& blue) {
  if (color.size() == 0) return;

  for (unsigned int i = 0; i < color.size(); i++) {
    ColorPoint& curr_color = color[i];
    if (value < curr_color.val) {
      int index = (i - 1) > 0 ? i - 1 : 0;
      ColorPoint& prev_color = color[index];
      float value_diff = (prev_color.val - curr_color.val);
      float fract_between = (value_diff == 0) ? 0 : (value - curr_color.val) / value_diff;
      red = (prev_color.r - curr_color.r) * fract_between + curr_color.r;
      green = (prev_color.g - curr_color.g) * fract_between + curr_color.g;
      blue = (prev_color.b - curr_color.b) * fract_between + curr_color.b;
      return;
    }
  }
  red = color.back().r;
  green = color.back().g;
  blue = color.back().b;
  return;
}

void HexToRGB(int64_t color, CircleParams& circle_params_2d, ColorStyle color_style) {
  switch (color_style) {
    case ColorStyle::kBlueToRed:
    case ColorStyle::kPurpleToYellow:
    case ColorStyle::kSkyBlueToWhite:
      circle_params_2d.color.r = (color >> 16 & 0xff) / 255.0f;
      circle_params_2d.color.g = (color >> 8 & 0xff) / 255.0f;
      circle_params_2d.color.b = (color & 0xff) / 255.0f;
      break;
    case ColorStyle::kBlueTransParency:
    case ColorStyle::kRedTransParency: {
      circle_params_2d.color.r = (color >> 16 & 0xff) / 255.0f;
      circle_params_2d.color.g = (color >> 8 & 0xff) / 255.0f;
      circle_params_2d.color.b = (color & 0xff) / 255.0f;
      break;
    }
    default: {
      std::string msg = "cannot find color style";
      // TODO: add log here
      break;
    }
  }
}

CircleParams ColorGradient::GetCircleParams(zilliz::render::ColorStyle color_style,
                                            double ratio) {
  CircleParams circle_params_2d;

  switch (color_style) {
    case ColorStyle::kBlueToRed: {
      circle_params_2d.color.a = 1.0;
      circle_params_2d.color.r = ratio;
      circle_params_2d.color.g = 0.0;
      circle_params_2d.color.b = 1 - ratio;
      break;
    }
    case ColorStyle::kPurpleToYellow: {
      circle_params_2d.color.a = 1.0;
      circle_params_2d.color.r = 1.0;
      circle_params_2d.color.g = ratio;
      circle_params_2d.color.b = 1 - ratio;
      break;
    }
    case ColorStyle::kSkyBlueToWhite: {
      int64_t sky_blue = SKYBLUE;
      circle_params_2d.color.a = 1.0;
      circle_params_2d.color.r = ((255 - (sky_blue >> 16 & 0xff)) * ratio) / 255.0f;
      circle_params_2d.color.g = ((255 - (sky_blue >> 8 & 0xff)) * ratio) / 255.0f;
      circle_params_2d.color.b = ((255 - (sky_blue & 0xff)) * ratio) / 255.0f;
      break;
    }
    case ColorStyle::kRedTransParency: {
      int64_t red_lp = RED;
      circle_params_2d.color.a = ratio + TRANSPARENCY;

      HexToRGB(red_lp, circle_params_2d, color_style);
      break;
    }
    case ColorStyle::kBlueTransParency: {
      int64_t blue_lp = BLUE;
      circle_params_2d.color.a = ratio + TRANSPARENCY;

      HexToRGB(blue_lp, circle_params_2d, color_style);
      break;
    }
    case ColorStyle::kBlueGreenYellow: {
      circle_params_2d.color.r = (17.0f + (208.0f - 17.0f) * ratio) / 255.0f;
      circle_params_2d.color.g = (95.0f + (244.0f - 95.0f) * ratio) / 255.0f;
      circle_params_2d.color.b = (154.0f * (1.0f - ratio)) / 255.0f;
      circle_params_2d.color.a = 1.0;
      break;
    }
    case ColorStyle::kWhiteToBlue: {
      circle_params_2d.color.r = (226.0f + (17.0f - 226.0f) * ratio) / 255.0f;
      circle_params_2d.color.g = (226.0f + (95.0f - 226.0f) * ratio) / 255.0f;
      circle_params_2d.color.b = (226.0f + (154.0f - 226.0f) * ratio) / 255.0f;
      circle_params_2d.color.a = 1.0;
      break;
    }
    case ColorStyle::kGreenYellowRed: {
      circle_params_2d.color.r = (77.0f + (194.0f - 77.0f) * ratio) / 255.0f;
      circle_params_2d.color.g = (144.0f + (55.0f - 144.0f) * ratio) / 255.0f;
      circle_params_2d.color.b = (79.0f + (40.0f - 79.0f) * ratio) / 255.0f;
      circle_params_2d.color.a = 1.0;
      break;
    }
    case ColorStyle::kBlueWhiteRed: {
      circle_params_2d.color.r = (25.0f + (194.0f - 25.0f) * ratio) / 255.0f;
      circle_params_2d.color.g = (132.0f + (55.0f - 132.0f) * ratio) / 255.0f;
      circle_params_2d.color.b = (197.0f + (40.0f - 197.0f) * ratio) / 255.0f;
      circle_params_2d.color.a = 1.0;
      break;
    }
    default: {
      std::string msg = "cannot find color style";
      // TODO: add log here
      break;
    }
  }
  return circle_params_2d;
}

}  // namespace render
}  // namespace zilliz
