from math import sqrt
from operator import mod

def SieveAtkin(limit):
    is_prime = [False for _ in range(limit + 1)]
    k = int(sqrt(limit)) + 1
    for i in range(k):
        for j in range(k):
            n = (4*(i**2) + (j**2))
            if (n <= limit) and (n % 12 in (1, 5)):
                is_prime[n] = not is_prime[n]
            n = (3*(i**2) + (j**2))
            if (n <= limit) and (n % 12 == 7):
                is_prime[n] = not is_prime[n]
            if (i > j):
                n = (3*(i**2) - (j**2))
                if (n <= limit) and (n % 12 == 11):
                    is_prime[n] = not is_prime[n]

    for num in range(5,k):
        if is_prime[num]:
            for m in range((num**2), limit, (num**2)):
                is_prime[m] = False

    primes = [2, 3]
    for num in range(5, limit):
        if is_prime[num]:
            primes.append(num)
    return primes

print(len(SieveAtkin(10**8)))
