/*
 * Wolf Shaper
 * Copyright (C) 2018 Patrick Desaulniers
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"
#include "extra/Mutex.hpp"
#include "extra/ScopedPointer.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

#include "WolfShaperParameters.hpp"
#include "Graph.hpp"
#include "Oversampler.hpp"
#include "Mathf.hpp"
#include "ParamSmooth.hpp"

#include "DspFilters/Dsp.h"
#include "Libs/TAL-Filter-2/FilterHandler.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class WolfShaper : public Plugin
{
  public:
	constexpr static float MIN_CUTOFF = 0.001f;
	constexpr static float MAX_CUTOFF = 1.0f;
	constexpr static float MIN_RESONANCE = 0.001f;
	constexpr static float MAX_RESONANCE = 0.95f;

	WolfShaper() : Plugin(paramCount, 0, 1),
				   oversampler(),
				   preFilterHandlerL(getSampleRate()),
				   preFilterHandlerR(getSampleRate()),
				   postFilterHandlerL(getSampleRate()),
				   postFilterHandlerR(getSampleRate()),
				   mustCopyLineEditor(false),
				   inputIndicatorPos(0.0f),
				   inputIndicatorAcceleration(0.0f),
				   removeDCPrev{0.f, 0.f}
	{
	}

  protected:
	const char *getLabel() const noexcept override
	{
		return "Wolf Shaper";
	}

	const char *getDescription() const noexcept override
	{
		return "Waveshaping distortion plugin with a spline-based graph.";
	}

	const char *getMaker() const noexcept override
	{
		return "Patrick Desaulniers";
	}

	const char *getHomePage() const noexcept override
	{
		return "https://github.com/pdesaulniers/wolf-shaper";
	}

	const char *getLicense() const noexcept override
	{
		return "GPL v3+";
	}

	uint32_t getVersion() const noexcept override
	{
		return d_version(0, 1, 0);
	}

	int64_t getUniqueId() const noexcept override
	{
		return d_cconst('s', 'W', 's', 'p');
	}

	void initParameter(uint32_t index, Parameter &parameter) override
	{
		if (index >= paramCount)
			return;

		switch (index)
		{
		case paramPreGain:
			parameter.name = "Pre Gain";
			parameter.symbol = "pregain";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 2.0f;
			parameter.ranges.def = 1.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsLogarithmic;
			break;
		case paramWet:
			parameter.name = "Wet";
			parameter.symbol = "wet";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 1.0f;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPostGain:
			parameter.name = "Post Gain";
			parameter.symbol = "postgain";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 1.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsLogarithmic;
			break;
		case paramRemoveDC:
			parameter.name = "Remove DC Offset";
			parameter.symbol = "removedc";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 1.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsBoolean | kParameterIsInteger;
			break;
		case paramOversample:
			//None, 2x, 4x, 8x, 16x
			parameter.name = "Oversample";
			parameter.symbol = "oversample";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 4.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
			break;
		case paramBipolarMode:
			parameter.name = "Bipolar Mode";
			parameter.symbol = "bipolarmode";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsBoolean | kParameterIsInteger;
			break;
		case paramHorizontalWarpType:
			//None, Bend +, Bend -, Bend +/-, Skew +, Skew -, Skew +/-
			parameter.name = "H Warp Type";
			parameter.symbol = "warptype";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 6.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
			break;
		case paramHorizontalWarpAmount:
			parameter.name = "H Warp Amount";
			parameter.symbol = "warpamount";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramVerticalWarpType:
			//None, Bend +, Bend -, Bend +/-, Skew +, Skew -, Skew +/-
			parameter.name = "V Warp Type";
			parameter.symbol = "vwarptype";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 6.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
			break;
		case paramVerticalWarpAmount:
			parameter.name = "V Warp Amount";
			parameter.symbol = "vwarpamount";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPreFilterOn:
			parameter.name = "Pre Filter On";
			parameter.symbol = "prefilteron";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsBoolean | kParameterIsInteger;
			break;
		case paramPreFilterType:
			parameter.ranges.min = 0;
			parameter.ranges.max = 6;
			parameter.ranges.def = 0;
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
			parameter.name = "Pre Filter Type";
			parameter.symbol = "prefiltertype";
			parameter.enumValues.count = parameter.ranges.max + 1;
			parameter.enumValues.restrictedMode = true;
			{
				ParameterEnumerationValue *const values = new ParameterEnumerationValue[parameter.enumValues.count];
				parameter.enumValues.values = values;
				values[0].label = "Lp24db";
				values[0].value = (float)FilterType::Lp24db;
				values[1].label = "Lp18db";
				values[1].value = (float)FilterType::Lp18db;
				values[2].label = "Lp12db";
				values[2].value = (float)FilterType::Lp12db;
				values[3].label = "Lp06db";
				values[3].value = (float)FilterType::Lp06db;
				values[4].label = "Hp24db";
				values[4].value = (float)FilterType::Hp24db;
				values[5].label = "Bp24db";
				values[5].value = (float)FilterType::Bp24db;
				values[6].label = "N24db";
				values[6].value = (float)FilterType::N24db;
			}
			break;
		case paramPreFilterCutoff:
			parameter.name = "Pre Filter Cutoff";
			parameter.symbol = "prefiltercutoff";
			parameter.ranges.min = MIN_CUTOFF;
			parameter.ranges.max = MAX_CUTOFF;
			parameter.ranges.def = MAX_CUTOFF;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPreFilterResonance:
			parameter.name = "Pre Filter Resonance";
			parameter.symbol = "prefilterresonance";
			parameter.ranges.min = MIN_RESONANCE;
			parameter.ranges.max = MAX_RESONANCE;
			parameter.ranges.def = MIN_RESONANCE;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPreFilterPreGain:
			//TODO
			break;
		case paramPreFilterWet:
			//TODO
			break;
		case paramPreFilterPostGain:
			//TODO
			break;
		case paramPostFilterOn:
			parameter.name = "Post Filter On";
			parameter.symbol = "postfilteron";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 1.0f;
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsBoolean | kParameterIsInteger;
			break;
		case paramPostFilterType:
			parameter.ranges.min = 0;
			parameter.ranges.max = 6;
			parameter.ranges.def = 0;
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
			parameter.name = "Post Filter Type";
			parameter.symbol = "postfiltertype";
			parameter.enumValues.count = parameter.ranges.max + 1;
			parameter.enumValues.restrictedMode = true;
			{
				ParameterEnumerationValue *const values = new ParameterEnumerationValue[parameter.enumValues.count];
				parameter.enumValues.values = values;
				values[0].label = "Lp24db";
				values[0].value = (float)FilterType::Lp24db;
				values[1].label = "Lp18db";
				values[1].value = (float)FilterType::Lp18db;
				values[2].label = "Lp12db";
				values[2].value = (float)FilterType::Lp12db;
				values[3].label = "Lp06db";
				values[3].value = (float)FilterType::Lp06db;
				values[4].label = "Hp24db";
				values[4].value = (float)FilterType::Hp24db;
				values[5].label = "Bp24db";
				values[5].value = (float)FilterType::Bp24db;
				values[6].label = "N24db";
				values[6].value = (float)FilterType::N24db;
			}
			break;
		case paramPostFilterCutoff:
			parameter.name = "Post Filter Cutoff";
			parameter.symbol = "postfiltercutoff";
			parameter.ranges.min = MIN_CUTOFF;
			parameter.ranges.max = MAX_CUTOFF;
			parameter.ranges.def = MAX_CUTOFF;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPostFilterResonance:
			parameter.name = "Post Filter Resonance";
			parameter.symbol = "postfilterresonance";
			parameter.ranges.min = MIN_RESONANCE;
			parameter.ranges.max = MAX_RESONANCE;
			parameter.ranges.def = MIN_RESONANCE;
			parameter.hints = kParameterIsAutomable;
			break;
		case paramPostFilterPreGain:
			break;
		case paramPostFilterWet:
			break;
		case paramPostFilterPostGain:
			break;
		case paramOut:
			parameter.name = "Out";
			parameter.symbol = "out";
			parameter.hints = kParameterIsOutput;
			parameter.ranges.def = 0.0f;
			break;
		}

		parameters[index] = ParamSmooth(parameter.ranges.def);
		parameters[index].calculateCoeff(20.f, getSampleRate());
	}

	float getParameterValue(uint32_t index) const override
	{
		return parameters[index].getRawValue();
	}

	int getOversamplingRatio()
	{
		return std::pow(2, std::round(parameters[paramOversample].getRawValue()));
	}

	void setParameterValue(uint32_t index, float value) override
	{
		parameters[index].setValue(value);

		if (index == paramOversample)
		{
			for (int i = 0; i < paramCount; ++i)
			{
				//these parameters are used out of the oversampled DSP, so we skip them
				if (i == paramPreFilterCutoff || i == paramPreFilterResonance || i == paramPostFilterCutoff || i == paramPostFilterResonance)
					continue;

				parameters[i].calculateCoeff(20.f, getSampleRate() * getOversamplingRatio());
			}
		}
	}

	void initState(uint32_t index, String &stateKey, String &defaultStateValue) override
	{
		switch (index)
		{
		case 0:
			stateKey = "graph";
			break;
		}

		//generated with fprintf(stderr, "%A,%A,%A,%d;%A,%A,%A,%d;\n", 0.0f, 0.0f, 0.0f, wolf::CurveType::Exponential, 1.0f, 1.0f, 0.0f, wolf::CurveType::Exponential);
		defaultStateValue = String("0x0p+0,0x0p+0,0x0p+0,0;0x1p+0,0x1p+0,0x0p+0,0;");
	}

	void setState(const char *key, const char *value) override
	{
		const MutexLocker cml(mutex);

		if (std::strcmp(key, "graph") == 0)
		{
			tempLineEditor.rebuildFromString(value);
			mustCopyLineEditor = true;
		}
	}

	float removeDCOffset(float input, int channel)
	{
		//Steep IIR filter at the DC frequency

		const float scaleFactor = 0.9999f; //Closer to 1 means steeper stop band

		const float value = input + scaleFactor * removeDCPrev[channel];
		const float result = value - removeDCPrev[channel];

		removeDCPrev[channel] = value;

		return result;
	}

	float calculateValueOutsideGraph(float value)
	{
		const bool bipolarMode = lineEditor.getBipolarMode();

		if (bipolarMode)
		{
			const bool positiveInput = value >= 0.0f;
			const float vertexY = lineEditor.getVertexAtIndex(positiveInput ? lineEditor.getVertexCount() - 1 : 0)->getY();
			const float absValue = std::abs(value);

			return absValue * (-1.0f + vertexY * 2.0f);
		}
		else
		{
			return value * lineEditor.getVertexAtIndex(lineEditor.getVertexCount() - 1)->getY();
		}
	}

	float getGraphValue(float input)
	{
		const float absInput = std::abs(input);

		if (absInput > 1.0f)
		{
			return calculateValueOutsideGraph(input);
		}

		const bool bipolarMode = lineEditor.getBipolarMode();

		if (bipolarMode)
		{
			const float x = (1.0f + input) * 0.5f;
			return -1.0f + lineEditor.getValueAt(x) * 2.0f;
		}
		else
		{
			return lineEditor.getValueAt(input);
		}
	}

	void updateInputIndicatorPos(const float peak, const uint32_t frames)
	{
		const float deadZone = 0.001f;
		const float speed = 0.35f;

		if (peak > deadZone && peak > inputIndicatorPos)
		{
			inputIndicatorPos = peak;
			inputIndicatorAcceleration = 0.0f;
		}
		else if (inputIndicatorPos > -deadZone)
		{
			inputIndicatorPos -= inputIndicatorAcceleration * frames;
			inputIndicatorAcceleration += std::pow(inputIndicatorAcceleration + speed / getSampleRate(), 2) * frames;
		}

		inputIndicatorPos = wolf::clamp(inputIndicatorPos, -deadZone, 1.0f);
	}

	void run(const float **inputs, float **outputs, uint32_t frames) override
	{
		if (mutex.tryLock())
		{
			if (mustCopyLineEditor)
			{
				lineEditor = tempLineEditor;

				for (int i = 0; i < lineEditor.getVertexCount(); ++i)
				{
					lineEditor.getVertexAtIndex(i)->setGraphPtr(&lineEditor);
				}

				mustCopyLineEditor = false;
			}
		}

		float peak = 0.0f;

		//pre filtering
		const bool preFilterOn = std::round(parameters[paramPreFilterOn].getRawValue());

		if (preFilterOn)
		{
			preFilterHandlerL.setFiltertype(parameters[paramPreFilterType].getRawValue());
			preFilterHandlerR.setFiltertype(parameters[paramPreFilterType].getRawValue());

			for (uint32_t i = 0; i < frames; ++i)
			{
				const float preCutoff = parameters[paramPreFilterCutoff].getSmoothedValue();
				const float preResonance = parameters[paramPreFilterResonance].getSmoothedValue();

				const float scaledCutoff = wolf::logScale(preCutoff, MIN_CUTOFF, MAX_CUTOFF);
				const float scaledResonance = wolf::logScale(preResonance, MIN_RESONANCE, MAX_RESONANCE);

				outputs[0][i] = preFilterHandlerL.process(inputs[0][i], scaledCutoff, scaledResonance);
				outputs[1][i] = preFilterHandlerR.process(inputs[1][i], scaledCutoff, scaledResonance);
			}
		}

		//distortion
		const int oversamplingRatio = getOversamplingRatio();
		uint32_t numSamples = frames * oversamplingRatio;

		const double sampleRate = getSampleRate();

		float **buffer;

		if (preFilterOn) //the input is now in the output, so we oversample the output
		{
			buffer = oversampler.upsample(oversamplingRatio, frames, sampleRate, outputs);
		}
		else
		{
			buffer = oversampler.upsample(oversamplingRatio, frames, sampleRate, inputs);
		}

		wolf::WarpType horizontalWarpType = (wolf::WarpType)std::round(parameters[paramHorizontalWarpType].getRawValue());
		lineEditor.setHorizontalWarpType(horizontalWarpType);

		wolf::WarpType verticalWarpType = (wolf::WarpType)std::round(parameters[paramVerticalWarpType].getRawValue());
		lineEditor.setVerticalWarpType(verticalWarpType);

		const bool mustRemoveDC = parameters[paramRemoveDC].getRawValue() > 0.50f;

		if (!mustRemoveDC)
		{
			removeDCPrev[0] = 0.f;
			removeDCPrev[1] = 0.f;
		}

		for (uint32_t i = 0; i < numSamples; ++i)
		{
			lineEditor.setHorizontalWarpAmount(parameters[paramHorizontalWarpAmount].getSmoothedValue());
			lineEditor.setVerticalWarpAmount(parameters[paramVerticalWarpAmount].getSmoothedValue());

			const float preGain = parameters[paramPreGain].getSmoothedValue();

			float inputL = preGain * buffer[0][i];
			float inputR = preGain * buffer[1][i];

			//this hack makes sure that dc offset in the graph doesn't oscillate... hopefully
			if (inputL < 0.0f && inputL > -0.00001f)
			{
				inputL = 0.0f;
			}
			if (inputR < 0.0f && inputR > -0.00001f)
			{
				inputR = 0.0f;
			}

			const float absL = std::abs(inputL);
			const float absR = std::abs(inputR);

			peak = std::max(peak, absL);
			peak = std::max(peak, absR);

			const bool bipolarMode = parameters[paramBipolarMode].getRawValue() > 0.50f;
			lineEditor.setBipolarMode(bipolarMode);

			float graphL, graphR;

			graphL = getGraphValue(inputL);
			graphR = getGraphValue(inputR);

			const float wet = parameters[paramWet].getSmoothedValue();
			const float dry = 1.0f - wet;
			const float postGain = parameters[paramPostGain].getSmoothedValue();

			buffer[0][i] = (dry * inputL + wet * graphL) * postGain;
			buffer[1][i] = (dry * inputR + wet * graphR) * postGain;

			if (mustRemoveDC)
			{
				buffer[0][i] = removeDCOffset(buffer[0][i], 0);
				buffer[1][i] = removeDCOffset(buffer[1][i], 1);
			}
		}

		oversampler.downsample(outputs);

		updateInputIndicatorPos(peak, frames);
		setParameterValue(paramOut, inputIndicatorPos);

		//post filtering

		const bool postFilterOn = std::round(parameters[paramPostFilterOn].getRawValue());

		if (postFilterOn)
		{
			postFilterHandlerL.setFiltertype(parameters[paramPostFilterType].getRawValue());
			postFilterHandlerR.setFiltertype(parameters[paramPostFilterType].getRawValue());

			for (uint32_t i = 0; i < frames; ++i)
			{
				const float postCutoff = parameters[paramPostFilterCutoff].getSmoothedValue();
				const float postResonance = parameters[paramPostFilterResonance].getSmoothedValue();

				const float scaledCutoff = wolf::logScale(postCutoff, MIN_CUTOFF, MAX_CUTOFF);
				const float scaledResonance = wolf::logScale(postResonance, MIN_RESONANCE, MAX_RESONANCE);

				outputs[0][i] = postFilterHandlerL.process(outputs[0][i], scaledCutoff, scaledResonance);
				outputs[1][i] = postFilterHandlerR.process(outputs[1][i], scaledCutoff, scaledResonance);
			}
		}

		mutex.unlock();
	}

  private:
	ParamSmooth parameters[paramCount];
	Oversampler oversampler;

	FilterHandler preFilterHandlerL;
	FilterHandler preFilterHandlerR;
	FilterHandler postFilterHandlerL;
	FilterHandler postFilterHandlerR;

	wolf::Graph lineEditor;
	wolf::Graph tempLineEditor;
	bool mustCopyLineEditor;

	float inputIndicatorPos;
	float inputIndicatorAcceleration;

	Mutex mutex;

	float removeDCPrev[2];

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WolfShaper)
};

Plugin *createPlugin()
{
	return new WolfShaper();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
