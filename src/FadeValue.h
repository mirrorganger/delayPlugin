#pragma once

#include "Common.h"
#include <optional>
namespace delay_plugin {
    template<typename ValuType>
    struct FadeValue {

        void prepare(float sampleRate) {
            _coeff =
                    1.0f -
                    std::exp(1.0f / (seconds_t(_fadePeriod).count() * sampleRate));
            _waitStep = 1.0f / (seconds_t(_waitPeriod).count() * sampleRate);
            _wait = 0.0f;
            _fade = 1.0f;
            _fadeTarget = 1.0f;
            _value = std::nullopt;
        }

        void update(ValuType newValue) {
            if (newValue != _target) {
                _target = newValue;
                if (!_value) {
                    _value = _target;
                } else {
                    _wait = _waitStep;
                    _fadeTarget = 0.0f;
                }
            }
        }

        void updateFade() {
            _fade += (_fadeTarget - _fade) * _coeff;
            if (_wait > 0.0f) {
                _wait += _waitStep;
                if (_wait >= 1.0f) {
                    _value = _target;
                    _wait = 0.0f;
                    _fadeTarget = 1.0f;// fade in
                }
            }
        }

        ValuType operator()(){
            return _value.has_value()? *_value : ValuType{};
        }

    private:
        constexpr static millis_t _waitPeriod{300};
        constexpr static millis_t _fadePeriod{50};
        float _wait;
        float _waitStep;
        float _fade;
        float _fadeTarget;
        float _coeff;
        std::optional<ValuType> _value = std::nullopt;
        ValuType _target;
    };

}// namespace delay_plugin
