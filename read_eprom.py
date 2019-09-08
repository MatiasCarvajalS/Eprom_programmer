import serial
import time
size_eprom = 1048576
size_eprom = 65536


ser = serial.Serial('COM3', baudrate= 115200)             # open serial port
print('Reading from', ser.name)         # check which port is used
print('Waiting 3 secs to start ARDUINO') #if i not wait, sometimes ARDUINO works weird
time.sleep(3)

ser.write('r'.encode('utf-8')) #send the signal to start reading
a = ser.read(size_eprom)       # read eprom completely
ser.close()                    # close port

print("Reading...")
with open("eprom_data/eprom_data",'wb') as f: #write file, overwrite file
    f.write(a)
print("Write has been written to eprom_data file")