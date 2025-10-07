#include "DataLoader.hpp"
#include "Portfolio.hpp"
#include "Optimizer.hpp"
#include <iostream>

int main() {
  auto data = DataLoader::loadCSV("data/prices.csv");
  Portfolio portfolio(data);

  auto mean = portfolio.meanReturns();
  auto cov = portfolio.covarianceMatrix();

  double riskFreeRate = 0.01;
  size_t trials = 100000;
  auto weights = Optimizer::maxSharpePortfolio(mean, cov, riskFreeRate, trials);
  // auto weights = Optimizer::minVariancePortfolio(mean, cov);

  std::cout << "\nOptimal Weights" << std::endl;

  for (size_t i = 0; i < weights.size(); i++) {
    std::cout << data.tickers[i] << ": " << weights[i] << "\n";
  }

  double ret = Optimizer::portfolioReturn(weights, mean);
  double var = Optimizer::portfolioVariance(weights, cov);
  std::cout << "Expected return: " << ret << std::endl;
  std::cout << "Portfolio variance: " << var << std::endl;
  std::cout << "Portfolio std dev: " << std::sqrt(var) << std::endl;

  return 0;
}
