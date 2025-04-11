#include "Api.hpp"
#include <curl/curl.h>
#include <sstream>
#include <iostream>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string  Api::buildApiUrl(const std::string& functionName,
							  const std::string& symbol,
							  const std::string& interval,
							  const std::string& outputSize,
							  const std::string& dataType) {
	if (m_apiKey.empty()) {
		std::cerr << "apiKey should be initialized" << '\n';
		exit(0);
	}

	std::stringstream url;
	url << "https://www.alphavantage.co/query?"
		<< "function=" << functionName
		<< "&symbol=" << symbol
		<< "&apikey=" << m_apiKey
		<< "&outputsize=" << outputSize
		<< "&datatype=" << dataType;
	
	// if there has interval
	if (!interval.empty()) {
		url << "&interval=" << interval;
	}
	return url.str();
}

Api::Api(const std::string& apiKey) : m_apiKey(apiKey) {}

std::vector<PriceData> Api::fetchDailyPrices(const std::string symbol) {
	if (m_apiKey.empty()) {
		std::cerr << "apiKey should be initialized" << '\n';
		exit(0);
	}

	std::vector<PriceData> data;

	std::string url = buildApiUrl(
		"TIME_SERIES_DAILY",
		symbol,
		"",		// no interval in daily method
		"full", // obtain dull data
		"csv"	// store in csv
	);

	CURL* curl = curl_easy_init();
	if (!curl) {
		std::cerr << "Error initializing curl" << '\n';
		return data;
	}

	// set http query options
	std::string readbuffer;
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readbuffer);

	// do the query
	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		curl_easy_cleanup(curl);
		return data;
	}
	curl_easy_cleanup(curl);
	//std::cout << "[DEBUG] API response:\n" << readbuffer << std::endl;

	// analyze csv
	// each row title: timestamp,open,high,low,close,adjusted_close,volume,dividend_amount,...
	std::istringstream iss(readbuffer);
	std::string line;
	bool isHeader = true;

	while (std::getline(iss, line)) {
		if (line.empty()) {
			continue;
		}

		if (isHeader) {
			isHeader = false;
			continue;
		}

		//std::cout << line << std::endl;
		std::stringstream linestream(line);


		std::string dateStr, openStr, highStr, lowStr, closeStr, volumeStr;

		std::getline(linestream, dateStr, ',');     
		std::getline(linestream, openStr, ',');     
		std::getline(linestream, highStr, ',');     
		std::getline(linestream, lowStr, ',');      
		std::getline(linestream, closeStr, ',');    
		std::getline(linestream, volumeStr, ',');   


		if (dateStr.empty() || openStr.empty() || highStr.empty() ||
			lowStr.empty() || closeStr.empty() || volumeStr.empty()) {
			continue;
		}

		PriceData pd;
		pd.date = dateStr;  // time stamp

		try {
			pd.open = std::stod(openStr);
			pd.high = std::stod(highStr);
			pd.low = std::stod(lowStr);
			pd.close = std::stod(closeStr);
			pd.volume = std::stod(volumeStr);
		}
		catch (const std::invalid_argument& e) {
			continue;
		}
		data.push_back(pd);
	}

	// Notice that the Alpha Ventage csv gives the latest data from up to down, we need to reverse the data to get the old data first
	std::reverse(data.begin(), data.end());
	
	return data;
}
