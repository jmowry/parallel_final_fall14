import random
import string

def genstr():
    return ''.join(random.choice(string.ascii_lowercase) for x in range(random.randint(1,30)))

def genfile(filename, num):
    l = []
    f = open(filename, 'w')
    for x in range(num):
        string = genstr()
        while string in l:
            string = genstr()
        l.append(string)
        f.write(string)
        f.write('\n')
    f.close()
        
        
