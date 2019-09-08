import serial
import time


#Parameters
size_eprom = 1048576
size_eprom = 65536
buffer_size = 64 #arduino buffer has 64 bytes only
chunk = 0 #counter to write by chunks
route_to_file = 'sandbox/testchunk2.smc'

#Initiate serial communication
ser = serial.Serial('COM3', baudrate=115200)             # open serial port
print('Writing to', ser.name)         # check which port is used
print('Waiting 3 secs to start ARDUINO') #if i not wait, sometimes ARDUINO works weird
time.sleep(3)

ser.write('w'.encode('utf-8')) #send the signal to start writing

#loop to write eprom by chunks
with open(route_to_file, "rb") as f:
    while chunk < size_eprom/buffer_size:
        if(ser.in_waiting == 1): # wait for arduino to complete and send a "T" (TRUE)
            ser.read(1) #to empty in_waiting
            values = f.read(buffer_size) 
            ser.flush() 
            ser.write(values) 
            chunk = chunk + 1 

print("Finish ... closing port")
ser.close()  # close port


