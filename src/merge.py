import sys
import random

COMMUNITY_SIZE = 100
CONSIDERED_SAME = True
# CONSIDERED_SAME = False

f1 = open(sys.argv[1])
f2 = open(sys.argv[2])
X = float(sys.argv[3]) / 100
num_replaced = int(COMMUNITY_SIZE * X)
merged_ids_in_g1 = random.sample(
    [i for i in range(COMMUNITY_SIZE)],
    num_replaced
)
merged_ids_in_g2 = random.sample(
    [i for i in range(COMMUNITY_SIZE)],
    num_replaced
)

merged = open("graph/merged.gr", "w")

merged_ids = {}
for id1, id2 in zip(merged_ids_in_g1, merged_ids_in_g2):
    merged_ids[id2] = id1

max_id = 0
s = set()
for line in f1.readlines():
    left, right = map(int, line.split())
    max_id = max(max_id, left, right)
    if (left, right) in s or (right, left) in s:
        continue
    s.add((left, right))
    line = str(left) + " " + str(right) + "\n"
    merged.write(line)
for line in f2.readlines():
    left, right = map(int, line.split())
    if CONSIDERED_SAME and left in merged_ids:
        left = merged_ids[left]
    else:
        left = left + max_id + 1
    if CONSIDERED_SAME and right in merged_ids:
        right = merged_ids[right]
    else:
        right = right + max_id + 1
    if (left, right) in s or (right, left) in s:
        continue
    s.add((left, right))
    line = str(left) + " " + str(right) + "\n"
    merged.write(line)
if not CONSIDERED_SAME:
    for id1, id2 in zip(merged_ids_in_g1, merged_ids_in_g2):
        id2 = id2 + max_id + 1
        line = str(id1) + " " + str(id2) + "\n"
        merged.write(line)
