from math import sqrt, mod

def SieveAtkin(limit):
    
    is_prime = [False]*(limit-2)
    k = sqrt(limit)
    for i in range(k):
        for j in range(k):
            n = (4*(i**2) + (k**2))
            if (n <= limit) and (mod(n, 12) in (1, 5)):
                is_prime[n] = not is_prime[n]
            n = (3*(i**2) + (k**2))
            if (n <= limi) and (mod(n, 12) == 7):
                is_prime[n] = not is_prime[n]
            n = (3*(i**2) - (k**2))
            if (i > j) and (n <= limit) and (mod(n, 12) == 11):
                is_prime[n] = not is_prime[n]

    for num in range(5,k):
        if is_prime[num]:
            for m in range((num**2), (limit + 1), (num**2)):
                is_prime[m] = False

    primes = [2, 3]
    for num in range(5, limit):
        if is_prime[num]:
            primes.append(num)
    return primes
