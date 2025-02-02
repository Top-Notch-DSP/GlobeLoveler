/*
Copyright (C) 2020 Phillip Lamp
This file is part of the SMPLComp project <https://github.com/p-hlp/SMPLComp>.
Modified 2024 by Kris Keillor

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
*/

/*
  ==============================================================================
    File:           SMPLCompStandaloneLookAndFeel.cpp
    Developers:     D. Robert Hoover and Kris Keillor
    Repository URL: https://github.com/Top-Notch-DSP/GlobeLoveler
    Date:           2024 Feb 1
    Forked From:    p-hlp
    Original URL:   https://github.com/p-hlp/SMPLComp/tree/master
    License:        GNU General Public License, version 3.0 (GPL-3.0)
  ==============================================================================
*/

#pragma once
#include "../../util/Constants.h"
#include "../JuceLibraryCode/JuceHeader.h"
//#include "juce_gui_basics/buttons/juce_ToolbarButton.h"

class GlobeLovelerLookAndFeel : public LookAndFeel_V4
{
private:
    //Vars
    Typeface::Ptr robotoLight, robotoRegular, robotoMedium, robotoBold;

    //Color objects
    Colour backgroundDarkGrey, backgroundDarkGreyDisabled;
    Colour backgroundMidGrey;
    Colour statusOutline, statusOutlineDisabled;
    Colour gradientDarkGrey, gradientDarkGreyDisabled;

public:
    class GlobeLoveler_LookAndFeel_DocumentWindowButton final : public Button
    {
    public:
        GlobeLoveler_LookAndFeel_DocumentWindowButton(const String& name, Colour c, const Path& normal, const Path& toggled)
            : Button(name), colour(c), normalShape(normal), toggledShape(toggled)
        {
        }

        void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
        {
            auto background = Colours::grey;

            if (auto* rw = findParentComponentOfClass<ResizableWindow>())
                if (auto lf = dynamic_cast<LookAndFeel_V4*> (&rw->getLookAndFeel()))
                    background = lf->getCurrentColourScheme().getUIColour(LookAndFeel_V4::ColourScheme::widgetBackground);

            g.fillAll(background);

            g.setColour((!isEnabled() || shouldDrawButtonAsDown) ? colour.withAlpha(0.6f)
                : colour);

            if (shouldDrawButtonAsHighlighted)
            {
                g.fillAll();
                g.setColour(background);
            }

            auto& p = getToggleState() ? toggledShape : normalShape;

            auto reducedRect = Justification(Justification::centred)
                .appliedToRectangle(Rectangle<int>(getHeight(), getHeight()), getLocalBounds())
                .toFloat()
                .reduced((float)getHeight() * 0.3f);

            g.fillPath(p, p.getTransformToScaleToFit(reducedRect, true));
        }

