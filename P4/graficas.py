import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

# Leer los datos
datos = pd.read_csv(sys.argv[1], names=["x", "y", "t"])
grafica = sys.argv[2]

# Tipos de graficas permitidas
tipos_graficas = ("yx", "txy")

# Comprobar el tipo de grafica
if grafica not in tipos_graficas:
    print(f"Error. Se esperaba que el segundo parametro fuese {tipos_graficas}")
    sys.exit(-1)

# Obtener los datos de forma separada
x = datos["x"].values
y = datos["y"].values
t = datos["t"].values

if grafica == "yx":
    plt.plot(y, x)
else:
    plt.plot(t, x, label="Presas")
    plt.plot(t, y, label="Depredadores")
    plt.legend()

plt.show()