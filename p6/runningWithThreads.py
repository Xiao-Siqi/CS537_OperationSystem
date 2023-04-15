import matplotlib.pyplot as plt

plt.figure(figsize = (20,10), dpi = 100)
plt.plot([1,2,4,8,12,16],[196.78,262.24,168.81,24.52,23.99,22.05],linewidth = '1')
plt.title('time spent with different numbers of threads')
plt.ylabel('Time(sec)')
plt.xlabel("Threads Count")
plt.savefig("p6graph.png")
plt.show()
