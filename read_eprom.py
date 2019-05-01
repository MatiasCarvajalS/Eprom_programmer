import serial
import time
size_eprom = 1048576

ser = serial.Serial('COM3')             # open serial port
print('Reading from', ser.name)         # check which port was really used
print('Waiting 3 secs to correct initiate ARDUINO')
time.sleep(3)

ser.write('r'.encode('utf-8')) #send the signal to start reading
a = ser.read(size_eprom)       # read eprom completely
ser.close()                    # close port

print("Correct Reading")
with open("eprom_data/eprom_data",'wb') as f: #write file, overwrite file
    f.write(a)
print("Correct Writing to file")