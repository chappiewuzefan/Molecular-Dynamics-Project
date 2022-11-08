"""
Name: Zefan Wu
Student Number: u6474528
Course: COMP3320
Assignment Number: 1
Name of this file: md1.py
I declare that the material I am submitting in this file is entirely my own work. I have not collaborated with anyone to produce it, nor have I copied it, in part or in full, from work produced by someone else. I have not given access to this material to any other student in this course.
"""
from math import sqrt
from typing import List
from lib.argument_parser import parse_arguments
import sys
import itertools
from lib.c_structures import *
from lib.utils import Vector, Atom
import ctypes as C


def initial_config(atoms: List[Atom], dt: float):
    """Perform any initial configuration if required and calculate the initial
    potential energy of the grid.

    This function is called once at the start of the program before performing
    any timestep updates.

    Args:
        atoms (List[Atom]): Array of atoms in AoS format
        dt (float): Length of one timestep

    Returns:
        potential_energy: The potential energy of the initial grid
    """
    potential_energy = 0

    """ YOUR CODE HERE FOR STEP 1 """
    for r1 in range(0, len(atoms) - 1):
        r1_x = atoms[r1].position.x
        r1_y = atoms[r1].position.y
        r1_z = atoms[r1].position.z
        for r2 in range(r1 + 1, len(atoms)):
            r2_x = atoms[r2].position.x
            r2_y = atoms[r2].position.y
            r2_z = atoms[r2].position.z
            dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2))
            potential_energy += 1 / pow(dis_r1r2, 12) - 2 / pow(dis_r1r2, 6)

    for r1 in range(0, len(atoms)):
        r1_x = atoms[r1].position.x
        r1_y = atoms[r1].position.y
        r1_z = atoms[r1].position.z
        # record old acceleration
        atoms[r1].old_acceleration.x = atoms[r1].acceleration.x
        atoms[r1].old_acceleration.y = atoms[r1].acceleration.y
        atoms[r1].old_acceleration.z = atoms[r1].acceleration.z
        atoms[r1].acceleration *= 0
        for r2 in range(0, len(atoms)):
            if r1 != r2:
                r2_x = atoms[r2].position.x
                r2_y = atoms[r2].position.y
                r2_z = atoms[r2].position.z
                # distance between r1 and r2
                dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2))
                # acceleration(t + dt) of r1 in x, y, z directions
                atoms[r1].acceleration.x += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_x - r2_x)
                atoms[r1].acceleration.y += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_y - r2_y)
                atoms[r1].acceleration.z += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_z - r2_z)
    # print(atoms[0].acceleration.x,atoms[0].acceleration.y,atoms[0].acceleration.z)
    return potential_energy


