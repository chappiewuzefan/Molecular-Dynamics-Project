/*
Name: Zefan Wu
Student Number: u6474528
Course: COMP3320
Assignment Number: 1
Name of this file: md_AoS.c
I declare that the material I am submitting in this file is entirely my own
work. I have not collaborated with anyone to produce it, nor have I copied it,
in part or in full, from work produced by someone else. I have not given access
to this material to any other student in this course.
*/

#include "md_lib_AoS.c"
// Any code contained in a `#ifdef PAPI` section will only be compiled if papi
// is available on the current machine
#ifdef PAPI
#include <papi.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
  if (argc < 5) {
    printf("Error: Insufficient number of arguments. Usage: ./md_AoS N R dt "
           "iter\n");
    return -1;
  }

  // Read in and display command arguments
  int N, n, iter;
  double R, dt;
  N = atoi(argv[1]);     // Integer number of atoms per side of initial cube 3
  n = N * N * N;         // Total number of atoms 27
  R = atof(argv[2]);     // Initial grid spacing 1
  dt = atof(argv[3]);    // Integration time step 0.001
  iter = atoi(argv[4]);  // Number of timesteps 10
  printf("Initial grid spacing  : %f\n", R);
  printf("Total number of atoms : %d\n", n);
  printf("Integration time step : %f\n", dt);
  printf("Number of timesteps   : %d\n", iter);

  // Initialize all atoms in a grid
  Atoms atoms;
  atoms.n = n;
  atoms.atoms = calloc(n, sizeof(Atom));
  double origin_displacement = 0.5 * (N - R);        // 0.5 * (3 - 1) = 1
  for (int x = 0; x < N; x++) {
    double x_pos = x * R - origin_displacement;      // x * 1 - 1 : -1/0/1
    for (int y = 0; y < N; y++) {
      double y_pos = y * R - origin_displacement;    // y * 1 - 1 : -1/0/1
      for (int z = 0; z < N; z++) {
        double z_pos = z * R - origin_displacement;  // z * 1 - 1 : -1/0/1
        int i = x * N * N + y * N + z;               // i : 0/1/2; 3/4/5; 6/7/8 ... 24/25/26
        atoms.atoms[i].position.x = x_pos;
        atoms.atoms[i].position.y = y_pos;
        atoms.atoms[i].position.z = z_pos;
      }
    }
  }

  // Prepare table for displaying the energy at each timestep
  printf("TIME      Total          Kinetic        Potential\n");
  printf("-----------------------------------------------------\n");

  // Perform initial configuration
  double potential_energy;
  double kinetic_energy = 0;
  initial_config(dt, &atoms, &potential_energy);

  // Iterate through timesteps updating the system state and outputting the
  // energy
  struct timeval tp1, tp2;
  gettimeofday(&tp1, NULL);

  //-------------------------------------------------------------------//
  //---------------------- PAPI Init  ---------------------------------//
  //-------------------------------------------------------------------//
  const int NUM_EVENTS = 7;
  int Events[NUM_EVENTS]; // PAPI_L1_STM, PAPI_L1_TCM, PAPI_L2_TCA, PAPI_L2_TCM, PAPI_L3_TCA, PAPI_L3_TCM
  Events[0] = PAPI_L1_LDM;
  Events[1] = PAPI_L1_STM;
  Events[2] = PAPI_L1_TCM;
  Events[3] = PAPI_L2_TCA;
  Events[4] = PAPI_L2_TCM;
  Events[5] = PAPI_L3_TCA;
  Events[6] = PAPI_L3_TCM;
  long long values[NUM_EVENTS];
  int retval = 0;
  /* Start Init library */
  if(PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT )
  {
    fprintf(stderr,"PAPI Library initialization error! %d\n",  __LINE__);
    exit(1);
  }    
  /* Start counting events */
  if ((retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
  {
    fprintf(stderr,"PAPI Start counter error! %d, %d\n",  retval, __LINE__);
    exit(1);
  }    
  long long StartTime = PAPI_get_virt_usec(); 

  for (int i = 0; i < iter; i++) {
    if (i != 0)
      update_timestep(dt, &atoms, &kinetic_energy, &potential_energy);
    printf("%-10f%-15e%-15e%-15e\n", i * dt, potential_energy + kinetic_energy,
           kinetic_energy, potential_energy);
  }

  printf("-----------------------------------------------------\n");
  //-------------------------------------------------------------------//
  //---------------------- PAPI stop counters----------------------------//
  //-------------------------------------------------------------------//
  long long StopTime = PAPI_get_virt_usec();  
  /* Stop counting events */
  if ((retval = PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK){    
    fprintf(stderr,"PAPI stop counters error! %d, %d\n", retval, __LINE__);
    exit(1);
  }
  //-------------------------------------------------------------------//
  //---------------------- print values    ----------------------------//
  //-------------------------------------------------------------------//
  printf("Exec. time (us)  %20lld\n", (StopTime - StartTime)); 
  printf("PAPI_L1_LDM      %20lld\n", values[0]);               // Level 1 load misses
  printf("PAPI_L1_STM     %20lld\n", values[1]);                // Level 1 store misses
  printf("PAPI_L1_TCM      %20lld\n", values[2]);               // Level 1 cache misses
  printf("PAPI_L2_TCA     %20lld\n", values[3]);                // Level 2 total cache accesses
  printf("PAPI_L2_TCM      %20lld\n", values[4]);               // Level 2 cache misses
  printf("PAPI_L3_TCA     %20lld\n", values[5]);                // Level 3 total cache accesses
  printf("PAPI_L3_TCM      %20lld\n", values[6]);               // Level 3 cache misses
  printf("-----------------------------------------------------\n");

  gettimeofday(&tp2, NULL);
  int ti = (tp2.tv_sec - tp1.tv_sec) * 1000000 + (tp2.tv_usec - tp1.tv_usec);
  printf("This is the gettimeofday result(microseconds): %4d \n", ti);
  return 0;
}
