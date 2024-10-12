#pragma once

#include <chrono>

namespace delay_plugin
{

using millis_t = std::chrono::duration<double, std::milli>;
using seconds_t = std::chrono::duration<double, std::ratio<1,1>>;


} // namespace delay_plugin
