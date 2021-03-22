import unittest.mock
from unittest.mock import Mock
import interfaces as ifs
import pytest
from thermometer import Thermometer
from thermostat import Thermostat


@pytest.fixture
def thermostat_test_fixture(mocker):
    heater = Mock(spec=ifs.Heater)
    cooler = Mock(spec=ifs.Cooler)
    user_interface = Mock(spec=ifs.Ui)

    thermometer = mocker.patch('thermometer.Thermometer').return_value
    return heater, cooler, user_interface, thermometer


def test_should_turn_off_all_devices_when_desired_and_current_temperatures_are_equal(thermostat_test_fixture):
    # Given
    heater, cooler, user_interface, thermometer = thermostat_test_fixture
    user_interface.get_desired_temp.return_value = 20
    thermometer.get_current_temp.return_value = 20
    sut = Thermostat(user_interface=user_interface, cooler=cooler, heater=heater)

    # When
    sut.on_timer_tick()

    # Then
    heater.turn_off.assert_called_once()
    cooler.turn_off.assert_called_once()
    heater.turn_on.assert_not_called()
    cooler.turn_on.assert_not_called()


def test_should_turn_on_cooler_when_current_temp_is_much_much_higher(thermostat_test_fixture):
    # Given
    heater, cooler, user_interface, thermometer = thermostat_test_fixture
    user_interface.get_desired_temp.return_value = 20
    thermometer.get_current_temp.return_value = 51
    sut = Thermostat(user_interface=user_interface, cooler=cooler, heater=heater)

    # When
    sut.on_timer_tick()

    # Then
    heater.turn_off.assert_called_once()
    cooler.turn_on.assert_called_once()
    heater.turn_on.assert_not_called()
    cooler.turn_off.assert_not_called()


def test_should_turn_off_all_devices_when_desired_and_current_temperatures_are_equal_but_very_high(thermostat_test_fixture):
    # Given
    heater, cooler, user_interface, thermometer = thermostat_test_fixture
    user_interface.get_desired_temp.return_value = 51
    thermometer.get_current_temp.return_value = 51
    sut = Thermostat(user_interface=user_interface, cooler=cooler, heater=heater)

    # When
    sut.on_timer_tick()

    # Then
    heater.turn_off.assert_called_once()
    cooler.turn_off.assert_called_once()
    heater.turn_on.assert_not_called()
    cooler.turn_on.assert_not_called()


