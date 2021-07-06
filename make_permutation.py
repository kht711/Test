import itertools

f = open("permutation.txt", "w")

seq = []
for i in range(0, 10):
    seq.append(i)

for perm in list(itertools.permutations(seq)):
    for p in perm:
        f.write(str(p))
    f.write("\n")

f.close()
