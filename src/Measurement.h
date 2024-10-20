#pragma once

#include <atomic>

namespace delay_plugin {
    template<typename MeasureT>
    struct Measurement {

        void reset() noexcept {
            value.store(MeasureT{});
        }

        void updateIfGreater(MeasureT newValue) {
            auto currentVal = value.load();
            // compare and exchange: compares the value with currentVal
            // if value == currentVal -> stores newValue in the atomic, and returns true
            // if value != currentVal -> loads the value of the atomic in currentVal, and returns false
            while (newValue > currentVal && !value.compare_exchange_weak(currentVal, newValue));
        }

        MeasureT readAndReset() noexcept {
            return value.exchange(0.0f);
        }

        std::atomic<MeasureT> value;
    };

}// namespace delay_plugin
