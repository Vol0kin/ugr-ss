import time
import sys
import os

programa = sys.argv[1]
salida = sys.argv[2]
num_sim = int(sys.argv[3])

for _ in range(num_sim):
    os.system(f'{programa} {2}| tail -n 13 | head -n 1 | grep -oE "media\([^,]*\)" | grep -oE "[0-9]+\.[0-9]+" >> {salida}')
    time.sleep(1)