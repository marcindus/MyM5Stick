from abc import ABC, abstractmethod


class Ui(ABC):
    @abstractmethod
    def get_desired_temp(self):
        pass


class IThermometer(ABC):
    @abstractmethod
    def get_current_temp(self):
        pass


class Heater(ABC):
    def turn_on(self):
        pass

    def turn_off(self):
        pass


class Cooler(ABC):
    def turn_on(self):
        pass

    def turn_off(self):
        pass
