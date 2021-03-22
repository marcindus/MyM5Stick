import pytest
import unittest.mock
import examples.foo_provider


class User:
    def use_provided(self):
        prov = examples.foo_provider.Provider()
        return "who's {}?".format(prov.get_da_foo())

@unittest.mock.patch('examples.foo_provider.Provider')
def test_pytest_patching(provider_mock):
    provider_mock.return_value.get_da_foo.return_value = "DaBar"
    usr = User()
    result = usr.use_provided()
    assert "who's DaBar?" == result
    provider_mock.return_value.get_da_foo.assert_called_with()

@pytest.fixture
def foo_provided(mocker):
    return mocker.patch('examples.foo_provider.Provider').return_value


def test_pytest_mocker(foo_provided):
    foo_provided.get_da_foo.return_value = "DaBar"

    usr = User()
    result = usr.use_provided()
    assert "who's DaBar?" == result
    foo_provided.get_da_foo.assert_called_once_with()
