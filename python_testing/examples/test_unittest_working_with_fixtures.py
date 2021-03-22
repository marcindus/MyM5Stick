import unittest


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
