## Agrupa por iteracao

import matplotlib.pyplot as plt

f = open('./remove_dup_on_bin_n_op.csv', 'r')
lines = f.readlines()

count = -1
by_len = {}

for line in lines:
  if count == -1:
    count += 1
    continue
  as_arr = line.split(',')
  if count % 3 == 0:
    by_len[as_arr[13]] = { 'linear': as_arr[10] }
  if count % 3 == 1:
    print(by_len)
    by_len[as_arr[13]]['op'] = as_arr[10] + "[" + as_arr[12]
  if count % 3 == 2:
    by_len[as_arr[13]]['binary']= as_arr[10] + "[" + as_arr[12]
  count += 1

for key, value in by_len.items():
  print(key + ',' + value['linear'] + ',' + value['op'] + ',' + value['binary'])
