/*
 * spoonie WaveShaper
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

#include "Graph.hpp"

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
		paramCount
	};

	WaveShaper()
		: Plugin(paramCount, 0, 1)
	{
		parameters[paramPreGain] = 1.0f;
		parameters[paramWet] = 1.0f;
		parameters[paramPostGain] = 1.0f;
		parameters[paramRemoveDC] = 0.0f;
		parameters[paramOversample] = 0.0f;
		parameters[paramBipolarMode] = 0.0f;

		lineEditor = new spoonie::Graph();
	}

  protected:
	const char *getLabel() const noexcept override
	{
		return "spoonie WaveShaper";
	}

	const char *getDescription() const noexcept override
	{
		return "Graphical distortion plugin.";
	}

	const char *getMaker() const noexcept override
	{
		return "spoonie (Patrick Desaulniers)";
	}

	const char *getHomePage() const noexcept override
	{
		return "https://github.com/desaulniersp/spoonie-waveshaper";
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
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
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
			parameter.hints = kParameterIsAutomable | kParameterIsInteger;
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

		//fprintf(stderr, "%A,%A,%A,%d;%A,%A,%A,%d;\n", 0.0f, 0.0f, 0.0f, spoonie::CurveType::Exponential, 1.0f, 1.0f, 0.0f, spoonie::CurveType::Exponential);
		defaultStateValue = String("0X0P+0,0X0P+0,0X0P+0,0;0X1P+0,0X1P+0,0X0P+0,0;");
	}

	void setState(const char *key, const char *value) override
	{
		if (std::strcmp(key, "graph") == 0) {
			lineEditor->rebuildFromString(value);
		}
	}

	float removeDCOffset(float input)
	{
		static float prev = 0.0f;

		const float w_n = input + 0.9999f * prev;
		const float result = w_n - prev;

		prev = w_n;

		return result;
	}

	void run(const float **inputs, float **outputs, uint32_t frames) override
	{
		for (uint32_t i = 0; i < frames; i++)
		{
			outputs[0][i] = lineEditor->getValueAt(parameters[paramPreGain] * inputs[0][i]);
			outputs[1][i] = lineEditor->getValueAt(parameters[paramPreGain] * inputs[1][i]);

			const float wet = parameters[paramWet];
			const float dry = 1.0f - wet;
			const float postGain = parameters[paramPostGain];
			const bool mustRemoveDC = parameters[paramRemoveDC] > 0.50f;

			outputs[0][i] = (dry * inputs[0][i] + wet * outputs[0][i]) * postGain;
			outputs[1][i] = (dry * inputs[1][i] + wet * outputs[1][i]) * postGain;

			if (mustRemoveDC)
			{
				outputs[0][i] = removeDCOffset(outputs[0][i]);
				outputs[1][i] = removeDCOffset(outputs[1][i]);
			}
		}
	}

  private:
	float parameters[paramCount];
	spoonie::Graph* lineEditor;

	DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveShaper)
};

Plugin *createPlugin()
{
	return new WaveShaper();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
