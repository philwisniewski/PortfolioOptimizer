#include "Optimizer.hpp"
#include <random>
#include <cmath>
#include <numeric>
#include <limits>
#include <iostream>

double Optimizer::portfolioVariance(const std::vector<double>& weights,
                                    const std::vector<std::vector<double>>& cov) {
  double var = 0.0;
  size_t N = weights.size();
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      var += weights[i] * weights[j] * cov[i][j];
    }
  }
  return var;
}

double Optimizer::portfolioReturn(const std::vector<double>& weights,
                                  const std::vector<double>& mean) {
  double ret = 0.0;
  for (size_t i = 0; i < weights.size(); i++) {
    ret += weights[i] * mean[i];
  }
  return ret;
}

std::vector<double> Optimizer::maxSharpePortfolio(const std::vector<double>& mean,
                                                  const std::vector<std::vector<double>>& cov,
                                                  double riskFreeRate,
                                                  size_t numTrials) {
  size_t N = mean.size();
  std:: vector<double> bestWeights(N);
  double bestSharpe = -std::numeric_limits<double>::infinity();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  for (size_t trial = 0; trial < numTrials; trial++) {
    std::vector<double> w(N);
    double sum = 0.0;
    for (size_t i = 0; i < N; i++) {
      w[i] = dis(gen);
      sum += w[i];
    }
    for (auto &x : w) {
      x /= sum;
    }

    double ret = portfolioReturn(w, mean);
    double var = portfolioVariance(w, cov);
    double sharpe = (ret - riskFreeRate) / std::sqrt(var);

    if (sharpe > bestSharpe) {
      bestSharpe = sharpe;
      bestWeights = w;
    }
  }

  std::cout << "Max Sharpe Ratio: " << bestSharpe << std::endl;
  return bestWeights;
}
