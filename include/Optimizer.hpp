#ifndef _OPTIMIZER_
#define _OPTIMIZER_

#include <vector>

class Optimizer {
public:
  static std::vector<double> minVariancePortfolio(
        const std::vector<double>& mean,
        const std::vector<std::vector<double>>& cov
      );

  static double portfolioVariance(const std::vector<double>& weights,
                                  const std::vector<std::vector<double>>& covMatrix);
  
  static double portfolioReturn(const std::vector<double>& weights,
                                const std::vector<double>& mean);

  static std::vector<double> maxSharpePortfolio(const std::vector<double>& mean,
                                                const std::vector<std::vector<double>>& covMatrix,
                                                double riskFreeRate,
                                                size_t numTrials,
                                                size_t portfolioSize = 20);
};

#endif
