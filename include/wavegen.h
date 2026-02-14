#ifndef WAVEGEN_H
#define WAVEGEN_H

#include <random>

namespace Scope {

template <typename T> static void generate_waveform(Channel &c, T wave_func) {
  for (int i = 0; i < c.trace.size(); ++i) {
    const float dx = 0.001;
    float time = static_cast<float>(i) * dx;
    c.trace.add_sample(time, wave_func(time));
  }
}

static float sinewave_1(float x) {
  return 0.5f + 0.5f * sinf(50 * (x + (float)ImGui::GetTime() / 20));
}

static float sinewave_2(float x) {
  return 0.6f + 0.5f * sinf(30 * (x + (float)ImGui::GetTime() / 18));
}

class WaveformGenerator {
public:
  WaveformGenerator() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> freqDist(2.0f, 20.0f);
    static std::uniform_real_distribution<float> phaseDist(0.0f, 30.28f);

    _frequency = freqDist(gen);
    _phaseShift = phaseDist(gen);
  }

  float generate(float t, float offset) const {

    return 0.5f + 0.5f * sinf(_frequency * (t + offset) + _phaseShift);
  }

private:
  float _frequency;
  float _phaseShift;
};
} // namespace Scope

#endif // WAVEGEN_H
