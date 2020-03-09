import random
import math
n = int(input())
q = int(input())
mn = int(input())
mx = int(input())
print(n, q)

for i in range(n):
  print(random.randrange(mn, mx))
