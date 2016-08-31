#!/usr/bin/env python 
from __future__ import print_function

def net_stat():
    net = {}
    f = open("/proc/net/dev")
    lines = f.readlines()
    f.close
    for line in lines[2:]:
        line = line.split(":")
        eth_name = line[0].strip()
        if eth_name != 'lo':
            net_io = {}
            net_io['receive'] = round(float(line[1].split()[0]) / (1024.0 * 1024.0),2)
            net_io['transmit'] = round(float(line[1].split()[8]) / (1024.0 * 1024.0),2)
            net[eth_name] = net_io
    return net

if __name__ == '__main__':
    netdevs = net_stat()
    print(netdevs)