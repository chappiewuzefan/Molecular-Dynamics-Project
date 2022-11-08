"""
Name: 
Student Number: 
Course: COMP3320
Assignment Number: 1
Name of this file: utils.py
I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.
"""

import math


class Vector:
    """A 3D vector class defining some convenient operation such as addition, scalar multiplication and dot product."""

    def __init__(self, x: float, y: float, z: float):
        self.x = x
        self.y = y
        self.z = z

    def __eq__(self, other):
        return abs(self - other) < 0.000001

    def __neg__(self):
        return Vector(-self.x, -self.y, -self.z)

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)

    # Multiplication by a scalar
    def __mul__(self, scalar: float):
        return Vector(self.x * scalar, self.y * scalar, self.z * scalar)

    def __rmul__(self, scalar: float):
        return self.__mul__(scalar)

    def dot(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z

    # Magnitude of vector
    def __abs__(self):
        return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)

    def magnitude(self):
        return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)


class Atom:
    """A datastructure holding the information (position, velocity, acceleration) about a single atom"""

    def __init__(self, x_pos, y_pos, z_pos):
        self.position = Vector(x_pos, y_pos, z_pos)
        self.velocity = Vector(0, 0, 0)
        self.acceleration = Vector(0, 0, 0)
        self.old_acceleration = Vector(0, 0, 0)
