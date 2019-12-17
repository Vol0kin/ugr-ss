import numpy as np
from scipy.stats import t

def transform_to_float(input_str):
    floats = list(map(lambda x: float(x), input_str.split()))

    return floats

def calcular_intervalo_confianza(rap, torm, n, alfa):
    dif_rap_torm = rap[:n] - torm[:n]

    media = np.mean(dif_rap_torm)
    desv = np.std(dif_rap_torm)

    print(f"\nMedia: {media}")
    print(f"Desviación típica: {desv}")
    print(f"N: {n}")
    print(f"alfa: {alfa}")

    t_values = t.interval(1-alfa, n-1)

    intervalo = [media + t * np.sqrt(desv/n) for t in t_values]
    print(f"Intervalo obtenido: {intervalo}")

# Leer los ficheros
with open("data/intervalo-rap.txt") as rapido, open("data/intervalo-torm.txt") as tormenta:
    rap = np.array(transform_to_float(rapido.read()))
    torm = np.array(transform_to_float(tormenta.read()))


calcular_intervalo_confianza(rap, torm, 20, 0.05)
calcular_intervalo_confianza(rap, torm, 40, 0.05)
calcular_intervalo_confianza(rap, torm, 60, 0.05)
calcular_intervalo_confianza(rap, torm, 80, 0.05)
calcular_intervalo_confianza(rap, torm, 100, 0.05)
calcular_intervalo_confianza(rap, torm, 120, 0.05)
