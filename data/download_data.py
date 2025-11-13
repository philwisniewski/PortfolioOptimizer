from urllib.request import Request, urlopen
import yfinance as yf
import pandas as pd
import time

# Step 1: Scrape S&P 500 tickers
url = 'https://en.wikipedia.org/wiki/List_of_S%26P_500_companies'
req = Request(
    url,
    headers={"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                           "AppleWebKit/537.36 (KHTML, like Gecko) "
                           "Chrome/115.0 Safari/537.36"}
)
with urlopen(req) as response:
    html = response.read()

tables = pd.read_html(html)
sp500_table = tables[0]
tickers = sp500_table['Symbol'].tolist()

# Fix tickers with dots (BRK.B → BRK-B)
tickers = [t.replace('.', '-') for t in tickers]

# Download historical prices in chunks
def download_chunks(tickers, start, end, chunk_size=5, sleep_time=1):
    all_data = []
    for i in range(0, len(tickers), chunk_size):
        chunk = tickers[i:i+chunk_size]
        print(f"Downloading tickers {i+1}-{i+len(chunk)}...")
        try:
            data = yf.download(chunk, start=start, end=end)["Close"]
            all_data.append(data)
        except Exception as e:
            print(f"Failed to download chunk {i+1}-{i+len(chunk)}: {e}")
        time.sleep(sleep_time)  # throttle requests
    if all_data:
        return pd.concat(all_data, axis=1)
    else:
        return pd.DataFrame()

# Download data
data = download_chunks(tickers, start="2023-10-01", end="2025-10-01")

# Drop columns with missing values
data_clean = data.dropna(axis=1)

# save to csv
data_clean.to_csv("prices.csv")
print("Done! CSV saved as prices.csv")

