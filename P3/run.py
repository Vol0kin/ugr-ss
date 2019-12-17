import os
import time

# Lista de archivos
originals=("data/original1.txt", "data/original5.txt", "data/original10.txt", "data/original25.txt", "data/original50.txt")
storms=("data/storm1.txt", "data/storm5.txt", "data/storm10.txt", "data/storm25.txt", "data/storm50.txt")
faster=("data/fast1.txt", "data/fast5.txt", "data/fast10.txt", "data/fast25.txt", "data/fast50.txt")

programs = ("bin/puerto-tons", "bin/puerto-tor", "bin/puerto-rem")

num_simulaciones = (1, 5, 10, 25, 50)

# Limpiar archivos
for o, s, f in zip(originals, storms, faster):
    open(o, mode="w").close()
    open(s, mode="w").close()
    open(f, mode="w").close()

# Simular
for group, p in zip((originals, storms, faster), programs):
    for f, n in zip(group, num_simulaciones):
        for i in range(100):
            print(f"Simulando: {p} {n} repeticiones. Salida en {f}. Progreso {i+1}/100")
            os.system(f'{p} {n}| tail -n 13 | head -n 1 | grep -oE "media\([^,]*\)" | grep -oE "[0-9]+\.[0-9]+" >> {f}')
            if n < 100:
                time.sleep(1)

