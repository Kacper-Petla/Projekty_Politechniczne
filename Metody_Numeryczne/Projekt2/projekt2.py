import numpy as np
import time
import matplotlib.pyplot as plt
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))

MAX_ITERATIONS = 500
TOLERANCE = 1e-9

def Create_Matrix_A(N, a1, a2, a3):
    A = np.zeros((N, N))
    row = [a1, a2, a3]

    for i in range(N):              #A[i,i] główna diagonala
        for j in range(len(row)):   #[i-2],[i-1],[i],[i+1],[i+2]
            if i - j >= 0:
                A[i, i - j] = row[j]
            if i + j < N:
                A[i, i + j] = row[j]
    return A

def Create_Vector_b(N):
    return np.sin(np.arange(1, N + 1) * (8 + 1))

def Solve_Jacobi(N, A, b):
    start = time.time()

    D = np.diag(np.diag(A))
    L = np.tril(A, -1)
    U = np.triu(A, 1)

    M = np.linalg.solve(-D, (L + U))
    w = np.linalg.solve(D, b)
    
    iteration = 0
    r_norm = []
    x = np.zeros(N)

    norm_res = np.linalg.norm(np.dot(A, x) - b)
    r_norm.append(norm_res)
    
    while norm_res > TOLERANCE and iteration < MAX_ITERATIONS:
        iteration += 1
        x = np.dot(M, x) + w
        norm_res = np.linalg.norm(np.dot(A, x) - b)
        r_norm.append(norm_res)

    end = time.time()
    
    return end - start, iteration, r_norm

def Solve_Gauss(N, A, b):
    start = time.time()

    T = np.tril(A)
    U = np.triu(A, 1)

    iteration = 0
    r_norm = []
    x = np.zeros(N)

    norm_res = np.linalg.norm(np.dot(A, x) - b)
    r_norm.append(norm_res)
    
    while norm_res > TOLERANCE and iteration < MAX_ITERATIONS:
        iteration += 1
        x = np.linalg.solve(T, b - np.dot(U, x))
        norm_res = np.linalg.norm(np.dot(A, x) - b)
        r_norm.append(norm_res)

    end = time.time()
    
    return end - start, iteration, r_norm

def Direct_LU(N, A):
    L = np.zeros((N, N))
    U = np.zeros((N, N))
    
    for i in range(N):
        for j in range(i, N):
            U[i, j] = A[i, j] - np.dot(L[i, :i], U[:i, j])
        
        L[i, i] = 1
        for j in range(i + 1, N):
            L[j, i] = (A[j, i] - np.dot(L[j, :i], U[:i, i])) / U[i, i]
    
    return L, U

def Solve_Direct(N, A, b):
    start = time.time()

    L, U = Direct_LU(N, A)
    y = np.linalg.solve(L, b)
    x = np.linalg.solve(U, y)

    end = time.time()
    return end - start, x


#zad A

N = 1286

a1 = 5 + 1
a2 = -1
a3 = -1
A = Create_Matrix_A(N, a1, a2, a3)
b = Create_Vector_b(N)

#zad B

duration_j, iteration_j, r_norm_j = Solve_Jacobi(N, A, b)
print(f"Jacobi: Zakończono po {iteration_j} iteracjach.")
print(f"Jacobi: Czas wykonania: {duration_j:.4f} sekundy")

duration_g, iteration_g, r_norm_g = Solve_Gauss(N, A, b)
print(f"Gauss: Zakończono po {iteration_g} iteracjach.")
print(f"Gauss: Czas wykonania: {duration_g:.4f} sekundy")

plt.semilogy(r_norm_j, label='Jacobi')
plt.semilogy(r_norm_g, label='Gauss-Seidel')
plt.axhline(y=TOLERANCE, color='red', linestyle='--')
plt.title("Norma residuum wzgledem iteracji")
plt.xlabel("Iteracja")
plt.ylabel("Norma residuum")
plt.legend()
plt.tight_layout()
plt.savefig("jacobi_vs_gauss1.jpg", dpi=300, bbox_inches='tight')
plt.show()

#zad C

a1 = 3
a2 = -1
a3 = -1
A = Create_Matrix_A(N, a1, a2, a3)

duration_j, iteration_j, r_norm_j = Solve_Jacobi(N, A, b)
print(f"Jacobi: Zakończono po {iteration_j} iteracjach.")
print(f"Jacobi: Czas wykonania: {duration_j:.4f} sekundy")

duration_g, iteration_g, r_norm_g = Solve_Gauss(N, A, b)
print(f"Gauss: Zakończono po {iteration_g} iteracjach.")
print(f"Gauss: Czas wykonania: {duration_g:.4f} sekundy")

plt.semilogy(r_norm_j, label='Jacobi')
plt.semilogy(r_norm_g, label='Gauss-Seidel')
plt.axhline(y=TOLERANCE, color='red', linestyle='--')
plt.title("Norma residuum wzgledem iteracji")
plt.xlabel("Iteracja")
plt.ylabel("Norma residuum")
plt.legend()
plt.tight_layout()
plt.savefig("jacobi_vs_gauss2.jpg", dpi=300, bbox_inches='tight')
plt.show()

#zad D

duration, x = Solve_Direct(N, A, b)
print(f"Direct: Norma residuum {np.linalg.norm(np.dot(A, x) - b)}.")
print(f"Direct: Czas wykonania: {duration:.4f} sekundy")

#zad E

N = []
for i in range(100, 2001, 100):
    N.append(i)

a1 = 5 + 1
a2 = -1
a3 = -1
Jacobi_Time = []
Gauss_Time = []
Direct_Time = []

for n in N:
    A = Create_Matrix_A(n, a1, a2, a3)
    b = Create_Vector_b(n)

    duration, _, _ = Solve_Jacobi(n, A, b)
    Jacobi_Time.append(duration)

    duration, _, _ = Solve_Gauss(n, A, b)
    Gauss_Time.append(duration)

    duration, _= Solve_Direct(n, A, b)
    Direct_Time.append(duration)

plt.figure(figsize=(10, 5))
plt.plot(N, Jacobi_Time, label='Jacobi')
plt.plot(N, Gauss_Time, label='Gauss-Seidel')
plt.plot(N, Direct_Time, label='Direct LU')
plt.xlabel('Rozmiar macierzy')
plt.ylabel('Czas rozwiązania [s]')
plt.title('Czas rozwiązania vs rozmiar macierzy (skala liniowa)')
plt.legend()
plt.tight_layout()
plt.savefig("compare_linear.jpg", dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(N, Jacobi_Time, label='Jacobi')
plt.plot(N, Gauss_Time, label='Gauss-Seidel')
plt.plot(N, Direct_Time, label='Direct LU')
plt.xlabel('Rozmiar macierzy')
plt.ylabel('Czas rozwiązania [s] (log)')
plt.yscale('log')
plt.title('Czas rozwiązania vs rozmiar macierzy(skala logarytmiczna)')
plt.legend()
plt.tight_layout()
plt.savefig("compare_log.jpg", dpi=300, bbox_inches='tight')
plt.show()

