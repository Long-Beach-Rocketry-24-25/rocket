from serializer import Serializer   #this is dumb



if __name__ == "__main__":
    ser = Serializer(0)
    print("hi")
    ports = ser.get_ports()
    index = 0
    for port in ports:
        print(f"Device {index}: {port.device}, VID:PID={port.vid}:{port.pid}, Serial Number: {port.serial_number}")
        index += 1
    while True:
        try:
            selection = int(input("which device: "))
            if selection in range(index):
                break
            print("number out of range")
        except ValueError:
            print("invalid input")




