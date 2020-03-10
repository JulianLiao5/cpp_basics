
import csv
import sys

example_dict1 = {1: 'a', 2: 'b', 3: 'c'}

'''
# ------------------[BEGIN]NOT work--------------
with open('example1.csv', 'wb') as f:
    w = csv.DictWriter(f, example_dict1.keys())
    # w.writerow(example_dict1) ## error: TypeError: a bytes-like object is required, not 'str'
    # writerow()每次只写一行
    # writerows()参数应该是一个dict的list(a list of dicts)，而不是一个dict
    # DictWriter.writeheader()是给csv文件写一个header
    w.writeheader()
    for item in example_dict1:
        # error: AttributeError: 'int' object has no attribute 'keys'
        w.writerow(item)
# ------------------[END]NOT work--------------
'''

''' 
example_dict1 = {"test": 1, "testing": 2}



with open('mycsvfile.csv', 'wb') as f:
    w = csv.DictWriter(f, mydict.keys())
    w.writeheader()
    w.writerow(mydict)
'''

'''
# -------------------[BEGIN] it works----------------------
example_dict2 = {1: 'a', 2: 'b', 3: 'c'}
# example_dict2 = {'Key Header': 'value Header', 'key1': 'value1', 'key2': 'value2'}

with open('example2.csv', 'w') as f:
    writer = csv.writer(f)
    for i in example_dict2:
        writer.writerow([i, example_dict2[i]])
f.close()
# -------------------[END] it works----------------------
# '''

'''
# -------------------[BEGIN] it works----------------------
example_dict2 = {1: 'a', 2: 'b', 3: 'c'}
# example_dict2 = {'Key Header': 'value Header', 'key1': 'value1', 'key2': 'value2'}

with open('example2.csv', 'w') as f:
    writer = csv.writer(f)
    for i in example_dict2:
        writer.writerow([i, example_dict2[i]])
f.close()
# -------------------[END] it works----------------------
# '''

'''
# -------------------[BEGIN] it works----------------------
# example_dict2 = {'Key Header': 'value Header', 'key1': 'value1', 'key2': 'value2'}

with open('example3.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(example_dict1.items())
f.close()
# -------------------[END] it works----------------------
'''

'''
# -------------------[BEGIN] it works----------------------
# example_dict2 = {'Key Header': 'value Header', 'key1': 'value1', 'key2': 'value2'}

with open('example5.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(example_dict1.keys())
    writer.writerow(example_dict1.values())
f.close()
# -------------------[END] it works----------------------
'''

'''
# -------------------[BEGIN] it works----------------------
# example_dict2 = {'Key Header': 'value Header', 'key1': 'value1', 'key2': 'value2'}

with open('example6.csv', 'w') as f:
    for key in example_dict1.keys():
      f.write('%s,%s\n'%(key, example_dict1[key]))
f.close()
# -------------------[END] it works----------------------
# '''


csv_columns = ['Range', 'Angle']
dict_data = [
    {'Range': 0.74, 'Angle': -41.3},
    {'Range': 0.84, 'Angle': -40.3},
    {'Range': 1.03, 'Angle': -39.2}
]
csv_file = 'radar.csv'
try:
    with open(csv_file, 'w') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=csv_columns)
        writer.writeheader()
        for data in dict_data:
            writer.writerow(data)
except IOError:
    print("I/O error")
