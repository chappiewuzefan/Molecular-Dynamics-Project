"""
Name: 
Student Number: 
Course: COMP3320
Assignment Number: 1
Name of this file: argument_parser.py
I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.
"""

import argparse


def parse_arguments():
    """Parse input arguments to md.py (N R dt iter [-v] [-c {AoS, SoA, Opt}])"""
    parser = argparse.ArgumentParser(
        description="COMP3320 Assignment 1 - Molecular dynamics simulation"
    )
    parser.add_argument(
        "N", type=int, help="Integer number of atoms per side of initial cube"
    )
    parser.add_argument("R", type=float, help="Side length of initial cube")
    parser.add_argument("dt", type=float, help="Integration time step")
    parser.add_argument("iter", type=int, help="Number of time steps")
    parser.add_argument(
        "-v",
        "--visual",
        default=False,
        action="store_true",
        help="Display vpython visualization",
    )
    parser.add_argument(
        "-c",
        "--c_version",
        choices=["AoS", "SoA", "Opt"],
        help="Specify C implementation",
    )
    return parser.parse_args()
