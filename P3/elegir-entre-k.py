import numpy as np

def transform_to_float(file):
    with open(file) as f:
        input_str = f.read()
        
        floats = list(map(lambda x: float(x), input_str.split()))

    return np.array(floats)


def calcula_w1_w2(n0, Ni, d, h1, s):
    n0_Ni = n0/Ni
    Ni_n0 = Ni/n0

    upper = (Ni-n0)*d**2
    down = s*h1**2
    inner = int(1 - upper/down)
    print(inner)

    squared = 1 - Ni_n0 * inner
    root = np.sqrt(squared)
    print(squared)

    w1 = n0_Ni * (1 + root)
    w2 = 1 - w1

    return w1, w2

atraque4 = transform_to_float("data/k-4.txt")
atraque5 = transform_to_float("data/k-5.txt")
rap = transform_to_float("data/k-rap.txt")
torm = transform_to_float("data/k-torm.txt")

n0 = 20
d = 0.1
h1 = 3.101
k = 4

media_atr4_1 = np.mean(atraque4[:n0])
media_atr5_1 = np.mean(atraque5[:n0])
media_rap_1 = np.mean(rap[:n0])
media_torm_1 = np.mean(torm[:n0])

desv_atr4 = np.std(atraque4[:n0])
desv_atr5 = np.std(atraque5[:n0])
desv_rap = np.std(rap[:n0])
desv_torm = np.std(torm[:n0])

N_atr4 = int(max(n0+1, np.round((h1**2 * desv_atr4) / d**2)))
N_atr5 = int(max(n0+1, np.round((h1**2 * desv_atr5) / d**2)))
N_rap = int(max(n0+1, np.round((h1**2 * desv_rap) / d**2)))
N_torm = int(max(n0+1, np.round((h1**2 * desv_torm) / d**2)))

print(f"n0: {n0}")
print(f"Puntos de atraque: 4 -> media 1: {media_atr4_1:.3f} std: {desv_atr4:.3f} N: {N_atr4}")
print(f"Puntos de atraque: 5 -> media 1: {media_atr5_1:.3f} std: {desv_atr5:.3f} N: {N_atr5}")
print(f"Remolcador mas rapido -> media 1: {media_rap_1:.3f} std: {desv_rap:.3f} N: {N_rap}")
print(f"Remolcador inmune a las tormentas -> media 1: {media_torm_1:.3f} std: {desv_torm:.3f} N: {N_torm}")

media_atr4_2 = np.mean(atraque4[n0:N_atr4])
media_atr5_2 = np.mean(atraque5[n0:N_atr5])
media_rap_2 = np.mean(rap[n0:N_rap])
media_torm_2 = np.mean(torm[n0:N_torm])

print(f"Puntos de atraque: 4 -> media 2: {media_atr4_2:.3f}")
print(f"Puntos de atraque: 5 -> media 2: {media_atr5_2:.3f}")
print(f"Remolcador mas rapido -> media 2: {media_rap_2:.3f}")
print(f"Remolcador inmune a las tormentas -> media 2: {media_torm_2:.3f}")

w1_atr4, w2_atr4 = calcula_w1_w2(n0, N_atr4, d, h1, desv_atr4)
w1_atr5, w2_atr5 = calcula_w1_w2(n0, N_atr5, d, h1, desv_atr5)
w1_rap, w2_rap = calcula_w1_w2(n0, N_rap, d, h1, desv_rap)
w1_torm, w2_torm = calcula_w1_w2(n0, N_torm, d, h1, desv_torm)

print(f"Puntos de atraque: 4 -> w1: {w1_atr4:.3f} w2: {w2_atr4:.3f}")
print(f"Puntos de atraque: 5 -> w1: {w1_atr5:.3f} w2: {w2_atr5:.3f}")
print(f"Remolcador mas rapido -> w1: {w1_rap:.3f} w2: {w2_rap:.3f}")
print(f"Remolcador inmune a las tormentas -> w1: {w1_torm:.3f} w2: {w2_torm:.3f}")

