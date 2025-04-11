#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Api.hpp"
#include "DataManager.hpp"
#include "MovingAverageCrossoverStrategy.hpp"
#include "Backtester.hpp"

void exportEquityCurve(const std::string& filename, const std::vector<DailyBalance>& equityCurve) {
    std::ofstream out(filename);
    out << "date,equity\n";
    for (auto& db : equityCurve) {
        out << db.date << "," << db.equity << "\n";
    }
    out.close();
}

nlohmann::json load_config(const std::string& filepath) {
    std::ifstream file(filepath);
    nlohmann::json config;
    file >> config;
    return config;
}

int main() {
    auto config = load_config("config.json");
    std::string apiKey = config["api_key"];
    Api api(apiKey);

    
    DataManager dm(api);
    dm.loadSymbolData("AAPL");
    auto priceSeries = dm.getSymbolData("AAPL");
    std::cout << "Loaded AAPL data size: " << priceSeries.size() << std::endl;

    MovingAverageCrossoverStrategy strategy(10, 50);

    Backtester backtester(100000.0, &strategy);
    backtester.run(priceSeries);

    double finalEquity = backtester.getFinalEquity();
    std::cout << "Final equity: " << finalEquity << std::endl;

    auto trades = backtester.getTradeRecord();
    for (auto& t : trades) {
        std::cout << t.date << " - "
            << ((t.signal == TradeSignal::BUY) ? "BUY" : "SELL")
            << " at " << t.price << std::endl;
    }

    double totalReturn = (finalEquity - 100000.0) / 100000.0 * 100.0;
    std::cout << "Total Return: " << totalReturn << " %\n";

    auto eqCurve = backtester.getDailyBanalce();
    exportEquityCurve("equity_curve.csv", eqCurve);

	return 0;
}