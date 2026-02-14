#ifndef TRACE_H
#define TRACE_H

#include <algorithm>
#include <array>
#include <string>

namespace Scope {

template <size_t N> class Trace {
public:
  std::string label;

  explicit Trace(std::string label)
      : label(std::move(label)), _offset(0), x_data{}, y_data{} {}

  void add_sample(float x, float y) {
    x_data[_offset] = x;
    y_data[_offset] = y;
    _offset = (_offset + 1) % N;
  }

  const float *get_data_x() const { return x_data.data(); }
  const float *get_data_y() const { return y_data.data(); }
  const int get_data_offset() const { return _offset; }

  size_t size() { return x_data.size(); }

  void clear() {
    _offset = 0;
    std::fill(x_data.begin(), x_data.end(), 0.0f);
    std::fill(y_data.begin(), y_data.end(), 0.0f);
  }

private:
  size_t _offset;
  std::array<float, N> x_data;
  std::array<float, N> y_data;
};

} // namespace Scope

#endif // TRACE_H
