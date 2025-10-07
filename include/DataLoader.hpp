#ifndef _DATA_LOADER_
#define _DATA_LOADER_

#include <string>
#include <vector>

struct PriceData {
  std::vector<std::string> tickers;
  std::vector<std::vector<double>> prices;
};

class DataLoader {
  public:
    static PriceData loadCSV(const std::string& path);
};


#endif
