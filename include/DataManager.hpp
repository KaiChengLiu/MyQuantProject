#pragma once

#include "Api.hpp"
#include <map>
#include <string>

class DataManager {
public:
	explicit DataManager(Api& api) : m_api(api) {}

	// download stock history data and store it
	void loadSymbolData(const std::string& symbol);

	// Get all daily line data of a certain stock
	const std::vector<PriceData>& getSymbolData(const std::string& symbol) const;

private:
	Api& m_api;
	std::map<std::string, std::vector<PriceData>> m_symbolData;
};