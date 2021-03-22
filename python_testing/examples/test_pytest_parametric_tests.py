import pytest


@pytest.mark.parametrize("a,b,c", [
    (1, 2, 3),
    (6, 18, 24),
    (-1, 12, 10)
])
def test_sum(a, b, c):
    assert (a + b) == c
