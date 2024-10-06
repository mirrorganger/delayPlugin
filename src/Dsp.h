#pragma once

#define _USE_MATH_DEFINES


#include <tuple>
#include <cmath>
namespace  delay_plugin
{

/**
 * @brief Returns left and right channel panning values based 
 * on the input 
 * @param panning: a value between -1.0 and 1.0. -1.0 fully left panned, 1.0 fully right panned.
 */
inline std::tuple<float, float> getPanningEqualPower(float panning){
    auto x = M_PI_4f  * (1.0f + panning);
    return {std::cos(x),std::sin(x)};
}

} // namespace  delay_plugin
