import socket
import threading
import time

class IRCClient:
    def __init__(self, server, port, password, nickname, username, realname, channel):
        self.server = server
        self.port = port
        self.password = password
        self.nickname = nickname
        self.username = username
        self.realname = realname
        self.channel = channel
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.server, self.port))
        self.sock.send(f"PASS {self.password}\r\n".encode('utf-8'))
        self.sock.send(f"NICK {self.nickname}\r\n".encode('utf-8'))
        self.sock.send(f"USER {self.username} 0 * :{self.realname}\r\n".encode('utf-8'))
        self.sock.send(f"JOIN {self.channel}\r\n".encode('utf-8'))
        threading.Thread(target=self.receive_messages).start()

    def disconnect(self):
        self.sock.send(f"QUIT :Disconnecting\r\n".encode('utf-8'))
        self.sock.close()

    def receive_messages(self):
        while True:
            try:
                response = self.sock.recv(2048).decode('utf-8')
                if response.startswith("PING"):
                    self.sock.send(f"PONG {response.split()[1]}\r\n".encode('utf-8'))
                else:
                    print(response)
            except socket.error:
                break

def start_clients(server, port, password, channel, nicknames):
    clients = []
    for nickname in nicknames:
        username = nickname
        realname = nickname
        client = IRCClient(server, port, password, nickname, username, realname, channel)
        client.connect()
        clients.append(client)

    # print("Disconnect clients after 10 seconds")
    # time.sleep(10)
    # for client in clients:
    #     client.disconnect()

if __name__ == "__main__":
    server = "localhost"
    port = 6667
    password = "1234"
    channel = "#test"
    nicknames = [f"client{i}"[:101] for i in range(1, 100)]  # 7 clients with nicknames not more than 9 characters long
    start_clients(server, port, password, channel, nicknames)
