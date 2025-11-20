import socket
import threading
import time

class IRCClient:
    def __init__(self, server, port, password, nickname, username, channel):
        self.server = server
        self.port = port
        self.password = password
        self.nickname = nickname
        self.username = username
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.server, self.port))
        self.sock.send(f"PASS {self.password}\r\n".encode('utf-8'))
        self.sock.send(f"NICK {self.nickname}\r\n".encode('utf-8'))
        self.sock.send(f"USER {self.username} 0 * :{self.username}\r\n".encode('utf-8'))
        self.sock.send(f"JOIN {self.channel}\r\n".encode('utf-8'))
        threading.Thread(target=self.receive_messages).start()
        # Add a delay to ensure the client has joined the channel
        time.sleep(1)

    def send_message(self, message):
        self.sock.send(f"PRIVMSG {self.channel} :{message}\r\n".encode('utf-8'))

    def receive_messages(self):
        while True:
            response = self.sock.recv(2048).decode('utf-8')
            if response.startswith("PING"):
                self.sock.send(f"PONG {response.split()[1]}\r\n".encode('utf-8'))
            else:
                print(response)

def start_clients(server, port, password, channel, nicknames):
    clients = []
    for nickname in nicknames:
        username = f"user_s"
        client = IRCClient(server, port, password, nickname, username, channel)
        client.connect()
        clients.append(client)
    return clients

def stress_test(clients, message, delay, count):
    for _ in range(count):
        for client in clients:
            client.send_message(message)
        time.sleep(delay)

if __name__ == "__main__":
    server = "localhost"
    port = 6667
    password = "1234"
    channel = "#test"
    nicknames = ["stress"]
    
    clients = start_clients(server, port, password, channel, nicknames)
    
    # Stress test parameters
    message = "This is a test message111111111111111"
    delay = 0.05  # Delay between messages in seconds
    count = 100000  # Number of messages to send

    stress_test(clients, message, delay, count)