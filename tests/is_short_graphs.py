import matplotlib.pyplot as plt

#################
# FETCHING DATA #
#################

f = "./build/results"

"""
21 lines of K
20 lines of P
40 lines of SIGMA
"""

data_K = ([], [], [])
data_P = ([], [], [])
data_SIGMA = ([], [], [])

with open(f, "r") as out:
    # 21 lines of K
    for _ in range(21):
        small = float(out.readline())
        big = float(out.readline())
        K = int(out.readline().split(";")[2])
        data_K[0].append(K)
        data_K[1].append(small)
        data_K[2].append(big)

    # 20 lines of P
    for _ in range(20):
        small = float(out.readline())
        big = float(out.readline())
        P = int(out.readline().split(";")[3])
        data_P[0].append(P)
        data_P[1].append(small)
        data_P[2].append(big)

    # 40 lines of SIGMA
    for _ in range(40):
        small = float(out.readline())
        big = float(out.readline())
        SIGMA = float(out.readline().split(";")[4])
        data_SIGMA[0].append(SIGMA)
        data_SIGMA[1].append(small)
        data_SIGMA[2].append(big)

#################
# PLOTTING DATA #
#################

f_save = "./build/is_short_graphs.png"

fig, (ax_k, ax_p, ax_s) = plt.subplots(1, 3)

label0 = "u = 0"
label1 = "u = 1"

ax_k.plot(data_K[0], data_K[1], label = label0)
ax_k.plot(data_K[0], data_K[2], label = label1)
ax_k.plot(data_K[0], [pow(2, k) for k in data_K[0]], label = "$\mathcal{O}(Q)$")
ax_k.set_yscale("log")
ax_k.set_xlabel("K")
ax_k.set_ylabel("Norm")
ax_k.legend()

ax_p.plot(data_P[0], data_P[1], label = label0)
ax_p.plot(data_P[0], data_P[2], label = label1)
ax_p.plot(data_P[0], [data_P[1][0] * pow(1.5, p) for p in data_P[0]], label = "$\mathcal{O}(P\sqrt{P})$")
ax_p.set_yscale("log")
ax_p.set_xlabel("P")
ax_p.legend()

ax_s.plot(data_SIGMA[0], data_SIGMA[1], label = label0)
ax_s.plot(data_SIGMA[0], data_SIGMA[2], label = label1)
ax_s.plot(data_SIGMA[0], [data_P[1][0] * s * s for s in data_SIGMA[0]], label = "$\mathcal{O}(\sigma^2)$")
ax_s.set_yscale("log")
ax_s.set_xlabel("$\sigma$")
ax_s.legend()

fig.suptitle("Influence of parameters on the norm \nof Dec's output matrix to determine \nwether it is short or not")
fig.tight_layout()

plt.savefig(f_save)
