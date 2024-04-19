import subprocess
from prime import generate_big_prime

f = "./build/results"

with open(f, "w") as out:
    pass

def linspace(a, b, N):
    step = (b - a) / (N - 1)
    return [a + i * step for i in range(N)]

def run_test(args):
    args_str = list(map(str, args))
    with open(f, "a") as out:
        out.write(";".join(args_str))
        out.write("\n")
        subprocess.run(
            ["./build/test_is_short", *args_str],
            stdout=out
        )


# Parameters which stay constant
N = 1
x = 1

# Parameters which vary
K_REF = 30
Q_REF = 1073707009
P_REF = 10
SIGMA_REF = 7.00

# Varying K and Q
P = P_REF
SIGMA = SIGMA_REF
for K in range(10, 30 + 1):
    Q = generate_big_prime(K)
    args = [N, Q, K, P, SIGMA, x]
    run_test(args)

# Varying P
K = K_REF
Q = Q_REF
SIGMA = SIGMA_REF
for P in range(1, 20 + 1):
    args = [N, Q, K, P, SIGMA, x]
    run_test(args)

# Varying SIGMA
K = K_REF
Q = Q_REF
P = P_REF
# do not start from 1.0 as it seems the algorithmF doesn't work
for SIGMA in linspace(2.0, 20.0, 40):
    args = [N, Q, K, P, SIGMA, x]
    run_test(args)
