#include "Animation.hpp"
#include "Color.hpp"
#include "src/DistrhoDefines.h"
#include "Mathf.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"

#include <chrono>

START_NAMESPACE_DISTRHO

Animation::Animation(float duration, EasingFunction easingFunction) : fDuration(duration),
																	  fCurrentTime(0.0f),
																	  fSpeed(1.0f),
																	  fTimeLastRun(std::chrono::steady_clock::now()),
																	  fPlaybackDirection(Forward),
																	  fRepeatMode(NoRepeat),
																	  fEasingFunction(easingFunction),
																	  fIsPlaying(false)

{
}

Animation::~Animation()
{
}

void Animation::play(PlaybackDirection playbackDirection, RepeatMode repeatMode)
{
	fIsPlaying = true;

	fPlaybackDirection = playbackDirection;
	fRepeatMode = repeatMode;

	fTimeLastRun = std::chrono::steady_clock::now();

	onPlay();
}

void Animation::onPlay()
{
}

void Animation::onSeek()
{
}

void Animation::onDurationChange()
{
}

void Animation::onSpeedChange()
{
}

void Animation::pause()
{
	fIsPlaying = false;
}

void Animation::seek(float time)
{
	fCurrentTime = wolf::clamp(time, 0.0f, fDuration);
	fTimeLastRun = std::chrono::steady_clock::now();

	onSeek();
}

void Animation::rewind()
{
	seek(0);
}

float Animation::getCurrentTime()
{
	return fCurrentTime;
}

float Animation::getDuration()
{
	return fDuration;
}

void Animation::setDuration(float duration)
{
	fDuration = duration;

	onDurationChange();
}

bool Animation::isPlaying()
{
	return fIsPlaying;
}

void Animation::applyEasing()
{
	//TODO
}

void Animation::setSpeed(float speed)
{
	fSpeed = speed;

	onSpeedChange();
}

void Animation::synchronize()
{
	using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();

	float deltaTime = duration_cast<duration<float>>((now - fTimeLastRun) * fSpeed).count();

	if (fPlaybackDirection == Forward)
		fCurrentTime = std::min(fDuration, fCurrentTime + deltaTime);
	else
		fCurrentTime = std::max(0.0f, fCurrentTime - deltaTime);

	fTimeLastRun = now;
}

void Animation::pauseIfDone()
{
	if ((fPlaybackDirection == Forward && fCurrentTime >= fDuration) || (fPlaybackDirection == Backward && fCurrentTime <= 0.0f))
	{
		this->pause();
	}
}

FloatTransition::FloatTransition() : Animation(0.0f, noEasing)
{
}

FloatTransition::FloatTransition(float duration, float *initialValue, float targetValue, EasingFunction easingFunction) : Animation(duration, easingFunction),
																														  fInitialValue(*initialValue),
																														  fCurrentValue(initialValue),
																														  fTargetValue(targetValue)
{
}

FloatTransition::~FloatTransition()
{
}

void FloatTransition::applyEasing()
{
}

void FloatTransition::run()
{
	synchronize();

	//Just some cheap lerp for now
	*fCurrentValue = wolf::lerp(fInitialValue, fTargetValue, fCurrentTime / fDuration);

	pauseIfDone();
}

AnimationContainer::AnimationContainer(float duration, EasingFunction easingFunction) : Animation(duration, easingFunction)
{
}

AnimationContainer::~AnimationContainer()
{
}

void AnimationContainer::applyEasing()
{
}

void AnimationContainer::onPlay()
{
	for (size_t i = 0; i < fAnimations.size(); ++i)
	{
		fAnimations[i]->play(fPlaybackDirection, fRepeatMode);
	}
}

void AnimationContainer::onSeek()
{
	for (size_t i = 0; i < fAnimations.size(); ++i)
	{
		fAnimations[i]->seek(fCurrentTime);
	}
}

void AnimationContainer::onDurationChange()
{
	for (size_t i = 0; i < fAnimations.size(); ++i)
	{
		fAnimations[i]->setDuration(fDuration);
	}
}

void AnimationContainer::onSpeedChange()
{
	for (size_t i = 0; i < fAnimations.size(); ++i)
	{
		fAnimations[i]->setSpeed(fSpeed);
	}
}

void AnimationContainer::run()
{
	synchronize();

	for (size_t i = 0; i < fAnimations.size(); ++i)
	{
		fAnimations[i]->run();
	}

	pauseIfDone();
}

ColorTransition::ColorTransition(float duration, Color *initialColor, Color targetColor, EasingFunction easingFunction) : AnimationContainer(duration, easingFunction)
{
	fAnimations = std::vector<std::shared_ptr<Animation>>(4);

	fAnimations[0] = std::make_shared<FloatTransition>(duration, &initialColor->red, targetColor.red, easingFunction);
	fAnimations[1] = std::make_shared<FloatTransition>(duration, &initialColor->green, targetColor.green, easingFunction);
	fAnimations[2] = std::make_shared<FloatTransition>(duration, &initialColor->blue, targetColor.blue, easingFunction);
	fAnimations[3] = std::make_shared<FloatTransition>(duration, &initialColor->alpha, targetColor.alpha, easingFunction);
}

ColorTransition::~ColorTransition()
{
}

GradientTransition::GradientTransition() : AnimationContainer(0)
{
}

GradientTransition::GradientTransition(float duration, NanoVG::Paint *initialGradient, NanoVG::Paint targetGradient, EasingFunction easingFunction) : AnimationContainer(duration, easingFunction)
{
	fAnimations = std::vector<std::shared_ptr<Animation>>(2);

	fAnimations[0] = std::make_shared<ColorTransition>(duration, &initialGradient->innerColor, targetGradient.innerColor, easingFunction);
	fAnimations[1] = std::make_shared<ColorTransition>(duration, &initialGradient->outerColor, targetGradient.outerColor, easingFunction);
}

GradientTransition::~GradientTransition()
{
}

END_NAMESPACE_DISTRHO