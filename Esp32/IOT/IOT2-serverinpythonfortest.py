import socket
import sys



def create_socket():#creates a socket (connect two machines)
    try:
        global host
        global port
        global s
        host = ""
        port = 9999
        s = socket.socket()
    except socket.error as msg:
        print("Socket creating error" + str(msg))
        
def bind_socket():
    try:
        global host
        global port
        global s
        print("Binding the Port " + str(port))
        s.bind((host,port))
        s.listen(5)
    except socket.error as msg:
        print("Socket creating error" + str(msg) + "\nRetrying..." )
        bind_socket()
        
        
def socket_accept():
    conn,adress = s.accept()
    print("Connection has been stablished...\IP:" +  adress[0] + " \PORT " + str(adress[1]))
    send_command(conn)
    conn.close()
    
def send_command(conn):
    while True:
        cmd = input()
        if cmd=="quit":
            conn.send(str.encode("quit-1"))
            break
            s.close()
            sys.exit()
        if len(str.encode(cmd))>0:#If you had typed something...
            conn.send(str.encode(cmd))#send the string encoded as a byte command
            client_response = str(conn.recv(1024),"utf-8")
            print(client_response,end = "")
            
def main():
    create_socket()
    bind_socket()
    socket_accept()




    
main()
    
    
    
    
            