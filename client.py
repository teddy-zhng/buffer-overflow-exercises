import sys
import socket

def client_error_wrapper(error_msg):
	print("*"*len(str(error_msg)))
	print(error_msg)
	print("*"*len(str(error_msg)))

NET_ORDER = "big"

class TicTacToe(object):
	"""interact with Stanford ZERO fun/vulnerable tic tac toe server"""
	GET_VERSION_PKT = b"\x00"
	def __init__(self, connect_tuple, debug = True):
		super(TicTacToe, self).__init__()
		self.connect_tuple = connect_tuple
		self.server_conn = None
		self.debug = debug

	def read_resposne(self):
		resp_len = self.server_conn.recv(4)
		if len(resp_len) != 4:
			client_error_wrapper("unable to read resposne length")
			raise Exception("need at least 4 bytes in resposne")
		resp_len = int.from_bytes(resp_len, NET_ORDER)
		if self.debug:
			print("getting msg resposne len:", resp_len)

		resp_data = self.server_conn.recv(resp_len)
		return resp_data

	def connect(self):
		try:
			self.server_conn = socket.socket() # default to tcp/ip
			self.server_conn.connect(self.connect_tuple)
		except Exception as e:
			client_error_wrapper(f"error connecting to {self.connect_tuple}")
			raise

	def get_version(self):
		self.server_conn.sendall(TicTacToe.GET_VERSION_PKT)
		return self.read_resposne()

def play(connect_tuple):
	game = TicTacToe(connect_tuple)
	print("connecting")
	game.connect()
	print("connected")
	print(game.get_version())



def main():
	if len(sys.argv) != 3:
		print("Need <tictactoe host ip> <tictactoe host port>")
		return
	
	play(tuple([sys.argv[1], int(sys.argv[2])]))

if __name__ == '__main__':
	main()