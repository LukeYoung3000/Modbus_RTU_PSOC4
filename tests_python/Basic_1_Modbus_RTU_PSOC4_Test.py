import minimalmodbus

COMPORT = 'COM7'
SLAVE_ADDRESS = 1

# Create instance of minimalmodbus
psoc = minimalmodbus.Instrument(COMPORT, SLAVE_ADDRESS)
# Set Serial Baudrate and Timeout
psoc.serial.timeout = 0.5
psoc.serial.baudrate = 115200

# Read/Write Register 0 - 10
data_write = [100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110]
psoc.write_registers(0, data_write)
print("Data Sent: ")
print(data_write)
data_read = psoc.read_registers(0, 11)
print("Data Read: ")
print(data_read)

data_write = [200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210]
psoc.write_registers(0, data_write)
print("Data Sent: ")
print(data_write)
data_read = psoc.read_registers(0, 11)
print("Data Read: ")
print(data_read)
