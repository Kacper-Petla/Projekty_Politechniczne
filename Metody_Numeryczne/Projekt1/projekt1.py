import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def EMA(data, N):                                               #Obliczanie EMA
    alpha = 2 / (N + 1)
    ema_values = [data[0]]

    for i in range(1, len(data)):
        ema_i = alpha * data[i] + (1 - alpha) * ema_values[-1]
        ema_values.append(ema_i)

    return ema_values


def MACD(data):                                                 #Obliczanie MACD
    ema_12 = np.array(EMA(data, 12))
    ema_26 = np.array(EMA(data, 26))

    return ema_12 - ema_26


def SIGNAL(data):                                               #Obliczanie SIGNAL
    return EMA(data, 9)

def Buy_Sell_Points(macd, signal):                              #Indeksy przecinania się MACD i SIGNAL
    buy_sell_points = ['NONE']

    for i in range(1, len(macd)):
        if macd[i-1] < signal[i-1] and macd[i] > signal[i]:     #Kupno
            buy_sell_points.append('BUY')
        elif macd[i-1] > signal[i-1] and macd[i] < signal[i]:   #Sprzedarz
            buy_sell_points.append('SELL')
        else:
            buy_sell_points.append('NONE')

    return buy_sell_points

def Simulation(price, buy_sell_points):                         #Symulacja wykożystania wskaźnika MACD                 
    capital = 1000
    shares = 0
    capital_history = []
    shares_buy_sell_points = []
    successful_trades = 0
    failed_trades = 0
    buy_price = 0 
    for _ in range(35):                                         #Pomijamy niepoprawne dane
        capital_history.append(capital)
        shares_buy_sell_points.append('NONE')

    for i in range(35, len(price)):
        if buy_sell_points[i] == 'BUY' and shares == 0:
            shares = capital / price[i]
            buy_price = price[i]
            capital = 0
            shares_buy_sell_points.append('BUY')
        elif buy_sell_points[i] == 'SELL' and shares > 0: 
            capital = shares * price[i]
            if price[i] > buy_price:
                successful_trades += 1
            else:
                failed_trades += 1
            shares = 0
            shares_buy_sell_points.append('SELL')
        else:
            shares_buy_sell_points.append('NONE')
        
        total_value = capital + (shares * price[i])
        capital_history.append(total_value)

    print(f"🔹 Udane transakcje: {successful_trades}")
    print(f"🔹 Nieudane transakcje: {failed_trades}")
    print(f"🔹 Końcowy kapitał: {capital:.2f}")

    return capital_history ,shares_buy_sell_points
    

#wczytujemy dane
file_path = "cdr_d.csv"
df = pd.read_csv(file_path, sep=',')

#ustawiamy poprawny format daty
df['Data'] = pd.to_datetime(df['Data'])
df.set_index('Data', inplace=True)

#obliczamy macd i signal
df['MACD'] = MACD(df['Zamkniecie'])
df['Signal'] = SIGNAL(df['MACD'])

#szukamy indeksy przecięć macd i signal
df['Buy_Sell'] = Buy_Sell_Points(df['MACD'], df['Signal'])

#konwersja do wykresu
df_buy = df[df['Buy_Sell'] == 'BUY']
df_sell = df[df['Buy_Sell'] == 'SELL']

#wykres Notowań Instrumentu Finansowego
plt.figure(figsize=(12, 6))
plt.plot(df['Zamkniecie'], label='Cena', color='black')
plt.title("Ceny akcji CD Projekt SA")
plt.xlabel("Data")
plt.ylabel("Cena [zł]")
plt.legend()
plt.grid()
plt.savefig("wykres_akcji.jpg", dpi=300, bbox_inches='tight')
plt.show()

