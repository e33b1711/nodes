import socket
import threading
import sys 

if len(sys.argv) != 3: 
    print ("Correct usage: script, IP address, port number")
    exit() 
 
IP_address = str(sys.argv[1]) 
Port = int(sys.argv[2]) 
list_of_clients = [] 
list_master_clients = [] 
 
def handle_client(client_socket):
    while True:
        data = client_socket.recv(1024)
        if not data:
            break
        broadcast(data, client_socket)
        message = data.decode('utf-8')
        print(f"Received message: {message}")
        print(client_socket)
        
    client_socket.close()
 

def broadcast(message, connection): 
    for clients in list_of_clients: 
        if clients!=connection: 
            try: 
                clients.send(message) 
            except: 
                clients.close() 
                remove(clients) 

def remove(connection): 
    if connection in list_of_clients: 
        list_of_clients.remove(connection) 
 
while True: 

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((IP_address, Port))
    server_socket.listen(5)
    print(f"Server listening on {IP_address}:{Port}")

    while True:
        client_socket, client_address = server_socket.accept()
        list_of_clients.append(client_socket) 
        print(f"Accepted connection from {client_address}")
        client_handler = threading.Thread(target=handle_client, args=(client_socket,))
        client_handler.start()

 
conn.close() 
server.close() 