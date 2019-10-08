import numpy as np
import matplotlib.pyplot as plt

bars = ('Corto-10%', 'Corto-20%', 'Medio-30%', 'Medio-45%', 'Largo-55%', 'Largo-75%')
height = [1.08575e+06, 1.94286e+06, 1.17389e+06, 1.73191e+06, 1.11183e+06, 1.5473e+06]
y_pos = np.arange(len(bars))

colors = 'rbgmyc'

plt.bar(y_pos, height, color=colors)

plt.xticks(y_pos, bars, rotation=20)

plt.show()
