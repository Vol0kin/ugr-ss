import numpy as np

def transform_to_float(input_str):
    floats = list(map(lambda x: float(x), input_str.split()))

    return floats

def mostrar_resultados(arch1, arch2, simluaciones):
    for a1, a2, n in zip(arch1, arch2, simluaciones):
        with open(a1) as f1, open(a2) as f2:
            f1_content = f1.read()
            f2_content = f2.read()

            f1_values = np.array(transform_to_float(f1_content))
            f2_values = np.array(transform_to_float(f2_content))

            mejor_f1 = np.count_nonzero(f1_values < f2_values)
            mejor_f2 = np.count_nonzero(f2_values < f1_values)

            print(f"{mejor_f1}% {mejor_f2}% {n} simulaciones")

# Archivos de los que leer
originals=("data/original1.txt", "data/original5.txt", "data/original10.txt", "data/original25.txt", "data/original50.txt")
storms=("data/storm1.txt", "data/storm5.txt", "data/storm10.txt", "data/storm25.txt", "data/storm50.txt")
faster=("data/fast1.txt", "data/fast5.txt", "data/fast10.txt", "data/fast25.txt", "data/fast50.txt")

num_simulaciones = (1, 5, 10, 25, 50)

print("Base-Tormentas-Num. simulaciones")
mostrar_resultados(originals, storms, num_simulaciones)

print("Base-Remolcador rapido-Num. simulaciones")
mostrar_resultados(originals, faster, num_simulaciones)
