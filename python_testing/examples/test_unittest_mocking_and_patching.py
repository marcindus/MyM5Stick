import unittest
import unittest.mock as mock

import examples.foo_provider


class User:
    def use_provided(self):
        prov = examples.foo_provider.Provider()
        return "who's {}?".format(prov.get_da_foo())


class DecoratedUserTests(unittest.TestCase):

    @mock.patch('examples.foo_provider.Provider')
    def test_da_bar(self, provider_mock):
        provider_mock.return_value.get_da_foo.return_value = "DaBar"
        usr = User()
        result = usr.use_provided()
        self.assertEqual("who's DaBar?", result)
        provider_mock.return_value.get_da_foo.assert_called_with()


class ContentManagedUserTests(unittest.TestCase):
    def test_da_bar(self):
        with mock.patch('examples.foo_provider.Provider') \
                as provider_mock:
            provider_mock.return_value.get_da_foo.return_value \
                = "DaBar"
            usr = User()
            result = usr.use_provided()
            self.assertEqual("who's DaBar?", result)
            provider_mock.return_value.get_da_foo.assert_called_with()


class UserTests(unittest.TestCase):
    def setUp(self):
        patcher = mock.patch('examples.foo_provider.Provider')
        self.provider_mock = patcher.start()
        self.addCleanup(
            patcher.stop)  # Called always after tests. No parentheses!

    def test_da_bar(self):
        self.provider_mock.return_value.get_da_foo.return_value \
            = "DaBar"

        self.assertEqual("who's DaBar?", User().use_provided())
        self.provider_mock.return_value.get_da_foo.assert_called_with()
