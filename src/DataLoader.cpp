#include "DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

PriceData DataLoader::loadCSV(const std::string& path) {
  PriceData data;
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << std::endl;
    return data;
  }

  std::string line;

  // first line looks like Date,Tckr1,Tckr2,...,TckrN
  if (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string ticker;
    std::getline(ss, ticker, ',');
    while (std::getline(ss, ticker, ',')) {
      data.tickers.push_back(ticker);
    }
  }

  // read prices
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string date;
    std::getline(ss, date, ',');

    std::vector<double> row;
    std::string price;
    while (std::getline(ss, price, ',')) {
      row.push_back(std::stod(price));
    }
    data.prices.push_back(row);
  }

  return data;
}
