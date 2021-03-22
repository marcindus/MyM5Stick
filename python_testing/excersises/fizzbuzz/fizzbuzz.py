def is_dividable_by_3(x):
    return x % 3 == 0


def is_dividable_by_5(x):
    return x % 5 == 0


def fizzbuzz(x):
    if is_dividable_by_5(x) and is_dividable_by_3(x):
        return "fizzbuzz"
    if is_dividable_by_3(x):
        return "fizz"
    if is_dividable_by_5(x):
        return "buzz"
    return x
