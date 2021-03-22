import thermometer as ti

class Thermostat:
    def __init__(self, user_interface, heater, cooler):
        self.user_interface = user_interface
        self.heater = heater
        self.cooler = cooler
        self.thermometer = ti.Thermometer()

    def on_timer_tick(self):
        if self.thermometer.get_current_temp() != self.user_interface.get_desired_temp():
            self._handle_too_hot()
        else:
            self._handle_if_equal()

    def _handle_if_equal(self):
        self.heater.turn_off()
        self.cooler.turn_off()

    def _handle_too_hot(self):
        self.cooler.turn_on()
        self.heater.turn_off()
