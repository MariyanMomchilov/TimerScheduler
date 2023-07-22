#pragma once

#include <functional>
#include <chrono>

using callback_t = std::function<void()>;
using ns_t = std::chrono::nanoseconds;
using time_point_t = std::chrono::time_point<std::chrono::system_clock, ns_t>;
