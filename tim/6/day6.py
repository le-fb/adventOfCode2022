with open("input.txt", "r") as f:
    file = f.read()

#print(file)
def find_marker(marker_len):
    buffer = []
    for ind, i in enumerate(file, start=1):
        buffer.append(i)
        if len(buffer) > marker_len:
            buffer.pop(0)
            if len(set(buffer)) == marker_len:
                return ind

print(f"First start-of-packet marker is at {find_marker(4)}")
print(f"First start-of-message marker marker is at {find_marker(14)}")
