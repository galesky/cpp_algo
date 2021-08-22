import matplotlib.pyplot as plt

f = open('./out_1kk_needles.txt', 'r')
content = f.readline().split()
plt.hist(content, 5000, rwidth=0.9)
plt.show()