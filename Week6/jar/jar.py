class Jar:
    def __init__(self, capacity=12):
        if capacity > 0:
            self._capacity = capacity
            self._size = 0
        else:
            raise ValueError("Capacity must be a non-negative integer.")

    def __str__(self):
        return self._size * "\U0001F36A"

    def deposit(self, n):
        if self._size + n <= self._capacity:
            self._size += n
        else:
            raise ValueError(f"Cannot add {n} cookies.")

    def withdraw(self, n):
        if self._size >= n:
            self._size -= n
        else:
            raise ValueError(f"Cannot remove {n} cookies.")

    @property
    def capacity(self):
        return self._capacity

    @property
    def size(self):
        return self._size
