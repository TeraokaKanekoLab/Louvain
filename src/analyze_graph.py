import sys

f = open(sys.argv[1])
counter = {}
for i in range(6):
    for j in range(6):
        counter[(i, j)] = 0
s = set()
for line in f.readlines():
    left, right = map(int, line.split())
    if (left, right) in s or (right, left) in s:
        print("duplicate")
        continue
    s.add((left, right))
    left_community = left // 100
    right_community = right // 100
    counter[(left_community, right_community)] += 1
    if left_community != right_community:
        counter[(right_community, left_community)] += 1

# for i in range(6):
#     for j in range(6):
#         cnt = str(counter[(i, j)])
#         print(" " * (4 - len(cnt)) + cnt, end=' ')
#     print()

print("||C1|C2|C3|C4|C5|C6|")
print("|:--:|:--:|:--:|:--:|:--:|:--:|:--:|")
for i in range(6):
    print('|C' + str(i + 1) + '|', end='')
    for j in range(6):
        cnt = str(counter[(i, j)])
        print(cnt, end='|')
    print()

print("," + str(counter[(0, 3)]))
