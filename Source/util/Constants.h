/*
Copyright (C) 2020 Phillip Lamp
This file is part of the SMPLComp project <https://github.com/p-hlp/SMPLComp>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

/*
  ==============================================================================
    File:           Constants.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once
#include <cstdint>

namespace Constants
{
    constexpr uint32_t BaseSampleRate = 44100;
    constexpr uint32_t BaseSamplesPerBlock = 480;

    static enum RadioGroups {
        AllPassEn = 1001, 
        AllPassMode = 1002
    };

    namespace LAF
    {
        constexpr uint8_t ButtonHeight = 30;
    }

    namespace Colors
    {
        //Backgrounds
        constexpr uint32_t bg_App{0xff424242};
        constexpr uint32_t bg_DarkGrey{0xff212121};
        constexpr uint32_t bg_MidGrey{0xff616161};
        constexpr uint32_t bg_LightGrey{0xff9e9e9e};

        //Gradients
        constexpr uint32_t gd_LightGreyStart{0xffeeeee};
        constexpr uint32_t gd_LightGreyEnd{0xffc4c4c4};
        constexpr uint32_t gd_DarktGreyStart{0xff646464};
        constexpr uint32_t gd_DarkGreyEnd{0xff464646};

        //Misc
        constexpr uint32_t statusOutline{0xfffb8c00};
        constexpr uint32_t knobShadow{0xff424242};
    }

    namespace Margins
    {
        constexpr float small = 2.0f;
        constexpr float medium = 4.0f;
        constexpr float big = 8.0f;
    }

    namespace Containers
    {
        constexpr float ButtonHeight = 88.0f;
        constexpr float ButtonWidth = 96.0f;
        constexpr float ToggleHeight = 30.0f;

        // Width enough for buttons and margins plus extra margins to widen the visualizer -KGK
        constexpr float WindowWidth = Constants::Containers::ButtonWidth * 4 + Constants::Margins::big * 2 + Constants::Margins::big * 4;
        constexpr float WindowHeight = Constants::Containers::ButtonHeight * 3 + Constants::Margins::big * 2;

        constexpr float PopupWidth = Constants::Containers::ButtonWidth * 3 + Constants::Margins::big * 2;
        constexpr float PopupHeight = Constants::Containers::ButtonHeight * 3 + Constants::Margins::big * 2;
    }

    namespace Parameter
    {
        // Compressor
        constexpr float inputStart = -30.0f;
        constexpr float inputEnd = 30.0f;
        constexpr float inputInterval = 0.1f;

        constexpr float thresholdStart = -60.0f;
        constexpr float thresholdEnd = 0.0f;
        constexpr float thresholdInterval = 0.1f;

        constexpr float ratioStart = 1.0f;
        constexpr float ratioEnd = 24.0f;
        constexpr float ratioInterval = 0.05f;

        constexpr float kneeStart = 0.0f;
        constexpr float kneeEnd = 24.0f;
        constexpr float kneeInterval = 0.1f;

        constexpr float attackStart = 0.0f;
        constexpr float attackEnd = 100.0f;
        constexpr float attackInterval = 0.01f;

        constexpr float releaseStart = 5.0f;
        constexpr float releaseEnd = 1500.0f;
        constexpr float releaseInterval = 0.01f;

        constexpr float makeupStart = -30.0f;
        constexpr float makeupEnd = 30.0f;
        constexpr float makeupInterval = 0.1f;

        constexpr float mixStart = 0.0f;
        constexpr float mixEnd = 1.0f;
        constexpr float mixInterval = 0.001f;
    }
}
