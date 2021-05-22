#ifndef WOLF_ANIMATION_DEFINED_H
#define WOLF_ANIMATION_DEFINED_H

#include "src/DistrhoDefines.h"
#include "Color.hpp"
#include "Geometry.hpp"
#include "Widget.hpp"
#include "NanoVG.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <vector>

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::Color;

class Animation
{
public:
  enum PlaybackDirection
  {
    Forward,
    Backward
  };

  enum RepeatMode
  {
    NoRepeat,
    PingPong //unsupported for now
  };

  enum EasingFunction
  {
    noEasing, //linear
    easeOutPower2,
    easeOutPower4,
    easeInPower2,
    easeInPower4
  };

  Animation(float duration, EasingFunction easingFunction = noEasing);
  ~Animation();

  void play(PlaybackDirection playbackDirection = Forward, RepeatMode repeatMode = NoRepeat);
  void pause();
  void seek(float time);
  void rewind();

  virtual void run() = 0;

  bool isPlaying();

  void setDuration(float duration);
  void setSpeed(float speed);

  float getCurrentTime();
  float getDuration();

protected:
  void synchronize();
  void pauseIfDone();

  virtual void onPlay();
  virtual void onSeek();
  virtual void onDurationChange();
  virtual void onSpeedChange();

  virtual void applyEasing();

  float fDuration;
  float fCurrentTime;
  float fSpeed;
  std::chrono::steady_clock::time_point fTimeLastRun;
  PlaybackDirection fPlaybackDirection;
  RepeatMode fRepeatMode;
  EasingFunction fEasingFunction;
  bool fIsPlaying;

private:
  DISTRHO_LEAK_DETECTOR(Animation)
};

class FloatTransition : public Animation
{
public:
  FloatTransition();

  FloatTransition(float duration, float *initialValue, float targetValue, EasingFunction easingFunction = noEasing);
  virtual ~FloatTransition();

  void run() override;

protected:
  void applyEasing() override;

  float fInitialValue;
  float *fCurrentValue;
  float fTargetValue;

private:
  DISTRHO_LEAK_DETECTOR(FloatTransition)
};

class AnimationContainer : public Animation
{
public:
  AnimationContainer(float duration, EasingFunction easingFunction = noEasing);
  virtual ~AnimationContainer();

  void run() override;

  void onPlay() override;
  void onSeek() override;
  void onDurationChange() override;
  void onSpeedChange() override;

protected:
  void applyEasing() override;

  std::vector<std::shared_ptr<Animation>> fAnimations;

private:
  DISTRHO_LEAK_DETECTOR(AnimationContainer)
};

class ColorTransition : public AnimationContainer
{
public:
  ColorTransition(float duration, Color *initialColor, Color targetColor, EasingFunction easingFunction = noEasing);
  virtual ~ColorTransition();

private:
  DISTRHO_LEAK_DETECTOR(ColorTransition)
};

class GradientTransition : public AnimationContainer
{
public:
  GradientTransition();
  GradientTransition(float duration, NanoVG::Paint *initialGradient, NanoVG::Paint targetGradient, EasingFunction easingFunction = noEasing);
  virtual ~GradientTransition();

private:
  DISTRHO_LEAK_DETECTOR(GradientTransition)
};

END_NAMESPACE_DISTRHO

#endif