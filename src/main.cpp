#include "DataLoader.hpp"
#include "Portfolio.hpp"
#include "Optimizer.hpp"
#include <iostream>
#include <string>

int main(int argc, char * argv[]) {
  auto data = DataLoader::loadCSV("data/prices.csv");
  Portfolio portfolio(data);

  const double tradingDays = 252.0;
  size_t topN = 20;

  if (argc > 1) {
    topN = std::stoul(argv[1]);
  }

  auto mean = portfolio.meanReturns();
  auto cov = portfolio.covarianceMatrix();



  for (auto &m : mean) m *= tradingDays;
  for (size_t i = 0; i < cov.size(); i++) {
    for (size_t j = 0; j < cov.size(); j++) {
      cov[i][j] *= tradingDays;
    }
  }

  double riskFreeRate = 0.01;

  std::vector<std::pair<double, size_t>> sharpeIndices;
  for (size_t i = 0; i < mean.size(); i++) {
    double stdDev = std::sqrt(cov[i][i]);
    double sharpe = (mean[i] - riskFreeRate) / stdDev;
    sharpeIndices.push_back({sharpe, i});
  }

  std::sort(sharpeIndices.begin(), sharpeIndices.end(),
            [](const auto&a, const auto&b) { return a.first > b.first; });

  std::vector<size_t> topIndices;
  for (size_t i = 0; i < topN && i < sharpeIndices.size(); i++) {
    topIndices.push_back(sharpeIndices[i].second);
  }

  std::vector<double> meanTop;
  


  std::vector<std::vector<double>> covTop(topN, std::vector<double>(topN));
  std::vector<std::string> tickersTop;
  for (size_t i = 0; i < topN; i++) {
    tickersTop.push_back(data.tickers[topIndices[i]]);
    meanTop.push_back(mean[topIndices[i]]);
    for (size_t j = 0; j < topN; j++) {
      covTop[i][j] = cov[topIndices[i]][topIndices[j]];
    }
  }

  // Run optimizer on top 20
  size_t trials = 100000;
  auto weights = Optimizer::maxSharpePortfolio(meanTop, covTop, riskFreeRate, trials);

  std::cout << "\nOptimal Weights (Top 20 by individual Sharpe ratio)\n";
  for (size_t i = 0; i < weights.size(); i++) {
    std::cout << tickersTop[i] << ": " << weights[i] << "\n";
  }

  double ret = Optimizer::portfolioReturn(weights, meanTop);
  double var = Optimizer::portfolioVariance(weights, covTop);
  std::cout << "Expected return: " << ret << std::endl;
  std::cout << "Portfolio variance: " << var << std::endl;
  std::cout << "Portfolio std dev: " << std::sqrt(var) << std::endl;

  return 0;
}


