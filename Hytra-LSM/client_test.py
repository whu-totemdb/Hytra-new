import socket
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client.connect(('127.0.0.1',9200))

client.send("put:2022-03-11@18438@0,0000000".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000003".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000004".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000005".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000006".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("get:2022-03-11@18438@0".encode('utf-8'))
msg = client.recv(10000)
print(msg)

client.send("put:2022-03-11@18438@0,0000007".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000008".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("put:2022-03-11@18438@0,0000009".encode('utf-8'))
msg = client.recv(10000)
print(msg)

client.send("get:2022-03-11@18438@0".encode('utf-8'))
msg = client.recv(10000)
print(msg)
client.send("exit".encode('utf-8'))
msg = client.recv(10000)
print(msg)

client.close()