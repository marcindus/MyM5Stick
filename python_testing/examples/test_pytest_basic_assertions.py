import pytest


def test_foo():
    assert True == True


def test_not_foo():
    assert True == False


def test_rises():
    with pytest.raises(TypeError):
        raise TypeError("Argh!")
