*** Settings ***

Library     ElectricalCircuit.py     R=${R_default}    U=${U_default}

Suite Setup       Turn The Circuit On
Suite Teardown    Turn The Circuit Off

Test Setup      Turn The Circuit On
Test Teardown   Run Keywords
...                 Turn The Circuit Off
...             AND
...                  Set The Voltage      ${U_default}
...             AND
...                  Change The Resistance    ${R_default}

Test Template   The Cuttent In The Circuit With Given Resistance And Power Supply Complies With The Law Of Ohm

*** Variables ***
${U_default}=    ${1.0}
${R_default}=    ${1.0}

*** Test Cases ***
There is no current in the open circuit
    ...    resistance=${R_default}
    ...    voltage=${U_default}
    ...    expected_current=0
    [Setup]    Turn The Circuit Off

There is no current in the open circuit when the voltage changes
    ...    resistance=${R_default}
    ...    voltage=${100}
    ...    expected_current=0
    [Setup]    Turn The Circuit Off

There is no current in the open circuit when the resistance changes
    ...    resistance=${100}
    ...    voltage=${U_default}
    ...    expected_current=0
    [Setup]    Turn The Circuit Off

When all parameters have one unit the current value is one ampere - calibration
    ...    resistance=${1.0}
    ...    voltage=${1.0}
    ...    expected_current=${1.0}

Ohm's law - test for parameters' different than ones
    ...    resistance=${10}
    ...    voltage=${12}
    ...    expected_current=${1.2}

Circuit breaker works when there is no resistance in close circuit
    ...    resistance=${0}
    ...    voltage=${U_default}
    ...    expected_current=0

*** Keywords ***

The Cuttent In The Circuit With Given Resistance And Power Supply Complies With The Law Of Ohm
    [Arguments]    ${resistance}    ${voltage}    ${expected_current}
    
    Set The Voltage      ${voltage}
    Change The Resistance    ${resistance}
    ${i_measured}=   Measure The Current
    Should Be Equal     float(${i_measured})   float(${expected_current})
