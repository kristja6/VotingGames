from numpy import random

n = int(input())
q = int(input())
loc = int(input())
scale = int(input())
print(n, q)

for i in range(n):
  print(round(max(0, random.normal(loc, scale))))
