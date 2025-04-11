#pragma once

#include <string>
#include <vector>

struct PriceData {
	std::string date;
	double open;
	double close;
	double high;
	double low;
	double volume;
};

class Api {
public:
	Api(const std::string& apiKey);

	//Download daily data of stock
	std::vector<PriceData> fetchDailyPrices(const std::string symbol);

	std::string buildApiUrl(const std::string& functionName,
							const std::string& symbol,
							const std::string& interval,
							const std::string& outputSize,
							const std::string& dataType);

private:
	std::string m_apiKey;
};