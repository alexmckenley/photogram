from cobs import cobs
import serial


ser = serial.Serial('COM6', 115200, timeout=2) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish

# Initializaiton
while True:
    # Send \xFF
    init = cobs.encode(b'\xFF')
    ser.write(init)
    ser.write(b'\x00')
    encoded_resp = ser.read_until(b'\x00')
    resp = cobs.decode(encoded_resp[:len(encoded_resp)-1])
    # Once \xFF is received, communication is established
    if (resp == b'\xff'):
        break

# Program loop
while True:
     counter +=1
     raw = bytearray(b'\xAA\xAA')
     print('sending:')
     print([x for x in raw])
     encoded = cobs.encode(raw)
     print([x for x in encoded])
     ser.write(encoded) # Convert the decimal number to ASCII then send it to the Arduino
     ser.write(bytearray(b'\x00'))
     encoded_resp = ser.read_until(b'\x00')
     print('resp:')
     print([x for x in encoded_resp])
    #  print("{0:b}".format(encoded_resp[0]))
     resp = cobs.decode(encoded_resp[:len(encoded_resp)-1])
     print([x for x in resp])
     if counter == 39:
        break