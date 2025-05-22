import serial.tools.list_ports
from typing import List

class Serializer:
    def __init__(self, baudrate: int, ) -> None:
        self.ser = serial.Serial()
        self.ser.baudrate = baudrate
        self.ports = None
        return

    def get_ports(self) -> List[serial.tools.list_ports_common.ListPortInfo]:
        ports = serial.tools.list_ports.comports()
        self.ports = ports
        return ports 
    
    def connect_port(self, port: str) -> bool:
        self.ser.port = port
        self.ser.open()
        return self.ser.is_open

    def send(self, msg: str) -> bool:

        pass

    def receive(self) -> str:
        pass