def update_timestep(atoms: List[Atom], dt: float):
    """Update the state of the atoms for one timestep of length dt.

    This is called `iter-1` times over the duration of the program.

    Args:
        atoms (List[Atom]): Array of atoms in AoS format
        dt (float): Length of one timestep

    Returns:
        potential_energy (float): The potential energy of the atoms after the timestep
        kinetic_energy (float): The kinetic energy of the atoms after the timestep.
    """
    potential_energy = 0
    kinetic_energy = 0

    """ YOUR CODE HERE FOR STEP 1 """

    for r1 in range(0, len(atoms)):
        atoms[r1].position.x = atoms[r1].position.x + dt * (atoms[r1].velocity.x + dt * atoms[r1].acceleration.x / 2)
        atoms[r1].position.y = atoms[r1].position.y + dt * (atoms[r1].velocity.y + dt * atoms[r1].acceleration.y / 2)
        atoms[r1].position.z = atoms[r1].position.z + dt * (atoms[r1].velocity.z + dt * atoms[r1].acceleration.z / 2)

    for r1 in range(0, len(atoms) - 1):
        r1_x = atoms[r1].position.x
        r1_y = atoms[r1].position.y
        r1_z = atoms[r1].position.z
        for r2 in range(r1 + 1, len(atoms)):
            r2_x = atoms[r2].position.x
            r2_y = atoms[r2].position.y
            r2_z = atoms[r2].position.z
            dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2))
            potential_energy += 1 / pow(dis_r1r2, 12) - 2 / pow(dis_r1r2, 6)

    for r1 in range(0, len(atoms)):
        r1_x = atoms[r1].position.x
        r1_y = atoms[r1].position.y
        r1_z = atoms[r1].position.z
        # record old acceleration
        atoms[r1].old_acceleration.x = atoms[r1].acceleration.x
        atoms[r1].old_acceleration.y = atoms[r1].acceleration.y
        atoms[r1].old_acceleration.z = atoms[r1].acceleration.z
        atoms[r1].acceleration *= 0
        for r2 in range(0, len(atoms)):
            if r1 != r2:
                r2_x = atoms[r2].position.x
                r2_y = atoms[r2].position.y
                r2_z = atoms[r2].position.z
                # distance between r1 and r2
                dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2))
                # acceleration(t + dt) of r1 in x, y, z directions
                atoms[r1].acceleration.x += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_x - r2_x)
                atoms[r1].acceleration.y += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_y - r2_y)
                atoms[r1].acceleration.z += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_z - r2_z)

    for r1 in range(0, len(atoms)):
        # update r1 velocity for x, y, z directions
        atoms[r1].velocity.x = atoms[r1].velocity.x + dt * (atoms[r1].acceleration.x + atoms[r1].old_acceleration.x) / 2
        atoms[r1].velocity.y = atoms[r1].velocity.y + dt * (atoms[r1].acceleration.y + atoms[r1].old_acceleration.y) / 2
        atoms[r1].velocity.z = atoms[r1].velocity.z + dt * (atoms[r1].acceleration.z + atoms[r1].old_acceleration.z) / 2
        # calculate the KE
        kinetic_energy += (pow(atoms[r1].velocity.x, 2) + pow(atoms[r1].velocity.y, 2) + pow(atoms[r1].velocity.z,
                                                                                             2)) / 2

    # print(atoms[0].acceleration.x, atoms[0].acceleration.y, atoms[0].acceleration.z)
    return potential_energy, kinetic_energy


