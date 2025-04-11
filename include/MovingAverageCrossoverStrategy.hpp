#pragma once
#include "Strategy.hpp"


class MovingAverageCrossoverStrategy : public Strategy {
public:
    MovingAverageCrossoverStrategy(int shortWindow, int longWindow);

    std::vector<TradeSignal> generateSignals(const std::vector<PriceData>& priceSeries) override;

    std::string name() override;

private:
    int m_shortWindow;
    int m_longWindow;

    std::vector<double> computeMA(const std::vector<PriceData>& data, int window);
};
