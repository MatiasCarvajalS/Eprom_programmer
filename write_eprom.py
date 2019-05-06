import serial
import time


#Parameters
size_eprom = 1048576
size_eprom = 65536
buffer_size = 64 #el buffer de arduino es de solo 64 bytes
chunk = 0 #contador de que chunk estoy
route_to_file = 'sandbox/testchunk2.smc'

#Initiate serial communication
ser = serial.Serial('COM3', baudrate=115200)             # open serial port
print('Writing to', ser.name)         # check which port was really used
print('Waiting 3 secs to correct initiate ARDUINO') #next time sleep
time.sleep(3)

ser.write('w'.encode('utf-8')) #send the signal to start writing

#loop to write eprom by chunks
with open(route_to_file, "rb") as f:
    while chunk < size_eprom/buffer_size:
        if(ser.in_waiting == 1): #ser.in_waiting espera al arduino complete y le mande una T
            ser.read(1) #para vaciar el in_waiting
            values = f.read(buffer_size) #leo el archivo por chunks
            ser.flush() #vacio el buffer para enviar los 64 bytes perfectamente
            ser.write(values) #mando los valores
            chunk = chunk + 1 #aumento el contador en 1

print("Writed Eprom ... closing port")
ser.close()  # close port


