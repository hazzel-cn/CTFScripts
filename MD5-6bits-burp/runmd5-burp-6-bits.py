import multiprocessing
from os import urandom
from hashlib import md5
import sys

processor_number = 8


def work(cipher):
    global flag
    for i in xrange(100):
        plain = urandom(16).encode('hex')
        if md5(plain).hexdigest()[:6] == cipher:
            print plain
            flag = False
            sys.exit(0)
            

if __name__ == '__main__':
    cipher = raw_input('md5:')
    print 'Processor Number:', multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=processor_number)
    while True:
        plain = urandom(16).encode('hex')
        pool.apply_async(work, (cipher, ))
    pool.close()
    pool.join()