def main():
    # Read in command arguments
    args = parse_arguments()
    N, R, dt, iter, show_visualization, c_version = (
        args.N,
        args.R,
        args.dt,
        args.iter,
        args.visual,
        args.c_version,
    )

    if show_visualization:
        from lib.visual_atom import VisualAtom
        from vpython import scene

    if c_version is not None:
        # Import md C Library
        if sys.platform in ["linux", "darwin"]:
            md_lib = C.CDLL(f"./md_lib_{c_version}.so")
        elif sys.platform in ["win32", "cygwin", "msys"]:
            md_lib = C.CDLL(f"./md_lib_{c_version}.dll")
        else:
            raise Exception("Error: Unknown system platform")
        if c_version == "AoS":
            c_atoms = CAtoms_AoS(N ** 3)
            md_lib.initial_config.argtypes = [C.c_double, C.POINTER(CAtoms_AoS), C.POINTER(C.c_double)]
            md_lib.update_timestep.argtypes = [C.c_double, C.POINTER(CAtoms_AoS), C.POINTER(C.c_double),
                                               C.POINTER(C.c_double)]
        elif c_version == "SoA":
            c_atoms = CAtoms_SoA(N ** 3)
            md_lib.initial_config.argtypes = [C.c_double, C.POINTER(CAtoms_SoA), C.POINTER(C.c_double)]
            md_lib.update_timestep.argtypes = [C.c_double, C.POINTER(CAtoms_SoA), C.POINTER(C.c_double),
                                               C.POINTER(C.c_double)]
        elif c_version == "Opt": # currently using AoS structure
            c_atoms = CAtoms_AoS(N ** 3)
            md_lib.initial_config.argtypes = [C.c_double, C.POINTER(CAtoms_AoS), C.POINTER(C.c_double)]
            md_lib.update_timestep.argtypes = [C.c_double, C.POINTER(CAtoms_AoS), C.POINTER(C.c_double),
                                               C.POINTER(C.c_double)]
        else:
            raise Exception(f"C version {c_version} has not yet been implemented")

    # Display command arguments
    print("Initial grid spacing  : {:f}".format(R))
    print("Total number of atoms : " + str(N ** 3))
    print("Integration time step : {:f}".format(dt))
    print("Number of timesteps   : " + str(iter))

    # Initialize all atoms in a grid
    atoms = []
    for i, (x, y, z) in enumerate(itertools.product(range(N), range(N), range(N))):
        x_pos = (x - 0.5 * (N - 1)) * R
        y_pos = (y - 0.5 * (N - 1)) * R
        z_pos = (z - 0.5 * (N - 1)) * R
        if show_visualization:
            atom = VisualAtom(x_pos, y_pos, z_pos)
        else:
            atom = Atom(x_pos, y_pos, z_pos)
        atoms.append(atom)
        if c_version is not None:
            if c_version == "AoS":
                c_atoms.atoms[i] = CAtom(x_pos, y_pos, z_pos)
            elif c_version == "SoA":
                c_atoms.position.x[i] = x_pos
                c_atoms.position.y[i] = y_pos
                c_atoms.position.z[i] = z_pos
            elif c_version == "Opt":
                c_atoms.atoms[i] = CAtom(x_pos, y_pos, z_pos) 
            else:
                raise Exception(f"C version {c_version} has not yet been implemented")

    # Prepare table heading for displaying the energy at each timestep
    print(
        "{0:<10}{1:<15}{2:<15}{3:<15}\n".format("TIME", "Total", "Kinetic", "Potential")
        + "-" * 53
    )

    # Perform initial configuration and calculate initial potential energy
    kinetic_energy, potential_energy = 0, 0
    if c_version is None:
        potential_energy = initial_config(atoms, dt)
    else:
        c_potential_energy = (C.c_double)()
        md_lib.initial_config(dt, c_atoms, c_potential_energy)
        potential_energy = c_potential_energy.value

    # Loop through all timesteps, update simulation and output energy
    for step in range(iter):
        if step != 0:
            if c_version is None:
                potential_energy, kinetic_energy = update_timestep(atoms, dt)
            else:
                c_potential_energy, c_kinetic_energy = (C.c_double)(), (C.c_double)()
                md_lib.update_timestep(dt, c_atoms, c_kinetic_energy, c_potential_energy)
                potential_energy, kinetic_energy = (c_potential_energy.value, c_kinetic_energy.value)

        # Output energies to visualization and the terminal
        print(
            "{0:<10f}{1:<15e}{2:<15e}{3:<15e}".format(
                *[
                    step * dt,
                    potential_energy + kinetic_energy,
                    kinetic_energy,
                    potential_energy,
                ]
            )
        )

        if show_visualization:
            for i, atom in enumerate(atoms):
                if c_version is not None:
                    if c_version == "SoA":
                        atom.position.x = c_atoms.position.x[i]
                        atom.position.y = c_atoms.position.y[i]
                        atom.position.z = c_atoms.position.z[i]
                    elif c_version == "AoS":
                        atom.position.x = c_atoms.atoms[i].position.x
                        atom.position.y = c_atoms.atoms[i].position.y
                        atom.position.z = c_atoms.atoms[i].position.z
                    elif c_version == "Opt":
                        atom.position.x = c_atoms.atoms[i].position.x
                        atom.position.y = c_atoms.atoms[i].position.y
                        atom.position.z = c_atoms.atoms[i].position.z
                    else:
                        raise Exception(f"C version {c_version} has not yet been implemented")
                atom.update_visual()
            scene.caption = (
                    f"Potential Energy: {potential_energy}\n"
                    + f"Kinetic Energy: {kinetic_energy}\n"
                    + f"Total Energy: {kinetic_energy + potential_energy}"
            )


if __name__ == "__main__":
    main()