    private:
        Colour colour;
        Path normalShape, toggledShape;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobeLoveler_LookAndFeel_DocumentWindowButton)
    };

    GlobeLovelerLookAndFeel()
    {
        robotoLight = Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize);
        robotoRegular = Typeface::createSystemTypefaceFor(BinaryData::RobotoRegular_ttf,
                                                          BinaryData::RobotoRegular_ttfSize);
        robotoMedium = Typeface::
            createSystemTypefaceFor(BinaryData::RobotoMedium_ttf, BinaryData::RobotoMedium_ttfSize);
        robotoBold = Typeface::createSystemTypefaceFor(BinaryData::RobotoBold_ttf, BinaryData::RobotoBold_ttfSize);

        backgroundDarkGrey = Colour(Constants::Colors::bg_DarkGrey);
        backgroundDarkGreyDisabled = Colour(Constants::Colors::bg_DarkGrey).withAlpha(0.35f);
        backgroundMidGrey = Colour(Constants::Colors::bg_MidGrey);
        statusOutline = Colour(Constants::Colors::statusOutline);
        statusOutlineDisabled = Colour(Constants::Colors::statusOutline).withAlpha(0.35f);
        gradientDarkGrey = Colour(Constants::Colors::gd_DarktGreyStart);
        gradientDarkGreyDisabled = Colour(Constants::Colors::gd_DarktGreyStart).withAlpha(0.35f);
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        const float radius = static_cast<float>(jmin(width / 2, height / 2));
        const float centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
        const float centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;

        const float min = static_cast<float>(slider.getMinimum());
        const float max = static_cast<float>(slider.getMaximum());
        const float zeroPos = -min / (max - min);
        const float zeroAngle = rotaryStartAngle + zeroPos * (rotaryEndAngle - rotaryStartAngle);
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        const float bedThickness = 2.0f;
        const float bedOutline = 1.4f;
        const float statusOutlineThickness = 2.5f;
        const float extraMargin = 2.0f;

        const float pointerThickness = 3.0f;
        const float pointerLength = (radius - extraMargin - statusOutlineThickness - bedOutline - bedThickness - 1.0f) *
            0.8f;

        Path p, q, a, h;
        Rectangle<float> rect = Rectangle<float>(rx, ry, rw, rw);

        //status bg
        slider.isEnabled() ? g.setColour(backgroundDarkGrey) : g.setColour(backgroundDarkGreyDisabled);
        a.addCentredArc(centreX, centreY, radius - extraMargin, radius - extraMargin, 0.0f, rotaryStartAngle,
                        rotaryEndAngle, true);
        g.strokePath(a, PathStrokeType(3.5f));

        //status ring
        slider.isEnabled() ? g.setColour(statusOutline) : g.setColour(statusOutlineDisabled);
        h.addCentredArc(centreX, centreY, radius - extraMargin, radius - extraMargin, 0.0f, zeroAngle, angle, true);
        g.strokePath(h, PathStrokeType(3.5f));

        //main knob
        slider.isEnabled()
            ? g.setGradientFill(ColourGradient(gradientDarkGrey, 0, 0, gradientDarkGrey, 0, rw, false))
            : g.setColour(gradientDarkGreyDisabled);
        g.fillEllipse(rect.reduced(extraMargin + statusOutlineThickness + bedOutline + bedThickness));

        //knob pointer
        p.addRectangle(-pointerThickness * 0.5f, -radius + 6.0f, pointerThickness, pointerLength);
        p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
        g.setColour(backgroundDarkGrey);
        g.fillPath(p);
    }


    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const auto bounds = button.getLocalBounds().toFloat();

        g.setColour(button.getToggleState()
                        ? statusOutline.withMultipliedAlpha(0.8f)
                        : backgroundMidGrey);
        g.fillRoundedRectangle(bounds, 2);
    }

    void drawButtonText(Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override
    {
        Font font(getTextButtonFont(button, button.getHeight()));
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState()
                                          ? TextButton::textColourOnId
                                          : TextButton::textColourOffId)
                          .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
        const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

        const int fontHeight = roundToInt(font.getHeight() * 0.6f);
        const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        const int textWidth = button.getWidth() - leftIndent - rightIndent;

        if (textWidth > 0)
            g.drawFittedText(button.getButtonText(),
                             leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                             Justification::centred, 2);
    }

    void drawBubble(Graphics& g, BubbleComponent& b, const Point<float>& tip, const Rectangle<float>& body) override
    {
        g.setColour(backgroundMidGrey);
        g.fillRoundedRectangle(body, 2);
    }


    Font getSliderPopupFont(Slider&) override
    {
        return robotoBold;
    }

    Typeface::Ptr getTypefaceForFont(const Font&) override
    {
        return robotoRegular;
    }

    Font getLabelFont(Label&) override
    {
        return Font(robotoMedium);
    }

    Font getTextButtonFont(TextButton& button, int height) override
    {
        return { jmin(16.0f, (float)buttonHeight * 0.6f) };
        /*Font font(robotoMedium);
        font.setHeight(12);
        return font;*/
    }

    Font getAlertWindowMessageFont() override
    {
        Font font(robotoRegular);
        font.setHeight(14.0f);
        return font;
    }

    void drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW,
                      int buttonH, ComboBox& box) override
    {
        auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 1.0f;
        Rectangle<int> boxBounds(0, 0, width, height);

        g.setColour(backgroundDarkGrey);
        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

        Rectangle<int> arrowZone(width - 30, 0, 20, height);
        Path path;
        path.startNewSubPath(arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
        path.lineTo(static_cast<float>(arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
        path.lineTo(arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

        g.setColour(box.findColour(ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
        g.strokePath(path, PathStrokeType(2.0f));
    }


    void positionComboBoxText(ComboBox& box, Label& label) override
    {
        label.setBounds(1, 1,
                        box.getWidth(),
                        box.getHeight() - 2);

        label.setFont(getComboBoxFont(box));
    }


    void drawPopupMenuBackground(Graphics& g, int width, int height) override
    {
        g.fillAll(backgroundMidGrey);
        ignoreUnused(width, height);

#if ! JUCE_MAC
        g.setColour(backgroundDarkGrey.withAlpha(0.6f));
        g.drawRect(0, 0, width, height);
#endif
    }

    Font getPopupMenuFont() override
    {
        return Font(robotoBold);
    }


    Font getComboBoxFont(ComboBox&) override
    {
        return Font(robotoMedium);
    }

    Button* createDocumentWindowButton(int buttonType) override {
        Path shape;
        auto crossThickness = 0.3f;

        if (buttonType == DocumentWindow::closeButton)
        {
            shape.addLineSegment({ 0.0f, 0.0f, 1.0f, 1.0f }, crossThickness);
            shape.addLineSegment({ 1.0f, 0.0f, 0.0f, 1.0f }, crossThickness);

            return new GlobeLoveler_LookAndFeel_DocumentWindowButton("close", Colour(0xffaf0000), shape, shape);
        }

        if (buttonType == DocumentWindow::minimiseButton)
        {
            shape.addLineSegment({ 0.0f, 0.5f, 1.0f, 0.5f }, crossThickness);

            return new GlobeLoveler_LookAndFeel_DocumentWindowButton("minimise", Colour(0xff00af00), shape, shape);
        }

        if (buttonType == DocumentWindow::maximiseButton)
        {
            shape.addLineSegment({ 0.5f, 0.0f, 0.5f, 1.0f }, crossThickness);
            shape.addLineSegment({ 0.0f, 0.5f, 1.0f, 0.5f }, crossThickness);

            Path fullscreenShape;
            fullscreenShape.startNewSubPath(45.0f, 100.0f);
            fullscreenShape.lineTo(0.0f, 100.0f);
            fullscreenShape.lineTo(0.0f, 0.0f);
            fullscreenShape.lineTo(100.0f, 0.0f);
            fullscreenShape.lineTo(100.0f, 45.0f);
            fullscreenShape.addRectangle(45.0f, 45.0f, 100.0f, 100.0f);
            PathStrokeType(30.0f).createStrokedPath(fullscreenShape, fullscreenShape);

            return new GlobeLoveler_LookAndFeel_DocumentWindowButton("maximise", Colour(0xff0000af), shape, fullscreenShape);
        }

        jassertfalse;
        return nullptr;
    }

    //==============================================================================
    static constexpr int buttonHeight = 25;
};

