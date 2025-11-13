import argparse
import re
import yfinance as yf

parser = argparse.ArgumentParser(description="A script to backtest specific stock portfolio")

parser.add_argument("--start", type=str, help="Start (buy) date for testing")
parser.add_argument("--end", type=str, help="End (sell) date for testing")
parser.add_argument("--file", type=str, help="File containing which stocks to buy and what percent of portfolio they encompass")

args = parser.parse_args()

def get_tickers(filename: str) -> dict:
    with open(filename) as f:
        data = f.read()
    tickers = re.findall(
            r'^([A-Z]+):\s*([+-]?\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)',
            data,
            flags=re.MULTILINE
    )
    # tickers = re.findall(r'^([A-Z]+):\s*([\d.]+)', data, flags=re.MULTILINE)
    ticker_dict = {t: float(v) for t, v in tickers}
    return ticker_dict

def get_prices(tickers, start, end):
    data = yf.download(list(tickers), start=start, end=end, progress=False)
    open_prices = data["Open"].iloc[0]
    close_prices = data["Close"].iloc[-1]
    return open_prices, close_prices

def get_sp_prices(start, end):
    data = yf.download("^GSPC", start=start, end=end, progress=False)
    open_price = data["Open"].iloc[0]
    close_price = data["Close"].iloc[-1]
    return open_price, close_price

if __name__ == "__main__":
    if args.file == None:
        print("Error: must provide file to read from with --file FILE")
        exit(1)

    start = "2025-01-01"
    end = "2025-04-01"
    file = args.file

    if args.start is not None:
        start = args.start
    if args.end is not None:
        end = args.end

    portfolio = get_tickers(file)

    tickers = portfolio.keys()

    open_prices, close_prices = get_prices(tickers, start, end)

    print(open_prices)
    print(close_prices)

    for t, w in portfolio.items():
        print(t)
        print((close_prices[t] - open_prices[t]) / open_prices[t])
        print(w) 
        print((close_prices[t] - open_prices[t]) / open_prices[t] * w)
        print()

    portfolio_return = sum(((close_prices[t] - open_prices[t]) / open_prices[t]) * w for t, w in portfolio.items()) * 100

    sp_open, sp_close = get_sp_prices(start, end)
    sp_return = (sp_close.iloc[0] - sp_open.iloc[0]) / sp_open.iloc[0] * 100

    print(f"\nPortfolio performance from {start} to {end}: {portfolio_return:.2f}%")
    print(f"S&P 500 performance from {start} to {end}: {sp_return:.2f}%")
    
