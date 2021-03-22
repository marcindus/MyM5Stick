import unittest
from unittest.mock import *

class Environment:
    def analyze(self, log):
        return True if log else False

class SomeVerySophisticatedTest(unittest.TestCase):

    def setUp(self):
        self.sut = Environment()
        self.logMock = MagicMock()

    def test_skeleton(self):
        self.assertTrue(self.sut.analyze(self.logMock))
        self.assertEqual(1, 1)

if __name__ == '__main__':
    unittest.main()
