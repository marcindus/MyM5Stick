import unittest
import unittest.mock

import pytest

import examples.foo_provider


# ===============   unittest
class BasicAssertions(unittest.TestCase):

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)


class Base(unittest.TestCase):
    def setUp(self):
        self.foo = "Foo"  # define a value in self
        self.bar = "Bar"

    def test_will_run_in_all_inheritors(self):
        self.assertEqual("Foo", self.foo)  # and use it in tests


class Inheritor(Base):  # fixtures can be composed
    def setUp(self):
        super(Inheritor, self).setUp()
        self.foo = "FooFoo"

    def test_foofoobar(self):
        self.assertEqual("FooFooBar", self.foo + self.bar)


# ================   PyTest ==========================
def test_foo():
    assert True == True


def test_not_foo():
    assert True == False


def test_rises():
    with pytest.raises(TypeError):
        raise TypeError("Argh!")


@pytest.fixture
def a_foo():
    return "Foo"


def test_a_foo(a_foo):
    assert a_foo == "Foo"


@pytest.mark.parametrize("a,b,c", [
    (1, 2, 3),
    (6, 18, 24),
    (-1, 12, 10)
])
def test_sum(a, b, c):
    assert (a + b) == c


# ============== Mock & Patch ===================
class Suite(unittest.TestCase):
    @unittest.mock.patch('examples.foo_provider.Provider')
    def test_da_bar_decorated(self, provider_mock):
        provider_mock.return_value.get_da_foo.return_value = "DaBar"
        provided = provider_mock()
        self.assertEqual("DaBar", provided.get_da_foo("Fu"))
        provider_mock.return_value.get_da_foo.assert_called_once_with("Fu")

    def test_da_bar_content_managed(self):
        with unittest.mock.patch('examples.foo_provider.Provider') as provider_mock:
            provider_mock.return_value.get_da_foo.return_value = "DaBar"
            provided = provider_mock()
            self.assertEqual("DaBar", provided.get_da_foo("Fu"))
            provider_mock.return_value.get_da_foo.assert_called_once_with("Fu")

    def test_da_bar_patcher_with_cleanup(self):
        # this would go to setUp
        patcher = unittest.mock.patch('examples.foo_provider.Provider')
        provider_mock = patcher.start()
        self.addCleanup(patcher.stop)  # guarantee that cleaned up after test

        provider_mock.return_value.get_da_foo.return_value = "DaBar"
        provided = provider_mock()
        self.assertEqual("DaBar", provided.get_da_foo("Fu"))
        provider_mock.return_value.get_da_foo.assert_called_once_with("Fu")


@pytest.fixture
def patched_get_da_foo(mocker):
    return mocker.patch('examples.foo_provider.Provider').return_value


def test_da_bar_fixtured(patched_get_da_foo):
    provider_mock = patched_get_da_foo()

    provider_mock.return_value.get_da_foo.return_value = "DaBar"
    provided = provider_mock()
    assert "DaBar" == provided.get_da_foo("Fu")
    provider_mock.return_value.get_da_foo.assert_called_once_with("Fu")


@unittest.mock.patch('examples.foo_provider.Provider')
def test_da_bar_decorated(provider_mock):
    provider_mock.return_value.get_da_foo.return_value = "DaBar"
    provided = examples.foo_provider.Provider()
    assert "DaBar" == provided.get_da_foo("Fu")
    provider_mock.return_value.get_da_foo.assert_called_once_with("Fu")
