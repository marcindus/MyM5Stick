import unittest
import ElectricalCircuit

class testOfElectricalCircuit(unittest.TestCase):

    def setUp(self):
        print ("\nStarting the test with the name '{}' ".format( self.id().split(".")[2] ) )
        self.circuit = ElectricalCircuit.ElectricalCircuit(R=2, U=12)
        self.circuit.turn_the_circuit_on()

    def test_if_2_ohm_is_set_correctly(self):
        assert self.circuit._R == 2.0

    def test_if_12V_is_set_correctly(self):
        assert self.circuit.measure_the_voltage() == 12.0

    def test_if_current_is_correct_for_12v_and_2_ohms(self):
        assert self.circuit.measure_the_current() == 6.0

    def test_if_voltage_is_changed_correctly(self):
        voltage_before_the_change = self.circuit._U
        self.circuit.set_the_voltage(self.circuit._U/2)
        assert self.circuit.measure_the_voltage() == voltage_before_the_change/2

    def test_if_resistance_is_changed_correctly(self):
        self.circuit.change_the_resistance(R=100)
        assert self.circuit._R == 100

    def test_if_voltage_change_does_not_change_the_resistance(self):
        circuit_resistance = self.circuit._R
        self.circuit.set_the_voltage(self.circuit._U*2)
        assert self.circuit._R == circuit_resistance

    def test_if_current_is_changed_correctly_after_the_voltage_change(self):
        current_before_the_change = self.circuit.measure_the_current()
        self.circuit.set_the_voltage(self.circuit._U/2)
        assert self.circuit.measure_the_current() == current_before_the_change/2

    def test_if_current_obeys_the_law_of_ohm(self):
        U = 20
        R = 100
        self.circuit.change_the_resistance(R)
        self.circuit.set_the_voltage(U)
        ohms_law_I = U / R
        assert self.circuit._I == ohms_law_I
