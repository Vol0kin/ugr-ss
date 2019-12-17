import numpy as np
import math

def transform_to_float(file):
    with open(file) as f:
        input_str = f.read()
        
        floats = list(map(lambda x: float(x), input_str.split()))

    return np.array(floats)

def calcular_w1(h, var, d, n0, N):
    val_1 = 1 - ((N-n0)*d*d)/(h*h*var)
    print(val_1, N/n0)
    val_2 = math.sqrt(1 - (N/n0)*val_1)
    val_3 = 1 + val_2
    val_4 = (n0/N)*val_3

    return val_4 

def calcula_w1_w2(n0, N, d, h1, s):
    val1 = 1 - ((N-n0)*d**2)/(s*h1**2)
    val2 = np.sqrt(1- (N/n0)*val1)
    val3 = 1 + val2

    w1 = (n0/N) * val3
    w2 = 1 - w1

    return w1, w2

atraque4 = transform_to_float("data/k-4.txt")
atraque5 = transform_to_float("data/k-5.txt")
rap = transform_to_float("data/k-rap.txt")
torm = transform_to_float("data/k-torm.txt")

n0 = 40
d = 0.1
h1 = 3.003
k = 4

media_atr4_1 = np.mean(atraque4[:n0])
media_atr5_1 = np.mean(atraque5[:n0])
media_rap_1 = np.mean(rap[:n0])
media_torm_1 = np.mean(torm[:n0])

var_atr4 = np.var(atraque4[:n0], ddof=1)
var_atr5 = np.var(atraque5[:n0], ddof=1)
var_rap = np.var(rap[:n0], ddof=1)
var_torm = np.var(torm[:n0], ddof=1)

N_atr4 = int(max(n0+1, np.round((h1**2 * var_atr4) / d**2)))
N_atr5 = int(max(n0+1, np.round((h1**2 * var_atr5) / d**2)))
N_rap = int(max(n0+1, np.round((h1**2 * var_rap) / d**2)))
N_torm = int(max(n0+1, np.round((h1**2 * var_torm) / d**2)))

media_atr4_2 = np.mean(atraque4[n0:N_atr4])
media_atr5_2 = np.mean(atraque5[n0:N_atr5])
media_rap_2 = np.mean(rap[n0:N_rap])
media_torm_2 = np.mean(torm[n0:N_torm])

w1_atr4, w2_atr4 = calcula_w1_w2(n0, N_atr4, d, h1, var_atr4)
w1_atr5, w2_atr5 = calcula_w1_w2(n0, N_atr5, d, h1, var_atr5)
w1_rap, w2_rap = calcula_w1_w2(n0, N_rap, d, h1, var_rap)
w1_torm, w2_torm = calcula_w1_w2(n0, N_torm, d, h1, var_torm)

xp_atr4 = w1_atr4 * media_atr4_1 + w2_atr4 * media_atr4_2
xp_atr5 = w1_atr5 * media_atr5_1 + w2_atr5 * media_atr5_2
xp_rap = w1_rap * media_rap_1 + w2_rap * media_rap_2  
xp_torm = w1_torm * media_torm_1 + w2_torm * media_torm_2

print(f"n0: {n0}")
print(f"Puntos de atraque: 4 -> media 1: {media_atr4_1:.3f} var: {var_atr4:.8f} N: {N_atr4} media 2: {media_atr4_2:.3f} w1: {w1_atr4:.3f} w2: {w2_atr4:.3f} xp: {xp_atr4:.3f}")
print(f"Puntos de atraque: 5 -> media 1: {media_atr5_1:.3f} var: {var_atr5:.8f} N: {N_atr5} media 2: {media_atr5_2:.3f} w1: {w1_atr5:.3f} w2: {w2_atr5:.3f} xp: {xp_atr5:.3f}")
print(f"Remolcador mas rapido -> media 1: {media_rap_1:.3f} var: {var_rap:.3f} N: {N_rap} media 2: {media_rap_2:.3f} w1: {w1_rap:.3f} w2: {w2_rap:.3f} xp: {xp_rap:.3f}")
print(f"Remolcador inmune a las tormentas -> media 1: {media_torm_1:.3f} var: {var_torm:.3f} N: {N_torm} media 2: {media_torm_2:.3f} w1: {w1_torm:.3f} w2: {w2_torm:.3f} xp: {xp_torm:.3f}")