import pytest


@pytest.fixture
def a_foo():
    return "Foo"
                                                                                                                                                                                                                                                                               

def test_a_foo(a_foo):
    assert a_foo == "Foo"