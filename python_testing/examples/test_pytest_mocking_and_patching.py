import pytest
import unittest.mock
import examples.foo_provider


class User:
    def use_provided(self):
        prov = examples.foo_provider.Provider()
        return "who's {}?".format(prov.get_da_foo())


@pytest.fixture
def patched_get_da_foo(mocker):
    return mocker.patch('examples.foo_provider.Provider')


def test_da_bar_fixtured(patched_get_da_foo):
    provider_mock = patched_get_da_foo()
    provider_mock.get_da_foo.return_value = "DaBar"
    usr = User()
    assert "who's DaBar?" == usr.use_provided()
    provider_mock.get_da_foo.assert_called_once_with()


@unittest.mock.patch('examples.foo_provider.Provider')
def test_da_bar_decorated(provider_mock):
    provider_mock.return_value.get_da_foo.return_value = "DaBar"
    usr = User()
    assert "who's DaBar?" == usr.use_provided()
    provider_mock.return_value.get_da_foo.assert_called_once_with()