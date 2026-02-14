#ifndef WAVEGEN_H
#define WAVEGEN_H

#include <random>

namespace Scope {

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
