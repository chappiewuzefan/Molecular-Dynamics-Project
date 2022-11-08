/*
Name: Zefan Wu
Student Number: u6474528
Course: COMP3320
Assignment Number: 1
Name of this file: md_lib_AoS.c
I declare that the material I am submitting in this file is entirely my own
work. I have not collaborated with anyone to produce it, nor have I copied it,
in part or in full, from work produced by someone else. I have not given access
to this material to any other student in this course.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Struct containting x, y, z components of a vector
typedef struct _vector {
  double x;
  double y;
  double z;
} Vector;

// Struct containting position, velocity and acceleration of an atom
// (old_acceleration is also provided in case you find it helpful)
typedef struct _atom {
  Vector position;
  Vector velocity;
  Vector acceleration;
  Vector old_acceleration;
} Atom;

// Struct containing list of atoms and it's length
typedef struct _atoms {
  Atom *atoms;
  int n;
} Atoms;

// 2 times loop unrolling
void update_position(double dt, Atoms *restrict atoms)
{
  for (int r1 = 0; r1 < atoms->n; r1+=2)
  {
    atoms->atoms[r1].position.x = atoms->atoms[r1].position.x + dt * (atoms->atoms[r1].velocity.x + dt * atoms->atoms[r1].acceleration.x / 2);
    atoms->atoms[r1].position.y = atoms->atoms[r1].position.y + dt * (atoms->atoms[r1].velocity.y + dt * atoms->atoms[r1].acceleration.y / 2);
    atoms->atoms[r1].position.z = atoms->atoms[r1].position.z + dt * (atoms->atoms[r1].velocity.z + dt * atoms->atoms[r1].acceleration.z / 2);

    atoms->atoms[r1+1].position.x = atoms->atoms[r1+1].position.x + dt * (atoms->atoms[r1+1].velocity.x + dt * atoms->atoms[r1+1].acceleration.x / 2);
    atoms->atoms[r1+1].position.y = atoms->atoms[r1+1].position.y + dt * (atoms->atoms[r1+1].velocity.y + dt * atoms->atoms[r1+1].acceleration.y / 2);
    atoms->atoms[r1+1].position.z = atoms->atoms[r1+1].position.z + dt * (atoms->atoms[r1+1].velocity.z + dt * atoms->atoms[r1+1].acceleration.z / 2);
  }
  return;
}

void calculate_pe(Atoms *restrict atoms, double *potential_energy)
{
  for (int r1 = 0; r1 < atoms->n - 1; r1++)
  {
    double r1_x = atoms->atoms[r1].position.x;
    double r1_y = atoms->atoms[r1].position.y;
    double r1_z = atoms->atoms[r1].position.z;
    for (int r2 = r1 + 1; r2 < atoms->n; r2++)
    {
      double r2_x = atoms->atoms[r2].position.x;
      double r2_y = atoms->atoms[r2].position.y;
      double r2_z = atoms->atoms[r2].position.z;
      double dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2));
      *potential_energy += 1 / pow(dis_r1r2, 12) - 2 / pow(dis_r1r2, 6);
    }
  }
  return;
}

// 2 times loop unrolling
void update_force(Atoms *restrict atoms)
{
  for (int r1 = 0; r1 < atoms->n; r1+=2)
  {
    double r1_x = atoms->atoms[r1].position.x;
    double r1_y = atoms->atoms[r1].position.y;
    double r1_z = atoms->atoms[r1].position.z;

    double r3_x = atoms->atoms[r1+1].position.x;
    double r3_y = atoms->atoms[r1+1].position.y;
    double r3_z = atoms->atoms[r1+1].position.z;
    // record old acceleration
    atoms->atoms[r1].old_acceleration.x = atoms->atoms[r1].acceleration.x;
    atoms->atoms[r1].old_acceleration.y = atoms->atoms[r1].acceleration.y;
    atoms->atoms[r1].old_acceleration.z = atoms->atoms[r1].acceleration.z;
    atoms->atoms[r1].acceleration.x = 0;
    atoms->atoms[r1].acceleration.y = 0;
    atoms->atoms[r1].acceleration.z = 0;

    atoms->atoms[r1+1].old_acceleration.x = atoms->atoms[r1+1].acceleration.x;
    atoms->atoms[r1+1].old_acceleration.y = atoms->atoms[r1+1].acceleration.y;
    atoms->atoms[r1+1].old_acceleration.z = atoms->atoms[r1+1].acceleration.z;
    atoms->atoms[r1+1].acceleration.x = 0;
    atoms->atoms[r1+1].acceleration.y = 0;
    atoms->atoms[r1+1].acceleration.z = 0;
    for (int r2 = 0; r2 < atoms->n; r2++)
    {
      if (r1 != r2 && r1 != r2+1 && r1+1 != r2+1)
      {
        double r2_x = atoms->atoms[r2].position.x;
        double r2_y = atoms->atoms[r2].position.y;
        double r2_z = atoms->atoms[r2].position.z;

        double r4_x = atoms->atoms[r2+1].position.x;
        double r4_y = atoms->atoms[r2+1].position.y;
        double r4_z = atoms->atoms[r2+1].position.z;
        // distance between r1 and r2
        double dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2));

        double dis_r3r4 = sqrt(pow(r3_x - r4_x, 2) + pow(r3_y - r4_y, 2) + pow(r3_z - r4_z, 2));
        // acceleration(t + dt) of r1 in x, y, z directions
        atoms->atoms[r1].acceleration.x += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_x - r2_x);
        atoms->atoms[r1].acceleration.y += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_y - r2_y);
        atoms->atoms[r1].acceleration.z += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_z - r2_z);

        atoms->atoms[r1+1].acceleration.x += (12 / pow(dis_r3r4, 14) - 12 / pow(dis_r3r4, 8)) * (r3_x - r4_x);
        atoms->atoms[r1+1].acceleration.y += (12 / pow(dis_r3r4, 14) - 12 / pow(dis_r3r4, 8)) * (r3_x - r4_x);
        atoms->atoms[r1+1].acceleration.z += (12 / pow(dis_r3r4, 14) - 12 / pow(dis_r3r4, 8)) * (r3_x - r4_x);
      }
    }
  }
  return;
}

// 2 times loop unrolling
void update_volocities(double dt, Atoms *restrict atoms, double *kinetic_energy)
{
  for (int r1 = 0; r1 < atoms->n; r1+=2)
  {
    // update r1 velocity for x, y, z directions
    atoms->atoms[r1].velocity.x = atoms->atoms[r1].velocity.x + dt * (atoms->atoms[r1].acceleration.x + atoms->atoms[r1].old_acceleration.x) / 2;
    atoms->atoms[r1].velocity.y = atoms->atoms[r1].velocity.y + dt * (atoms->atoms[r1].acceleration.y + atoms->atoms[r1].old_acceleration.y) / 2;
    atoms->atoms[r1].velocity.z = atoms->atoms[r1].velocity.z + dt * (atoms->atoms[r1].acceleration.z + atoms->atoms[r1].old_acceleration.z) / 2;

    atoms->atoms[r1+1].velocity.x = atoms->atoms[r1+1].velocity.x + dt * (atoms->atoms[r1+1].acceleration.x + atoms->atoms[r1+1].old_acceleration.x) / 2;
    atoms->atoms[r1+1].velocity.y = atoms->atoms[r1+1].velocity.y + dt * (atoms->atoms[r1+1].acceleration.y + atoms->atoms[r1+1].old_acceleration.y) / 2;
    atoms->atoms[r1+1].velocity.z = atoms->atoms[r1+1].velocity.z + dt * (atoms->atoms[r1+1].acceleration.z + atoms->atoms[r1+1].old_acceleration.z) / 2;
    // calculate the KE
    *kinetic_energy += (pow(atoms->atoms[r1].velocity.x, 2) + pow(atoms->atoms[r1].velocity.y, 2) + pow(atoms->atoms[r1].velocity.z, 2)) / 2;
    *kinetic_energy += (pow(atoms->atoms[r1+1].velocity.x, 2) + pow(atoms->atoms[r1+1].velocity.y, 2) + pow(atoms->atoms[r1+1].velocity.z, 2)) / 2;
  }
  return;
}

// Update all atoms by a single timestep of length dt and return the kinetic and
// potential energy of the system
void update_timestep(double dt, Atoms *restrict atoms, double *kinetic_energy,
                     double *potential_energy) {
  *potential_energy = 0;
  *kinetic_energy = 0;

  /*** YOUR CODE HERE FOR STEP 2 ***/
  // update position
  update_position(dt, atoms);

  // calculate new PE
  calculate_pe(atoms, potential_energy);

  // update new acceleration and record old acceleration
  update_force(atoms);

  // update velocities
  update_volocities(dt, atoms, kinetic_energy);

  //  printf("%-15e%-15e%-15e\n", atoms->atoms[0].acceleration.x, atoms->atoms[0].acceleration.y, atoms->atoms[0].acceleration.z);
  return;
}

// Put any one time initialization code here - this will be called once at the
// start of your program. You do not need to add code here if you have no
// initialization requirements.
void initial_config(double dt, Atoms *restrict atoms,
                    double *potential_energy) {

  *potential_energy = 0;

  /*** YOUR CODE HERE FOR STEP 2 ***/
  calculate_pe(atoms, potential_energy);
  // update new acceleration and record old acceleration
  update_force(atoms);
//   printf("atoms->atoms[0].acceleration.x : %-20e\n", atoms->atoms[0].acceleration.x);
//   printf("%-15e%-15e%-15e\n", atoms->atoms[0].acceleration.x, atoms->atoms[0].acceleration.y, atoms->atoms[0].acceleration.z);
  return;
}
