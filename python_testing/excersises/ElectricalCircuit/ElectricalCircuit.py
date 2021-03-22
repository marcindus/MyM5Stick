class ElectricalCircuit:

#    ROBOT_LIBRARY_SCOPE = 'TEST SUITE'

    def __init__(self, R, U):
        self._U = float(U)
        self._R = float(R)
        self._is_activated = False
        #self._I = 0
        self._I = 1

    def set_the_voltage(self, U):
        self._U = float(U)
        self._calculate_the_current()

    def change_the_resistance(self, R):
        self._R = float(R)
        print ('*INFO* Resistance set to {}ohm'.format(self._R))
        self._calculate_the_current()

    def measure_the_current(self):
        print ('*INFO* Ammeter displays: {}A'.format(self._I))
        return self._I

    def measure_the_voltage(self):
        print ('*INFO* Voltmeter displays: {}V'.format(self._U))
        return self._U

    def turn_the_circuit_on(self):
        self._is_activated = True
        self._calculate_the_current()

    def turn_the_circuit_off(self):
        self._Is_activated = False
        self._I = 0
        #bug#1#self._I = 1

    def _calculate_the_current(self):
        #   put ':#' after the 'self._Is_activated' to generate errors
        if self._is_activated and self._is_the_circuit_breaker_ok():
            self._I = float(self._U/self._R)
            #bug#2#self._I = float(self._U/self._R*1.1)

    def _is_the_circuit_breaker_ok(self):
        if self._R == 0:
                self._is_activated = False
                self._I = 0
                ##bug#1#self._I = 1
                print ('*ERROR* Circuit breaker activated!!')
                return False
                #bug#3#return True
        return True
        #bug#4#return False
