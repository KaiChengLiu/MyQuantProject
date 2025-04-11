#include "Backtester.hpp"
#include <iostream>

Backtester::Backtester(double initialCapital, Strategy* strategy) : m_equity(initialCapital), m_sharesHeld(0.0), m_strategy(strategy) {}

void Backtester::run(const std::vector<PriceData>& priceSeries) {
	// obtain signal of every time step
	auto signals = m_strategy->generateSignals(priceSeries);

	// Simplified: Use closing price to buy and sell
	for (size_t i = 0; i < priceSeries.size(); ++i) {
		double closePrice = priceSeries[i].close;

		if (signals[i] == TradeSignal::BUY) {
			if (m_equity > 0) {
				m_sharesHeld = m_equity / closePrice;
				m_equity = 0.0;

				TradeRecord tr{ priceSeries[i].date, closePrice, TradeSignal::BUY };
				m_trades.push_back(tr);
			}
		}
		else if (signals[i] == TradeSignal::SELL) {
			if (m_sharesHeld > 0) {
				m_equity = m_sharesHeld * closePrice;
				m_sharesHeld = 0.0;

				TradeRecord tr{priceSeries[i].date, closePrice, TradeSignal::SELL};
				m_trades.push_back(tr);
			}
		}

		double currentVlaue = m_equity + (m_sharesHeld * closePrice);
		DailyBalance db{ priceSeries[i].date, currentVlaue };
		m_equityCurve.push_back(db);
	}
}