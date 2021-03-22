*** Settings ***

Library     ElectricalCircuit.py     R=${R_default}    U=${U_default}

Suite Setup       Turn The Circuit On
Suite Teardown    Turn The Circuit Off

Test Setup      Turn The Circuit On
Test Teardown   Turn The Circuit Off 

*** Variables ***
${U_default}=    1.0
${R_default}=    1.0

*** Test Cases ***
There is no current in the open circuit
    [Setup]    Turn The Circuit Off

    Ammeter Should Measure '0'A In The Electrical Circuit

    [Teardown]    None

There is no current in the open circuit when the voltage changes
    [Setup]    Turn The Circuit Off

    Set The Voltage '20'V In The Electrical Circuit
    Ammeter Should Measure '0'A In The Electrical Circuit

    [Teardown]  Set The Voltage '${U_default}'V In The Electrical Circuit

There is no current in the open circuit when the resistance changes
    [Setup]    Turn The Circuit Off

    Set The Resistance To '1.0' Ohm
    Ammeter Should Measure '0'A In The Electrical Circuit

    [Teardown]     Set The Resistance To '${R_default}' Ohm

When all parameters have one unit the current value is one ampere - calibration
    Ammeter Should Measure '1.0'A In The Electrical Circuit

Ohm's law - test for parameters' different than ones
    Set The Voltage '12.0'V In The Electrical Circuit
    Set The Resistance To '10.0' Ohm
    Ammeter Should Measure '1.2'A In The Electrical Circuit

Circuit breaker works when there is no resistance in close circuit
    Set The Voltage '1.0'V In The Electrical Circuit
    Set The Resistance To '0.0' Ohm
    Circuit Breaker Was Activated And The Circuit Is Now Open

*** Keywords ***
Ammeter Should Measure '${i}'A In The Electrical Circuit
    ${i_measured}=   Measure The Current
    Should Be Equal     float(${i_measured})   float(${i})

Set The Voltage '${u}'V In The Electrical Circuit
    Set The Voltage      ${u}
    Voltometer Should Measure '${u}'V In The Electrical Circuit

Voltometer Should Measure '${Vu}'V In The Electrical Circuit
    ${u_measured}=   Measure The Voltage
    Should Be True      ${u_measured} == float(${Vu})

Set The Resistance To '${r}' Ohm
    Change The Resistance    R=${r}

Circuit Breaker Was Activated And The Circuit Is Now Open
    Ammeter Should Measure '0'A In The Electrical Circuit   
