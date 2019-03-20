import serial
import datetime


class Communicator:
    def __init__(self, length):
        self.ser = serial.Serial('/dev/ttyACM0', 9600)
        self.length = length
        self.lights = [(0, 0, 0) for i in range(self.length)]

    def turn_on(self, start, length, value):
        for light in range(start, start+length):
            self.lights[light] = value
        write_string = b'0x01'

        self.ser.write(write_string)

    def turn_off(self, start, length, value=(0, 0, 0)):
        self.turn_on(start, length, value)

    def update(self):
        self.ser.write(b'0xFF')

    def check_for_time(self):
        lights_on = False
        morning = 7
        night = 22

        if morning <= datetime.datetime.now().hour < night:
            if not lights_on:
                self.turnon(0, self.length, (255, 255, 255))
                lights_on = True
        elif night <= datetime.datetime.now().hour < 24 or 0 <= datetime.datetime.now().hour < morning:
            if lights_on:
                self.turn_off(0, self.length)
                lights_on = False
        return lights_on

    def run(self):
        while True:
            self.update()  # Change this to call a different function to change behavior, add behavior


tank_controller = Communicator(72)
Communicator.run()











