
import math

file1 = open('./sidney/6/data.txt', 'r')
chars = file1.read()
file1.close()

required_length = 14

buffer = [c for c in chars[0:required_length]]

for i in range(required_length, len(chars)):

    different = True

    for j in range(0, len(buffer)):
        for k in range(j+1, len(buffer)):
            if buffer[j] == buffer[k]:
                different = False
    
    if different:
        break

    buffer[i % required_length] = chars[i]


print(f'Chars to packet {i}')