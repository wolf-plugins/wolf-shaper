/*
 * wolf shaper
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

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "Graph.hpp"
#include "Oversampler.hpp"
#include "DspFilters/Dsp.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class WaveShaper : public Plugin
{
  public:
	enum Parameters
	{
		paramPreGain = 0,
		paramWet,
		paramPostGain,
		paramRemoveDC,
		paramOversample,
		paramBipolarMode,
		paramOut,
		paramCount
	};

	WaveShaper() : Plugin(paramCount, 0, 1),
				   oversampler()
	{
		parameters[paramPreGain] = 1.0f;
		parameters[paramWet] = 1.0f;
		parameters[paramPostGain] = 1.0f;
		parameters[paramRemoveDC] = 0.0f;
		parameters[paramOversample] = 1.0f;
		parameters[paramBipolarMode] = 0.0f;
		parameters[paramOut] = 0.0f;
	}

  protected:
	const char *getLabel() const noexcept override
	{
		return "Wolf Shaper";
	}

	const char *getDescription() const noexcept override
	{
		return "Waveshaping distortion plugin with spline-based graph.";
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
		return "ISC";
	}

	uint32_t getVersion() const noexcept override
	{
		return d_version(0, 0, 1);
	}

	int64_t getUniqueId() const noexcept override
	{
		return d_cconst('s', 'W', 's', 'p');
	}

	void initParameter(uint32_t index, Parameter &parameter) override
	{
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
			parameter.ranges.def = 0.0f;
			parameter.hints = kParameterIsAutomable | kParameterIsBoolean | kParameterIsInteger;
			break;
		case paramOversample:
			//None, 2x, 4x, 8x, 16x
			parameter.name = "Oversample";
			parameter.symbol = "oversample";
			parameter.ranges.min = 0.0f;
			parameter.ranges.max = 4.0f;
			parameter.ranges.def = 1.0f;
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
		case paramOut:
			parameter.name = "Out";
			parameter.symbol = "out";
			parameter.hints = kParameterIsOutput;
			break;
		}
	}

	float getParameterValue(uint32_t index) const override
	{
		return parameters[index];
	}

	void setParameterValue(uint32_t index, float value) override
	{
		parameters[index] = value;
	}

	void initState(uint32_t index, String &stateKey, String &defaultStateValue) override
	{
		switch (index)
		{
		case 0:
			stateKey = "graph";
			break;
		}

		//generated with fprintf(stderr, "%A,%A,%A,%d;%A,%A,%A,%d;\n", 0.0f, 0.0f, 0.0f, spoonie::CurveType::Exponential, 1.0f, 1.0f, 0.0f, spoonie::CurveType::Exponential);
		defaultStateValue = String("0x0p+0,0x0p+0,0x0p+0,0;0x1p+0,0x1p+0,0x0p+0,0;");
	}

	void setState(const char *key, const char *value) override
	{
		if (std::strcmp(key, "graph") == 0)
		{
			lineEditor.rebuildFromString(value);
		}
	}

	float removeDCOffset(float input)
	{
		//Steep IIR filter at the DC frequency
		//Should probably be a FIR filter

		//Previous calculated value
		static float prev = 0.0f;

		const float scaleFactor = 0.9999f; //Closer to 1 means steeper stop band

		const float value = input + scaleFactor * prev;
		const float result = value - prev;

		prev = value;

		return result;
	}

	void run(const float **inputs, float **outputs, uint32_t frames) override
	{
		float max = 0.0f;

		const int oversamplingRatio = std::pow(2, std::round(parameters[paramOversample]));
		const bool mustOversample = oversamplingRatio > 1;
		const uint32_t numSamples = frames * oversamplingRatio;

		float **buffer = oversampler.upsample(oversamplingRatio, frames, getSampleRate(), inputs);

		for (uint32_t i = 0; i < numSamples; ++i)
		{
			const int index = i;

			const float inputL = parameters[paramPreGain] * buffer[0][index];
			const float inputR = parameters[paramPreGain] * buffer[1][index];

			max = std::max(max, std::abs(inputL));
			max = std::max(max, std::abs(inputR));

			const bool bipolarMode = parameters[paramBipolarMode] > 0.50f;
			lineEditor.setBipolarMode(bipolarMode);

			float graphL, graphR;

			if (bipolarMode)
			{
				const float xl = (1.0f + inputL) * 0.5f;
				const float xr = (1.0f + inputR) * 0.5f;

				graphL = -1.0f + lineEditor.getValueAt(xl) * 2.0f;
				graphR = -1.0f + lineEditor.getValueAt(xr) * 2.0f;
			}
			else
			{
				graphL = lineEditor.getValueAt(inputL);
				graphR = lineEditor.getValueAt(inputR);
			}

			const float wet = parameters[paramWet];
			const float dry = 1.0f - wet;
			const float postGain = parameters[paramPostGain];
			const bool mustRemoveDC = parameters[paramRemoveDC] > 0.50f;

			buffer[0][index] = (dry * inputL + wet * graphL) * postGain;
			buffer[1][index] = (dry * inputR + wet * graphR) * postGain;

			if (mustRemoveDC)
			{
				buffer[0][index] = removeDCOffset(buffer[0][index]);
				buffer[1][index] = removeDCOffset(buffer[1][index]);
			}
		}

		oversampler.downsample(outputs);

		setParameterValue(paramOut, max);
	}

  private:
	float parameters[paramCount];
	Oversampler oversampler;

	spoonie::Graph lineEditor;

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaper)
};

Plugin *createPlugin()
{
	return new WaveShaper();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
