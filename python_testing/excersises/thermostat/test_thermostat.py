import pytest

from unittest.mock import Mock
import unittest.mock
import thermostat
import interfaces 


@pytest.fixture
def thermostat_fixture(mocker):

    heater = Mock(spec=interfaces.Heater)
    cooler = Mock(spec=interfaces.Cooler)
    ui = Mock(spec=interfaces.Ui)
    thermometer_ = mocker.patch('thermostat.Thermometer').return_value
    thermostat_ = thermostat.Thermostat(heater=heater, cooler=cooler, thermometer=thermometer_, ui=ui)

    return heater, cooler, ui, thermometer_, thermostat_


def test_when_desired_temp_equal_to_current_temp_cooler_and_heater_are_off(thermostat_fixture):
    #Given
    heater, cooler, ui, thermometer, thermostat = thermostat_fixture

    desired_temp = 19
    current_temp = 19

    ui.get_desired_temp.return_value = desired_temp
    thermometer.get_current_temp.return_value = current_temp

    #When
    thermostat.on_timer_tick()

    #Then
    thermometer.get_current_temp.assert_called_once()
    heater.turn_off.assert_called_once()
    cooler.turn_off.assert_called_once()

    heater.turn_on.assert_not_called()
    cooler.turn_on.assert_not_called()



def test_when_desired_temp_higher_than_current_temp_heater_is_on_cooler_is_off(thermostat_fixture):
    #Given
    heater, cooler, ui, thermometer, thermostat = thermostat_fixture

    desired_temp = 20
    current_temp = 19

    ui.get_desired_temp.return_value = desired_temp
    thermometer.get_current_temp.return_value = current_temp

    #When
    thermostat.on_timer_tick()

    #Then
    thermometer.get_current_temp.assert_called_once()
    heater.turn_on.assert_called_once()
    cooler.turn_off.assert_called_once()
    heater.turn_off.assert_not_called()
    cooler.turn_on.assert_not_called()


def test_when_desired_temp_lower_than_current_temp_heater_is_off_cooler_is_on(thermostat_fixture):
    #Given
    heater, cooler, ui, thermometer, thermostat = thermostat_fixture

    desired_temp = 19
    current_temp = 20

    ui.get_desired_temp.return_value = desired_temp
    thermometer.get_current_temp.return_value = current_temp

    #When
    thermostat.on_timer_tick()

    #Then
    thermometer.get_current_temp.assert_called_once()
    heater.turn_on.assert_called_once()
    cooler.turn_off.assert_called_once()
    heater.turn_off.assert_not_called()
    cooler.turn_on.assert_not_called()







