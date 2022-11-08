"""
Name: 
Student Number: 
Course: COMP3320
Assignment Number: 1
Name of this file: visual_atom.py
I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.
"""

from .utils import Atom
from vpython import sphere, vector


class VisualAtom(Atom):
    """Atom with additional vpython behaviour for a visual display."""

    def __init__(self, x_pos, y_pos, z_pos):
        super().__init__(x_pos, y_pos, z_pos)

        self.visual_atom = sphere(radius=0.3)
        self.visual_atom.pos = vector(x_pos, y_pos, z_pos)

    def update_visual(self):
        self.visual_atom.pos.x = self.position.x
        self.visual_atom.pos.y = self.position.y
        self.visual_atom.pos.z = self.position.z
