#ifndef _PORTFOLIO_
#define _PORTFOLIO_

#include "DataLoader.hpp"
#include <vector>
#include <string>

class Portfolio {
private:
  std::vector<std::string> tickers;
  std::vector<std::vector<double>> returns;

public:
  Portfolio(const PriceData& data);
  std::vector<double> meanReturns() const;
  std::vector<std::vector<double>> covarianceMatrix() const;
};

#endif
