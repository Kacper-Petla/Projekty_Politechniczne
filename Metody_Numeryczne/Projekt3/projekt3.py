import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))

def Przeskaluj_do(x, a, b): #[-1,1]
    return 2 * (x - a) / (b - a) - 1

def Przeskaluj_z(x, a, b): #[-1,1]
    return 0.5 * (x + 1) * (b - a) + a

def Wybierz_wezly_rownomiernie(df, n): #[-1,1]
    dystans = df['Dystans_Przeskalowany'].values

    target = np.linspace(-1, 1, n)
    indices = [np.abs(dystans - t).argmin() for t in target]

    wezly = df.iloc[indices]

    return wezly

def Chebyshev_wezly(n): #[-1,1]
    k = np.arange(n)
    return np.cos(np.pi * k / (n - 1))

def Wybierz_wezly_chebyshev(df, n): #[-1,1]
    dystans = df['Dystans_Przeskalowany'].values

    target = Chebyshev_wezly(n)
    indices = [np.abs(dystans - t).argmin() for t in target]

    wezly = df.iloc[indices]

    return wezly

def Lagrange(w_x, w_y, x):
    suma = 0
    n = len(w_x)
    for i in range(n):
        iloczyn = w_y[i]
        for j in range(n):
            if i != j:
                iloczyn *= (x - w_x[j]) / (w_x[i] - w_x[j])
        suma += iloczyn
    return suma

def Metoda_Lagrange(wezly, punkty): #[-1,1]
    w_x = wezly['Dystans_Przeskalowany'].values
    w_y = wezly['Wysokosc'].values

    x_l = np.linspace(-1, 1, punkty)
    y_l = [Lagrange(w_x, w_y, x) for x in x_l]

    return x_l, y_l

def Funkcja_sklejalna(x, y):
    n = len(x) - 1
    h = np.diff(x)

    alpha = [0]
    for i in range(1, n):
        alpha_i = (3 / h[i]) * (y[i+1] - y[i]) - (3 / h[i-1]) * (y[i] - y[i-1])
        alpha.append(alpha_i)

    l = np.zeros(n + 1)
    mu = np.zeros(n + 1)
    z = np.zeros(n + 1)

    l[0] = 1
    for i in range(1, n):
        l[i] = 2 * (x[i+1] - x[i-1]) - h[i-1] * mu[i-1]
        mu[i] = h[i] / l[i]
        z[i] = (alpha[i] - h[i-1] * z[i-1]) / l[i]
    l[n] = 1


    a = y.copy()
    b = np.zeros(n)
    c = np.zeros(n + 1)
    d = np.zeros(n)

    for j in reversed(range(n)):
        c[j] = z[j] - mu[j] * c[j+1]
        b[j] = ((a[j+1] - a[j]) / h[j]) - h[j] * (c[j+1] + 2 * c[j]) / 3
        d[j] = (c[j+1] - c[j]) / (3 * h[j])

    return a[:-1], b, c[:-1], d

def Metoda_funkcji_sklejalnych(wezly, punkty):

    w_x = wezly['Dystans_Przeskalowany'].values
    w_y = wezly['Wysokosc'].values

    x_l = np.linspace(-1, 1, punkty)

    a, b, c, d = Funkcja_sklejalna(w_x, w_y)

    y_l = []
    for x in x_l:
        for i in range(len(w_x) - 1):
            if w_x[i] <= x <= w_x[i+1]:
                dx = x - w_x[i]
                y = a[i] + b[i]*dx + c[i]*dx**2 + d[i]*dx**3
                y_l.append(y)
                break
        else:
            dx = x - w_x[-2]
            y = a[-1] + b[-1]*dx + c[-1]*dx**2 + d[-1]*dx**3
            y_l.append(y)
            
    return x_l, np.array(y_l)


l_wezlow = [4,8,12,16,32,48]
test_w = 6

file_path = "MountEverest.csv"
df = pd.read_csv(file_path, sep=',')

plt.figure(figsize=(12, 6))
plt.plot(df['Dystans'], df['Wysokosc'], color='black')
plt.title("Dane wejściowe - MountEverest")
plt.xlabel("Dystans (m)")
plt.ylabel("Wysokosc (m)")
plt.grid()
plt.savefig("1_trasa.jpg", dpi=300, bbox_inches='tight')

df['Dystans_Przeskalowany'] = Przeskaluj_do(df['Dystans'], df['Dystans'].min(), df['Dystans'].max())

#1
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "1_trasa_L_R.jpg"

