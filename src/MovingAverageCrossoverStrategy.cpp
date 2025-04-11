#include "MovingAverageCrossoverStrategy.hpp"
#include <cmath>

MovingAverageCrossoverStrategy::MovingAverageCrossoverStrategy(int shortWindow, int longWindow)
    : m_shortWindow(shortWindow), m_longWindow(longWindow) {}


std::vector<TradeSignal> MovingAverageCrossoverStrategy::generateSignals(const std::vector<PriceData>& priceSeries) {
    std::vector<TradeSignal> signals(priceSeries.size(), TradeSignal::HOLD);

    std::vector<double> shortMA = computeMA(priceSeries, m_shortWindow);
    std::vector<double> longMA = computeMA(priceSeries, m_longWindow);

    bool hasPosition = false;
    for (size_t i = 1; i < priceSeries.size(); ++i) {
        if (shortMA[i] > longMA[i] && shortMA[i - 1] <= longMA[i - 1]) {
            if (!hasPosition) {
                signals[i] = TradeSignal::BUY;
                hasPosition = true;
            }
        }
        else if (shortMA[i] < longMA[i] && shortMA[i - 1] >= longMA[i - 1]) {
            if (hasPosition) {
                signals[i] = TradeSignal::SELL;
                hasPosition = false;
            }
        }
    }

    return signals;
}


std::vector<double> MovingAverageCrossoverStrategy::computeMA(const std::vector<PriceData>& data, int window) {
    std::vector<double> ma(data.size(), 0.0);
    double sum = 0.0;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i].close;
        if (i >= static_cast<size_t>(window)) {
            sum -= data[i - window].close;
            ma[i] = sum / window;
        }
        else {
            ma[i] = sum / (i + 1);
        }
    }
    return ma;
}


std::string MovingAverageCrossoverStrategy::name() {
    return "MovingAverage(" + std::to_string(m_shortWindow) + "," + std::to_string(m_longWindow) + ")";
}