#!/usr/bin/env python

import socket
from struct import *

TCP_IP = '192.172.1.2'
TCP_PORT = 55000
BUFFER_SIZE = 4  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print("Connection address:", addr)
while 1:
    data = conn.recv(BUFFER_SIZE)
    
    #message = unpack('dd', data)
    #print(message)
    print("received data:", data)

    if not data: break
    
    #conn.send(data)  # echo
conn.close()
