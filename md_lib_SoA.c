/*
Name: Zefan Wu
Student Number: u6474528
Course: COMP3320
Assignment Number: 1
Name of this file: md_lib_SoA.c
I declare that the material I am submitting in this file is entirely my own
work. I have not collaborated with anyone to produce it, nor have I copied it,
in part or in full, from work produced by someone else. I have not given access
to this material to any other student in this course.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#ifdef __SSE2__
#include <immintrin.h>
#endif

// Struct containting x, y, z components of a vector using SoA format
typedef struct _vector_array {
  double *restrict x;
  double *restrict y;
  double *restrict z;
} VectorArray;

// Struct containting the position, velocity, acceleration along with the number
// of atoms n for a set of atoms
typedef struct _atoms {
  VectorArray position;
  VectorArray velocity;
  VectorArray acceleration;
  VectorArray old_acceleration;
  int n;
} Atoms;

void update_position(double dt, Atoms *restrict atoms) {
  for (int r1 = 0; r1 < atoms->n; r1++)
  {
    atoms->position.x[r1] = atoms->position.x[r1] + dt * (atoms->velocity.x[r1] + dt * atoms->acceleration.x[r1] / 2);
    atoms->position.y[r1] = atoms->position.y[r1] + dt * (atoms->velocity.y[r1] + dt * atoms->acceleration.y[r1] / 2);
    atoms->position.z[r1] = atoms->position.z[r1] + dt * (atoms->velocity.z[r1] + dt * atoms->acceleration.z[r1] / 2);
  }
  return;
}

void calculate_pe(Atoms *restrict atoms, double *potential_energy) {
  // calculate new PE
  for (int r1 = 0; r1 < atoms->n - 1; r1++)
  {
    double r1_x = atoms->position.x[r1];
    double r1_y = atoms->position.y[r1];
    double r1_z = atoms->position.z[r1];
    for (int r2 = r1 + 1; r2 < atoms->n; r2++)
    {
      double r2_x = atoms->position.x[r2];
      double r2_y = atoms->position.y[r2];
      double r2_z = atoms->position.z[r2];
      double dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2));
      *potential_energy += 1 / pow(dis_r1r2, 12) - 2 / pow(dis_r1r2, 6);
    }
  }
  return;
}

void update_force(Atoms *restrict atoms) {
  for (int r1 = 0; r1 < atoms->n; r1++)
  {
    double r1_x = atoms->position.x[r1];
    double r1_y = atoms->position.y[r1];
    double r1_z = atoms->position.z[r1];
    // record old acceleration
    atoms->old_acceleration.x[r1] = atoms->acceleration.x[r1];
    atoms->old_acceleration.y[r1] = atoms->acceleration.y[r1];
    atoms->old_acceleration.z[r1] = atoms->acceleration.z[r1];
    atoms->acceleration.x[r1] = 0;
    atoms->acceleration.y[r1] = 0;
    atoms->acceleration.z[r1] = 0;
    for (int r2 = 0; r2 < atoms->n; r2++)
    {
      if(r1 != r2)
      {
        double r2_x = atoms->position.x[r2];
        double r2_y = atoms->position.y[r2];
        double r2_z = atoms->position.z[r2];
        // distance between r1 and r2
        double dis_r1r2 = sqrt(pow(r1_x - r2_x, 2) + pow(r1_y - r2_y, 2) + pow(r1_z - r2_z, 2));
        // acceleration(t + dt) of r1 in x, y, z directions
        atoms->acceleration.x[r1] += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_x - r2_x);
        atoms->acceleration.y[r1] += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_y - r2_y);
        atoms->acceleration.z[r1] += (12 / pow(dis_r1r2, 14) - 12 / pow(dis_r1r2, 8)) * (r1_z - r2_z);
      }
    }
  }
  return;
}

void update_volocities(double dt, Atoms *restrict atoms, double *kinetic_energy) {
  for (int r1 = 0; r1 < atoms->n; r1++){
    // update r1 velocity for x, y, z directions
    atoms->velocity.x[r1] = atoms->velocity.x[r1] + dt * (atoms->acceleration.x[r1] + atoms->old_acceleration.x[r1]) / 2;
    atoms->velocity.y[r1] = atoms->velocity.y[r1] + dt * (atoms->acceleration.y[r1] + atoms->old_acceleration.y[r1]) / 2;
    atoms->velocity.z[r1] = atoms->velocity.z[r1] + dt * (atoms->acceleration.z[r1] + atoms->old_acceleration.z[r1]) / 2;
    // calculate the KE
    *kinetic_energy += (pow(atoms->velocity.x[r1], 2) + pow(atoms->velocity.y[r1], 2) + pow(atoms->velocity.z[r1], 2)) / 2;
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
  update_position(dt, atoms);

  // calculate new PE
  calculate_pe(atoms, potential_energy);

  // update new acceleration and record old acceleration
  update_force(atoms);

  // update velocities
  update_volocities(dt, atoms, kinetic_energy);
  return;
}

// Put any one time initialization code here - this will be called once at the
// start of your program. You do not need to add code here if you have no
// initialization requirements.
void initial_config(double dt, Atoms *restrict atoms,
                    double *potential_energy) {

  *potential_energy = 0;

  /*** YOUR CODE HERE FOR STEP 2 ***/
  // calculate intial PE
  calculate_pe(atoms, potential_energy);
  // update new acceleration and record old acceleration
  update_force(atoms);
  return;
}