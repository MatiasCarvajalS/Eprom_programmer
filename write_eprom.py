import serial
import time
import sys

#Parameters
size_eprom = 1048576
size_eprom = 65536
route_to_file = 'sandbox/testchunk2.smc'


with open(route_to_file, "rb") as f:
    values = f.read()

if(len(values) != size_eprom):
    sys.exit("Something wrong which size file")

ser = serial.Serial('COM3')             # open serial port
print('Writing to', ser.name)         # check which port was really used
print('Waiting 3 secs to correct initiate ARDUINO')
time.sleep(3)

ser.write('w'.encode('utf-8')) #send the signal to start writing
ser.write(values)              #write data from file
print('Data Send to Arduino')

FIN = ser.read(3) #read finish sentence arduino
if(FIN.decode('utf-8') == 'END'): #finish if arduino stop programming
    print("Correctly programmed")
    ser.close()                    # close port
