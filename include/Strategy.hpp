#pragma once

#include "Api.hpp"
#include <vector>

enum class TradeSignal {
	BUY,
	SELL,
	HOLD
};

class Strategy {
public:
	virtual ~Strategy() = default;

	// Calculate the trading signal corresponding to each time step based on the historical price
	virtual std::vector<TradeSignal> generateSignals(const std::vector<PriceData>& priceSeries) = 0;

	virtual std::string name() = 0;
};