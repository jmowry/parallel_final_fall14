import random
import string

def genstr():
    return ''.join(random.choice(string.ascii_lowercase) for x in range(random.randint(1,30)))

def genfile(filename, num):
    f = open(filename, 'w')
    for x in range(num):
        f.write(genstr())
        f.write('\n')
    f.close()
        
