"""
Name: Ryan Stocks
Student Number: u6311272
Course: COMP3320
Assignment Number: 1
Name of this file: C_Structures.py
I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.
"""

import ctypes as C


class CVector(C.Structure):
    """Data structure using a struct of arrays format for holding an array of 3D vectors."""

    _fields_ = [("x", C.c_double), ("y", C.c_double), ("z", C.c_double)]

    def __init__(self, x: float, y: float, z: float):
        self.x = x
        self.y = y
        self.z = z


class CAtom(C.Structure):
    """Data structure for holding the position, velocity and acceleration of an atom"""

    _fields_ = [
        ("position", CVector),
        ("velocity", CVector),
        ("acceleration", CVector),
        ("old_acceleration", CVector),
    ]

    def __init__(self, x: float, y: float, z: float):
        self.position = CVector(x, y, z)
        self.velocity = CVector(0, 0, 0)
        self.acceleration = CVector(0, 0, 0)


class CAtoms_AoS(C.Structure):
    """Data structure for holding the position, velocity and acceleration of all atoms using AoS"""

    _fields_ = [
        ("atoms", C.POINTER(CAtom)),
        ("n", C.c_int),
    ]

    def __init__(self, n: int):
        self.atoms = aligned_array(n, CAtom)
        self.n = n


class CVectorArray(C.Structure):
    """Data structure using a SoA format for holding an array of 3D vectors"""

    _fields_ = [
        ("x", C.POINTER(C.c_double)),
        ("y", C.POINTER(C.c_double)),
        ("z", C.POINTER(C.c_double)),
    ]

    def __init__(self, n: int):
        self.x = aligned_array(n, C.c_double)
        self.y = aligned_array(n, C.c_double)
        self.z = aligned_array(n, C.c_double)


class CAtoms_SoA(C.Structure):
    """Data structure for holding the position, velocity and acceleration of all atoms using SoA"""

    _fields_ = [
        ("position", CVectorArray),
        ("velocity", CVectorArray),
        ("acceleration", CVectorArray),
        ("old_acceleration", CVectorArray),
        ("n", C.c_int),
    ]

    def __init__(self, n: int):
        self.position = CVectorArray(n)
        self.velocity = CVectorArray(n)
        self.acceleration = CVectorArray(n)
        self.old_acceleration = CVectorArray(n)
        self.n = n


def aligned_array(n: int, array_type):
    """Create an array aligned to 64 byte boundaries

    Args:
        n (int): n
        array_type: type of array to create (e.g. C.c_double)
    """
    size = n * C.sizeof(array_type) + 2 ** 6
    buffer = (C.c_char * size)()
    offset = 2 ** 6 - (C.addressof(buffer) % (2 ** 6))
    return (array_type * n).from_buffer(buffer, offset)
