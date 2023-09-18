import zmq

context = zmq.Context()
sock = context.socket(zmq.REQ)
print("Connecting to the server...")
sock.connect("tcp://localhost:8080")
print("Connected!")

while True:
    msg = input(">> ").encode('utf-8')
    sock.send(msg)
    if msg == "exit":
        break
    response = sock.recv().decode('utf-8')
    print(response)

sock.close()
context.term()
