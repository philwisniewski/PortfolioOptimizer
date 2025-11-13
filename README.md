# Portfolio Optimizer

Portfolio optimizer with a simple backtest primarily based on Monte Carlo simulations to maximize Sharpe ratio.

### To run:

```
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```


### To gather data:

```
cd data
python download_data.py
```

### To set up CMake:
```
mkdir build
cd build
cmake ..
cd ..
```

### To Compile and Run Optimizer:

```
cmake --build build
./quant_portfolio > tests/results.txt
```

### To backtest:

```
cd tests
python backtest_daily.py --file [file containing output of quant_portfolio] --start [YYYY-MM-DD] --end [YYY-MM-DD] --risk-free [float, suggested values: 0.0, 0.02] --benchmark [ticker to compare against, suggested: spy]
```