#wykres do analizy Notowań Instrumentu Finansowego
plt.figure(figsize=(12, 6))
plt.plot(df['Zamkniecie'], label='Cena', color='black')
plt.title("Ceny akcji CD Projekt SA")
plt.scatter(df_buy.index, df_buy['Zamkniecie'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_sell.index, df_sell['Zamkniecie'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedaż")
plt.xlabel("Data")
plt.ylabel("Cena [zł]")
plt.legend()
plt.grid()
plt.savefig("wykres_akcji_macd.jpg", dpi=300, bbox_inches='tight')
plt.show()

#wykres macd i signal
plt.figure(figsize=(12, 6))
plt.plot(df['MACD'], label='MACD', color='blue')
plt.plot(df['Signal'], label='Signal', color='red')
plt.axhline(y=0, color='black', linestyle='--', linewidth=0.8)
plt.scatter(df_buy.index, df_buy['MACD'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_sell.index, df_sell['MACD'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedaż")
plt.xlabel("Data")
plt.legend()
plt.title('Wskaźnik MACD')
plt.savefig("wykres_macd.jpg", dpi=300, bbox_inches='tight')
plt.show()



#ograniczony wykres macd i signal
zoom_dates = (df.index >= '2023-02-15') & (df.index <= '2023-09-19')
df_zoom = df[zoom_dates]
df_zoom_buy = df_zoom[df['Buy_Sell'] == 'BUY']
df_zoom_sell = df_zoom[df['Buy_Sell'] == 'SELL']
best_buy_point = df_zoom_buy.loc[df_zoom_buy['MACD'].idxmin()] 
best_sell_point = df_zoom_sell.loc[df_zoom_sell['MACD'].idxmax()]
#df_zoom_sell_after_buy = df_zoom_sell[df_zoom_sell.index > best_buy_point.name]
#best_sell_point = df_zoom_sell_after_buy.iloc[df_zoom_sell_after_buy['MACD'].idxmax()]


plt.figure(figsize=(12, 6))
plt.plot(df_zoom.index, df_zoom['MACD'], label='MACD', color='blue')
plt.plot(df_zoom.index, df_zoom['Signal'], label='Signal', color='red')
plt.axhline(y=0, color='black', linestyle='--', linewidth=0.8)
plt.scatter(df_zoom_buy.index, df_zoom_buy['MACD'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_zoom_sell.index, df_zoom_sell['MACD'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedarz")
plt.scatter(best_buy_point.name, best_buy_point['MACD'], color='yellow', marker='^', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Kupno")
plt.scatter(best_sell_point.name, best_sell_point['MACD'], color='yellow', marker='v', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Sprzedarz")
plt.text(best_buy_point.name, best_buy_point['MACD'] + 1, f"{best_buy_point['Zamkniecie']:.2f}", fontsize=10, color='black', ha='center', va='top')
plt.text(best_sell_point.name, best_sell_point['MACD'] + 1, f"{best_sell_point['Zamkniecie']:.2f}", fontsize=10, color='black', ha='center', va='top')
plt.xlabel("Data")
plt.legend()
plt.title(f'Wskaźnik MACD (2023-02-15 - 2023-09-19)')
plt.savefig("wykres_macd_zoom1.jpg", dpi=300, bbox_inches='tight')
plt.show()


#wykres do analizy Notowań Instrumentu Finansowego
plt.figure(figsize=(12, 6))
plt.plot(df_zoom['Zamkniecie'], label='Cena', color='black')
plt.title("Ceny akcji CD Projekt SA (2023-02-15 - 2023-09-19)")
plt.scatter(df_zoom_buy.index, df_zoom_buy['Zamkniecie'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_zoom_sell.index, df_zoom_sell['Zamkniecie'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedarz")
plt.scatter(best_buy_point.name, best_buy_point['Zamkniecie'], color='yellow', marker='^', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Kupno")
plt.scatter(best_sell_point.name, best_sell_point['Zamkniecie'], color='yellow', marker='v', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Sprzedarz")
plt.xlabel("Data")
plt.ylabel("Cena [zł]")
plt.legend()
plt.grid()
plt.savefig("wykres_akcji_macd_zoom1.jpg", dpi=300, bbox_inches='tight')
plt.show()


#ograniczony wykres macd i signal
zoom_dates = (df.index >= '2021-02-15') & (df.index <= '2021-07-19')
df_zoom = df[zoom_dates]
df_zoom_buy = df_zoom[df['Buy_Sell'] == 'BUY']
df_zoom_sell = df_zoom[df['Buy_Sell'] == 'SELL']
best_buy_point = df_zoom_buy.loc[df_zoom_buy['MACD'].idxmin()] 
best_sell_point = df_zoom_sell.loc[df_zoom_sell['MACD'].idxmax()]
#df_zoom_sell_after_buy = df_zoom_sell[df_zoom_sell.index > best_buy_point.name]
#best_sell_point = df_zoom_sell_after_buy.iloc[df_zoom_sell_after_buy['MACD'].idxmax()]


plt.figure(figsize=(12, 6))
plt.plot(df_zoom.index, df_zoom['MACD'], label='MACD', color='blue')
plt.plot(df_zoom.index, df_zoom['Signal'], label='Signal', color='red')
plt.axhline(y=0, color='black', linestyle='--', linewidth=0.8)
plt.scatter(df_zoom_buy.index, df_zoom_buy['MACD'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1.5, label="Kupno")
plt.scatter(df_zoom_sell.index, df_zoom_sell['MACD'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1.5, label="Sprzedarz")
plt.scatter(best_buy_point.name, best_buy_point['MACD'], color='yellow', marker='^', s=150, zorder=4, edgecolors='black', linewidth=2, label="Best_Kupno")
plt.scatter(best_sell_point.name, best_sell_point['MACD'], color='yellow', marker='v', s=150, zorder=4, edgecolors='black', linewidth=2, label="Best_Sprzedarz")
plt.text(best_buy_point.name, best_buy_point['MACD'] + 1, f"{best_buy_point['Zamkniecie']:.2f}", fontsize=10, color='black', ha='center', va='top')
plt.text(best_sell_point.name, best_sell_point['MACD'] + 1, f"{best_sell_point['Zamkniecie']:.2f}", fontsize=10, color='black', ha='center', va='top')
plt.xlabel("Data")
plt.legend()
plt.title(f'Wskaźnik MACD (2021-02-15 - 2021-07-19)')
plt.savefig("wykres_macd_zoom2.jpg", dpi=300, bbox_inches='tight')
plt.show()


#wykres do analizy Notowań Instrumentu Finansowego
plt.figure(figsize=(12, 6))
plt.plot(df_zoom['Zamkniecie'], label='Cena', color='black')
plt.title("Ceny akcji CD Projekt SA (2021-02-15 - 2021-07-19)")
plt.scatter(df_zoom_buy.index, df_zoom_buy['Zamkniecie'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_zoom_sell.index, df_zoom_sell['Zamkniecie'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedarz")
plt.scatter(best_buy_point.name, best_buy_point['Zamkniecie'], color='yellow', marker='^', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Kupno")
plt.scatter(best_sell_point.name, best_sell_point['Zamkniecie'], color='yellow', marker='v', s=150, zorder=4, edgecolors='black', linewidth=1.5, label="Best_Sprzedarz")
plt.xlabel("Data")
plt.ylabel("Cena [zł]")
plt.legend()
plt.grid()
plt.savefig("wykres_akcji_macd_zoom2.jpg", dpi=300, bbox_inches='tight')
plt.show()


#przeprowadzenie symulacji macd
df['Symulation'], df['Buy_Sell_Shares']= Simulation(df['Zamkniecie'], df['Buy_Sell'])

df_buy = df[df['Buy_Sell_Shares'] == 'BUY']
df_sell = df[df['Buy_Sell_Shares'] == 'SELL']

# Wykres symulacji macd
plt.figure(figsize=(12, 6))
plt.plot(df.index, df['Symulation'], label='Kapitał w czasie', color='green')
plt.scatter(df_buy.index, df_buy['Symulation'], color='green', marker='^', s=50, zorder=3, edgecolors='black', linewidth=1, label="Kupno")
plt.scatter(df_sell.index, df_sell['Symulation'], color='red', marker='v', s=50, zorder=3, edgecolors='black', linewidth=1, label="Sprzedaż")
plt.title("Symulacja kapitału na podstawie MACD")
plt.xlabel("Data")
plt.ylabel("Kapitał [zł]")
plt.legend()
plt.grid()
plt.savefig("wykres_symulacji.jpg", dpi=300, bbox_inches='tight')
plt.show()