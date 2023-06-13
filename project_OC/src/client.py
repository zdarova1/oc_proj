import socket
from encoding import useKey
from time import sleep
import customtkinter as ctk
from os.path import basename
#параметры
host = '192.168.1.17'
port = 1234
buff_size = 1024
private_key, public_key = None, None



def sendFile(client, file_name):
    #client.send(command.encode())
    #print(file_name)
    try:
        file = open(file_name, 'rb')
    except:
        print(f"[CLIENT] Can't open sended file: {file_name}")
    data = file.read(buff_size)
    while data:
        client.send((data))
        data = file.read(buff_size)

    client.send(b'<END>')  
    print('[CLIENT] msg shared!')  
    
def recieveFile(client, file_name):
    
    file = open(file_name, 'wb')
    data = client.recv(buff_size)
    while data[-5:] != b'<END>':
        file.write(data)
        data = client.recv(buff_size)
    
    if(len(data) > 5):
        file.write(data[0:-5])
        

    print("[SERVER]  transfered succesfully")
    
    file.close()
      

def ServerMeet(socket_, command):
    #command = input()
    
    #while command != 'exit':
    s = command.split(' ')
    if s[0] == 'send_file':
       # filepath = ctk.filedialog.askopenfilename()
       # print(filepath)
        socket_.send((s[0]+ ' ' +basename(s[1])).encode())
        file_name = s[1]
        sendFile(socket_, file_name)
    elif s[0] == 'use_key':
        socket_.send(command.encode())
        with open(s[1], 'rb') as f:
            data = f.read(1024)
        socket_.send(data)
        #private_key = useKey(s[1])
        #socket_.send(private_key.encode())
    elif s[0] == 'use_key_pub':
        socket_.send(command.encode())
        with open(s[1], 'rb') as f:
            data = f.read(1024)
        socket_.send(data)
    elif s[0] == 'encode_file':
        socket_.send(command.encode())  
        recieveFile(socket_, s[1]+'.sig')
        
    elif s[0] == 'verify_sign':
        socket_.send(command.encode())
        return socket_.recv(buff_size).decode()
    elif s[0] == 'send_encode_file': #send_encode_file <path>
        socket_.send(command.encode())
        file_name = s[1]
        sendFile(socket_, file_name)
        recieveFile(socket_, s[1]+'.sig')
    elif s[0] == 'send_encode_file_with_key': #send_encode_file <path> <key_path>
        file_name = s[1]
        socket_.send(command.encode())
        #socket_.send(file_name.encode())
        sendFile(socket_, file_name)
        #time.sleep(1)
        with open(s[2], 'rb') as f:
            data = f.read(1024)
        socket_.send(data)
        #time.sleep(1)
        sig_name = socket_.recv(buff_size).decode()
        print(f'Rec sig_name: {sig_name}')
        recieveFile(socket_, sig_name)
    elif s[0] == 'rec_file':
        socket_.send(command.encode())
        file_name = s[1]
        recieveFile(socket_, file_name)
        #command = input()
    elif s[0] == 'exit':
        socket_.send(command.encode())
    return None
    #socket_.close()
#socket_ = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#socket_.connect((host, port))

#ServerMeet(socket_, '')