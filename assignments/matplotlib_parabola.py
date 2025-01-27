import matplotlib.pyplot as plt

plt.plot([x**2 for x in range(100)])  # list comprehension
plt.ylabel("some numbers")
plt.show()
