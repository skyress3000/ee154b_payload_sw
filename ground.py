import serial
import serial.tools.list_ports
import threading
import logging


ACK_BYTE    = b'\xFF' # response indicating a command has been received
ERR_BYTE    = b'\x0F' # response indicating some error with a command (command not found or length does not match data bytes)
DONE_BYTE   = b'\xF0' # response indicating a command has finished running and response data will follow
STAT_BYTE   = b'\x55' # byte indicating payload status message


ser = None
ser_lock = threading.Lock() # idk if the serial object will like being called from multiple threads so lock it just to be sure

command_list: {
	"EXAMPLE_COMMAND": {
		"arg_type": "string", # either "string", "int", or "none"
		"arg_len": 2, # number of bytes the argument should take
		"encoding": b'e' # character representing this command, should match radio.cpp table
	}
}

def serial_logging():
	while True:
		if ser.in_waiting > 0:
			ser_lock.acquire()
			resp_byte = ser.read()
			if(resp_byte == ACK_BYTE) {
				logging.info("Command acknowledged")
			} else if(resp_byte == ERR_BYTE) {
				logging.warning("Error receiving command!")
			} else if(resp_byte == DONE_BYTE) {
				resp_len = ser.read()
				resp_str = ser.read(int.from_bytes(resp_len, 'little'))
				logging.info(f"Command response: {resp_str.decode('ascii')}")
			} else if(resp_byte == STAT_BYTE) {
				stat_str = ser.readline().decode('ascii')
				logging.info(f"Payload status: {stat_str}")
			} else {
				logging.warning(f"Unknown byte received: {int.from_bytes(resp_byte, 'little'):#02x}")
			}
			ser_lock.release()

if __name__ == "__main__":
	format = "[%(levelname)s] %(asctime)s: %(message)s"
	logging.basicConfig(filename='ee154b_HAB_comms.log', format=format, level=logging.INFO)

	ports = serial.tools.list_ports.comports()

	for port, desc, hwid in sorted(ports):
	        if "/dev/cu.usbmodem" in port:
	        	ser = serial.Serial(port)
	        	ser.baudrate = 9600
	        	logging.info("Connecting to {}: {} [{}]".format(port, desc, hwid))

	serial_logging_thread = threading.Thread(target=thread_function, daemon=True)

	print("Available commands:")
	for cmd in command_list.keys():
		print(cmd)

	while True:
		cmd = input("Input command: ")
		if not cmd in command_list.keys():
			logging.warning(f"Unrecognized command! {cmd}")
		else:
			try:
				arg = b''
				if command_list[cmd]["arg_type"] == "string":
					# just truncate to argument length
					arg = input("Argument: ")[:command_list[cmd]["arg_len"]].encode('ascii')
				if command_list[cmd]["arg_type"] == "int":
					arg = int.to_bytes(int(input("Argument: ")), length=command_list[cmd]["arg_len"], byteorder="little")

				# send command
				ser_lock.acquire()
				ser.write(command_list[cmd]["encoding"])
				ser.write(command_list[cmd]["arg_len"])
				ser.write(arg)
				ser_lock.release()

				logging.info(f"Sent command {command_list[cmd]} with argument (raw bytestring) {arg}")

			except Exception as e:
				logging.error(f"Command input error: {str(e)}")