import matplotlib.pyplot as plt
import struct
import numpy as np
from matplotlib.animation import FuncAnimation
from collections import deque
import threading

fps = 30
maxlen = 180
dataQueue = deque([0]*maxlen, maxlen=180)
pipe_path = r'\\.\pipe\pipePython'

def readData(pipe_path):
    print("Attente du serveur C++...")
    with open(pipe_path, 'rb') as pipe:
        while True:
            try:
                data = pipe.read(4)
                if not data:
                    break
                valeur = struct.unpack('f', data)[0]
                dataQueue.append(valeur)
            except Exception as e:
                print(f"Connexion interrompue : {e}")
                break


def drawData():
    fig, ax = plt.subplots()
    line, = ax.plot(np.zeros(maxlen))
    line_min, = ax.plot(np.zeros(maxlen), 'r--', label='Min', linewidth=0.5)
    line_max, = ax.plot(np.zeros(maxlen), 'g--', label='Max',  linewidth=0.5)
    
    ax.set_ylim(-2, 2)
    ax.set_xlim(0, maxlen)
    ax.set_title('Evolution de l\'angle $\\theta$ par rapport au temps')
    ax.set_xlabel(f'{maxlen} dernières snapshots (intervalle entre chaque snapshot : {1000/fps:.1f} ms)')
    ax.set_ylabel(f'$\\theta$ en radian')
    ax.legend(loc='upper right')

    def update(frame):

        dataQueueArray = np.array(dataQueue)
        min_val = np.min(dataQueueArray)
        max_val = np.max(dataQueueArray)

        line_min.set_ydata(np.full(maxlen, min_val))
        line_max.set_ydata(np.full(maxlen, max_val))
        line.set_ydata(dataQueueArray)

        return line,line_min,line_max
    
    ani = FuncAnimation(fig, update, interval=100, blit=True, cache_frame_data=False)
    plt.show()
           
    


if __name__ == "__main__":
    threading.Thread(target=readData, args=(pipe_path,), daemon=True).start()
    drawData()
    