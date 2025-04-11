#pragma once 

#include "Strategy.hpp"
#include <vector>

struct TradeRecord {
	std::string date;
	double price;
	TradeSignal signal;
};

struct DailyBalance {
	std::string date;
	double equity;
};

class Backtester {
public:
	Backtester(double initialCapital, Strategy* strategy);

	void run(const std::vector<PriceData>& priceSeries);
	double getFinalEquity() const { return m_equity; }
	const std::vector<TradeRecord>& getTradeRecord() const { return m_trades; }
	const std::vector<DailyBalance> getDailyBanalce() const { return m_equityCurve; }

private:
	double m_initialCapital;
	double m_equity;
	double m_sharesHeld; // Simplified: only stocks, long and short positions
	Strategy* m_strategy;

	std::vector<TradeRecord> m_trades;
	std::vector<DailyBalance> m_equityCurve;
};