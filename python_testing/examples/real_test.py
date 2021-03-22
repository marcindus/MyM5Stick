'''
from libraries.setups.enb import Commissioning, SwconfigFlags
import pytest


LOGS_DIR = '/some/logs/directory'


@pytest.fixture
def commissioning_lib(mocker):
    return mocker.patch('libraries.setups.enb.ta_commissioning').return_value


def test_commissioning_should_be_performed(commissioning_lib):
    sut = Commissioning(LOGS_DIR)
    sut.perform('commissioning/file/path')

    commissioning_lib.find_scfc_and_fix_and_send_full_commissioning.assert_called_once_with(
        output_dirpath=LOGS_DIR,
        remote_host=None,
        scfc_filename='commissioning/file/path',
        yaml_filename=None)
'''