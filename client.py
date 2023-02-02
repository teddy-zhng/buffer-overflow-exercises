import sys
import socket

def client_error_wrapper(error_msg):
	print("*"*len(str(error_msg)))
	print(error_msg)
	print("*"*len(str(error_msg)))

class TicTacToe(object):
	"""interact with Stanford ZERO fun/vulnerable tic tac toe server"""
	GET_VERSION_PKT = b"\x03"
	def __init__(self, connect_tuple):
		super(TicTacToe, self).__init__()
		self.connect_tuple = connect_tuple
		self.server_conn = None

	def connect(self):
		try:
			self.server_conn = socket.socket() # default to tcp/ip
			self.server_conn.connect(self.connect_tuple)
		except Exception as e:
			client_error_wrapper(f"error connecting to {self.connect_tuple}")
			raise

	def get_version(self):
		self.server_conn.sendall(GET_VERSION_PKT)
		return self.server_conn.recv(0x1000)

def play(connect_tuple):
	game = TicTacToe(connect_tuple)
	print("connecting")
	game.connect()
	print("connected")
	print()



def main():
	if len(sys.argv) != 3:
		print("Need <tictactoe host ip> <tictactoe host port>")
		return
	
	play(tuple([sys.argv[1], int(sys.argv[2])]))

if __name__ == '__main__':
	main()