for i in range(0,test_w):
    wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    x_l, y_l = Metoda_Lagrange(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja Lagrange dla " + str(l_wezlow[i]) + " wezlow wyznaczonych rownomiernie")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')

#2
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "1_trasa_L_C.jpg"
for i in range(0,test_w):
    wezly = Wybierz_wezly_chebyshev(df, l_wezlow[i])
    x_l, y_l = Metoda_Lagrange(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja Lagrange dla " + str(l_wezlow[i]) + " wezlow wyznaczonych metoda Chebysheva")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')


#3
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "1_trasa_S_R.jpg"
for i in range(0,test_w):
    wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    x_l, y_l = Metoda_funkcji_sklejalnych(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja metoda funkcji sklejanych dla " + str(l_wezlow[i]) + " wezlow wyznaczonych rownomiernie")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')

#4
for i in range(0,test_w):
    nazwa_pliku = "1_trasa_S_C_" + str(l_wezlow[i]) + ".jpg"

    #wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    wezly = Wybierz_wezly_chebyshev(df, l_wezlow[i])

    #x_l, y_l = Metoda_Lagrange(wezly, 1000)
    x_l, y_l = Metoda_funkcji_sklejalnych(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    plt.figure(figsize=(12, 6))
    plt.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    plt.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    plt.plot(x_l, y_l, color='red', label ='Aproksymacja')
    plt.title("Interpolacja metoda funkcji sklejanych dla " + str(l_wezlow[i]) + " wezlow wyznaczonych metoda Chebysheva")
    plt.xlabel("Dystans (m)")
    plt.ylabel("Wysokosc (m)")
    plt.legend()
    plt.grid()
    plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')
    #plt.show()














#t2
file_path = "SpacerniakGdansk.csv"
df = pd.read_csv(file_path, sep=',')

plt.figure(figsize=(12, 6))
plt.plot(df['Dystans'], df['Wysokosc'], color='black')
plt.title("Dane wejściowe - SpacerniakGdansk")
plt.xlabel("Dystans (m)")
plt.ylabel("Wysokosc (m)")
plt.grid()
plt.savefig("2_trasa.jpg", dpi=300, bbox_inches='tight')

df['Dystans_Przeskalowany'] = Przeskaluj_do(df['Dystans'], df['Dystans'].min(), df['Dystans'].max())

#1
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "2_trasa_L_R.jpg"

for i in range(0,test_w):
    wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    x_l, y_l = Metoda_Lagrange(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja Lagrange dla " + str(l_wezlow[i]) + " wezlow wyznaczonych rownomiernie")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')

#2
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "2_trasa_L_C.jpg"
for i in range(0,test_w):
    wezly = Wybierz_wezly_chebyshev(df, l_wezlow[i])
    x_l, y_l = Metoda_Lagrange(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja Lagrange dla " + str(l_wezlow[i]) + " wezlow wyznaczonych metoda Chebysheva")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')


#3
fig, axs = plt.subplots(test_w // 2, 2, figsize=(24, 4 * test_w))
nazwa_pliku = "2_trasa_S_R.jpg"
for i in range(0,test_w):
    wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    x_l, y_l = Metoda_funkcji_sklejalnych(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    ax = axs[i // 2][i % 2]
    ax.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    ax.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    ax.plot(x_l, y_l, color='red', label ='Aproksymacja')
    ax.set_title("Interpolacja metoda funkcji sklejanych dla " + str(l_wezlow[i]) + " wezlow wyznaczonych rownomiernie")
    ax.set_xlabel("Dystans (m)")
    ax.set_ylabel("Wysokosc (m)")
    ax.legend()
    ax.grid()

plt.tight_layout()
plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')

#4
for i in range(0,test_w):
    nazwa_pliku = "2_trasa_S_C_" + str(l_wezlow[i]) + ".jpg"

    #wezly = Wybierz_wezly_rownomiernie(df, l_wezlow[i])
    wezly = Wybierz_wezly_chebyshev(df, l_wezlow[i])

    #x_l, y_l = Metoda_Lagrange(wezly, 1000)
    x_l, y_l = Metoda_funkcji_sklejalnych(wezly, 1000)

    x_l = Przeskaluj_z(x_l, df['Dystans'].min(), df['Dystans'].max())

    plt.figure(figsize=(12, 6))
    plt.plot(df['Dystans'], df['Wysokosc'], color='black', label ='Dane rzeczywiste')
    plt.plot(wezly['Dystans'], wezly['Wysokosc'], color='red', marker = 'o', markersize = 5, linestyle='None')
    plt.plot(x_l, y_l, color='red', label ='Aproksymacja')
    plt.title("Interpolacja metoda funkcji sklejanych dla " + str(l_wezlow[i]) + " wezlow wyznaczonych metoda Chebysheva")
    plt.xlabel("Dystans (m)")
    plt.ylabel("Wysokosc (m)")
    plt.legend()
    plt.grid()
    plt.savefig(nazwa_pliku, dpi=300, bbox_inches='tight')
    #plt.show()


