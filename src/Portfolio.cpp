#include "Portfolio.hpp"
#include <cmath>
#include <numeric>
#include <iostream>

Portfolio::Portfolio(const PriceData& data) {
  tickers = data.tickers;
  size_t numAssets = data.tickers.size();
  size_t numDays = data.prices.size();

  returns.resize(numAssets, std::vector<double>(numDays - 1));

  for (size_t i = 0; i < numAssets; i++) {
    for (size_t t = 1; t < numDays; t++) {
      returns[i][t - 1] = std::log(data.prices[t][i] / data.prices[t - 1][i]);
    }
  }
}

std::vector<double> Portfolio::meanReturns() const {
  std::vector<double> mean(returns.size());
  for (size_t i = 0; i < returns.size(); i++) {
    double sum = std::accumulate(returns[i].begin(), returns[i].end(), 0.0);
    mean[i] = sum / returns[i].size();
  }
  return mean;
}

std::vector<std::vector<double>> Portfolio::covarianceMatrix() const {
  size_t N = returns.size();
  size_t T = returns[0].size();
  std::vector<std::vector<double>> cov(N, std::vector<double>(N, 0.0));

  auto mean = meanReturns();

  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      double sum = 0.0;
      for (size_t t = 0; t < T; t++) {
        sum += (returns[i][t] - mean[i]) * (returns[j][t] - mean[j]);
      }
      cov[i][j] = sum / (T - 1);
    }
  }

  return cov;
}
