import argparse
import re
import yfinance as yf
import matplotlib.pyplot as plt
import pandas as pd

parser = argparse.ArgumentParser(description="A script to backtest specific stock portfolio")
parser.add_argument("--start", type=str, help="Start (buy) date for testing")
parser.add_argument("--end", type=str, help="End (sell) date for testing")
parser.add_argument("--file", type=str, help="File containing which stocks to buy and what percent of portfolio they encompass")
parser.add_argument("--risk-free", type=float, help="Risk free return rate")
parser.add_argument("--benchmark", type=str, help="TCKR for benchmark stock or index. Default is SPX")
args = parser.parse_args()

print(args)

def get_tickers(filename: str) -> dict:
    with open(filename) as f:
        data = f.read()
    tickers = re.findall(
        r'^([A-Z]+):\s*([+-]?\d+(?:\.\d+)?(?:[eE][+-]?\d+)?)',
        data,
        flags=re.MULTILINE
    )
    ticker_dict = {t: float(v) for t, v in tickers}
    return ticker_dict

def get_prices(tickers, start, end):
    data = yf.download(list(tickers), start=start, end=end, progress=False)["Close"]
    return data

def get_sp_prices(start, end, tckr):
    data = yf.download(tckr, start=start, end=end, progress=False)["Close"]
    return data

if __name__ == "__main__":
    if args.file is None:
        print("Error: must provide file to read from with --file FILE")
        exit(1)

    start = args.start or "2025-01-01"
    end = args.end or "2025-04-01"
    cmp_tckr = args.benchmark or "^GSPC"

    portfolio = get_tickers(args.file)
    tickers = list(portfolio.keys())

    # --- Download prices ---
    prices = get_prices(tickers, start, end)
    sp_prices = get_sp_prices(start, end, cmp_tckr)

    # --- Compute daily returns ---
    daily_returns = prices.pct_change().dropna()
    sp_daily_returns = sp_prices.pct_change().dropna()

    # --- Compute weighted portfolio daily return ---
    weights = pd.Series(portfolio)
    weights /= weights.sum()  # normalize to sum to 1 (just in case)
    portfolio_daily_returns = daily_returns.mul(weights, axis=1).sum(axis=1)

    # --- Compute cumulative growth (1 + returns).cumprod() ---
    portfolio_growth = (1 + portfolio_daily_returns).cumprod()
    sp_growth = (1 + sp_daily_returns).cumprod()

    # --- Sharpe Ratio ---
    if args.risk_free is None:
        risk_free_rate = 0.0
    else:
        risk_free_rate = args.risk_free

    trading_days = 252
    portfolio_mean = portfolio_daily_returns.mean()
    portfolio_std = portfolio_daily_returns.std()
    portfolio_sharpe = ((portfolio_mean - risk_free_rate / trading_days) / portfolio_std) * (trading_days ** 0.5)

    sp_mean = sp_daily_returns.mean()
    sp_std = sp_daily_returns.std()
    sp_sharpe = ((sp_mean - risk_free_rate / trading_days) / sp_std) * (trading_days ** 0.5)

    if isinstance(portfolio_sharpe, pd.Series):
        portfolio_sharpe = portfolio_sharpe.iloc[0]
    if isinstance(sp_sharpe, pd.Series):
        sp_sharpe = sp_sharpe.iloc[0]

    portfolio_sharpe = float(portfolio_sharpe)
    sp_sharpe = float(sp_sharpe)

    # --- Print summary stats ---
    total_return = (portfolio_growth.iloc[-1] - 1) * 100
    sp_total_return = (sp_growth.iloc[-1] - 1) * 100

    if isinstance(sp_total_return, pd.Series):
        sp_total_return = sp_total_return.iloc[0]
    sp_total_return = float(sp_total_return)

    print(f"\nPortfolio performance from {start} to {end}: {total_return:.2f}%")
    print(f"{cmp_tckr} performance from {start} to {end}: {sp_total_return:.2f}%")

    print(f"Sharpe Ratio (Portfolio): {portfolio_sharpe:.3f}")
    print(f"Sharpe Ratio ({cmp_tckr}): {sp_sharpe:.3f}")

    # --- Plot ---
    plt.figure(figsize=(10,6))
    plt.plot(portfolio_growth.index, portfolio_growth, label="Portfolio", linewidth=2)
    plt.plot(sp_growth.index, sp_growth, label=cmp_tckr, linewidth=2, linestyle="--")
    plt.title(f"Portfolio vs {cmp_tckr} Performance ({start} to {end})")
    plt.xlabel("Date")
    plt.ylabel("Cumulative Growth (Starting at 1.0)")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()
