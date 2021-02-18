#A ideia desse doc é comunicar-se com o webserver do esp32 enviando strings.

import socket

s = socket.socket()
host= "192.168.0.119"
port = 80

#pega pelo print no arduino IDE

s.connect((host,port))


while True:
    data = s.recv(1024)

    if data.decode("utf-8") == "quit-1":
        break
        s.close()
        
    if len(data)>0:
        print(data.decode("utf-8"))
        
    msg = input("command: ")
    s.send(str.encode(msg))#Just to send something.
    
    if msg=="quit":
        break
        s.close()
