

class Thermometer:
    def get_current_temp():
        pass


class Thermostat:
    def __init__(self, thermometer, cooler, heater, ui):
        self.thermometer = thermometer
        self.ui = ui
        self.cooler = cooler
        self.heater = heater

    def on_timer_tick(self):

        current_temp = self.thermometer.get_current_temp()
        desired_temp = self.ui.get_desired_temp()

        if current_temp == desired_temp:
            self.heater.turn_off()
            self.cooler.turn_off()
        elif current_temp < desired_temp:
            self.heater.turn_on()
            self.cooler.turn_off()






