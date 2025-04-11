#include "DataManager.hpp"
#include "stdexcept"

void DataManager::loadSymbolData(const std::string& symbol) {
	auto data = m_api.fetchDailyPrices(symbol);
	m_symbolData[symbol] = data;
}

const std::vector<PriceData>& DataManager::getSymbolData(const std::string& symbol) const{
	auto it = m_symbolData.find(symbol);
	if (it == m_symbolData.end()) {
		throw std::runtime_error("Symbol data is not found" + symbol);
	}
	return it->second;
}