import pytest
from .fizzbuzz import fizzbuzz

@pytest.mark.parametrize("expected,input_val", [
    ("fizz", 3),
    (1, 1),
    (2, 2),
    ("fizz", 6),
    ("buzz", 5),
    ("buzz", 10),
    ("fizzbuzz", 15)
])
def test_fizzbuzz(expected, input_val):
    assert expected == fizzbuzz(input_val)
