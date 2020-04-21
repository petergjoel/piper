import fileinput
import sys

sys.stderr.write("Loaded python")

for line in fileinput.input():
    print(line)
    sys.stdout.flush()
