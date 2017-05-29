#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "randomlib.c"
#include "randomlib.h"

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

// Compile program as "gcc Argon.c -lm"

double boltzmannDistribution(double T, double m){
  double v1, v2, s, std;

  double k = 1.3806488*pow(10,-23); 
 
  double eps = 119.8*k;

  double vscaling = sqrt(m/eps);

  do {
    v1 = RandomDouble(-0.999999999999, 0.999999999999);
    v2 = RandomDouble(-0.999999999999, 0.999999999999);
	  s = v1*v1 + v2*v2;
	} while (s >= 1.0 || s == 0.);

  double S = sqrt(-2*log(s)/s);
  std = sqrt(k*T/m);
  return std*S*v1*vscaling;
}

void createLattice(double b, double m, int nDimensions, int N, double u[nDimensions][N], 
    int Nx, int Ny, int Nz, double R[Nx][Ny][Nz][N], int nAtoms,
    double r[nAtoms][nDimensions], double v[nAtoms][nDimensions]){

  printf("Creating a box lattice of %d atoms.\n\n", nAtoms);

  double ivec[3] = {1,0,0}; // Creating unit vectors.
  double jvec[3] = {0,1,0};
  double kvec[3] = {0,0,1};

  int i, j, k, l, p;
  
  // Creating atom positions:
  for ( i = 0; i < nDimensions; i++ ){
    u[i][0] = 0;
    u[i][1] = 0.5*b*ivec[i] + 0.5*b*jvec[i];
    u[i][2] = 0.5*b*ivec[i] + 0.5*b*kvec[i];
    u[i][3] = 0.5*b*jvec[i] + 0.5*b*kvec[i];
  }
 

  // Creating lattice:
  
  double tmp[3];

  int seed1 = 1;
  int seed2 = 2;

  RandomInitialise(seed1, seed2);

  int n = 0;

  double Vcx = 0;
  double Vcy = 0;
  double Vcz = 0;

  double sigma = 3.405; // [Angstrom]
  
  for ( p = 0; p < 4; p++){
    for ( i = 0; i < Nx; i++ ){
      for ( j = 0; j < Ny; j++ ){
        for ( k = 0; k < Nz; k++ ){
          /* fprintf(f, "Ar "); */
          for ( l = 0; l < 3; l++){
            R[i][j][k][l] = 
              (i)*b*ivec[l] + (j)*b*jvec[l] + (k)*b*kvec[l];

            tmp[l] = boltzmannDistribution(100., m);

            r[n][l] = u[l][p] + R[i][j][k][l];
            v[n][l] = tmp[l];
            
          } 
          Vcx += tmp[0];
          Vcy += tmp[1];
          Vcz += tmp[2];
          n++;
        }
      }
    }
  }

  Vcx = Vcx/nAtoms;
  Vcy = Vcy/nAtoms;
  Vcz = Vcz/nAtoms;


  FILE *F = fopen("../../res/lattice.xyz", "w");
  char status[] = "Lattice works.";
  fprintf(F, "%d\n%s\n", nAtoms, status);
  
  for (n = 0; n < nAtoms; n++) {
    v[n][0] = v[n][0] - Vcx;
    v[n][1] = v[n][1] - Vcy;
    v[n][2] = v[n][2] - Vcz;
    fprintf(F, "Ar %f %f %f %f %f %f\n", 
        r[n][0], r[n][1], r[n][2], v[n][0], v[n][1], v[n][2]);
  }
  fclose(F);
}

void updateForce(int nAtoms, int nDimensions, double L, double r[nAtoms][nDimensions], double v[nAtoms][nDimensions], double f[nAtoms][nDimensions]){
  int i,j;
  double xi, yi, zi;
  double xj, yj, zj;
  double vxi, vyi, vzi;
  double dxij, dyij, dzij;
  double rij2, irij2, irij6, irij12;
  double dU;
  double L2, L2m;
  double E, Ep, Ek;
  E = 0;
  Ep = 0;
  Ek = 0;
  L2 = L/2.;
  L2m = -L/2.; 
  

  for (i = 0; i < nAtoms-1; i++) {
    xi = r[i][0];
    yi = r[i][1];
    zi = r[i][2];

    vxi = v[i][0];
    vyi = v[i][1];
    vzi = v[i][2];

    Ek += 0.5*(vxi*vxi + vyi*vyi + vzi*vzi);
    for (j = i+1; j < nAtoms; j++) {
      xj = r[j][0];
      yj = r[j][1];
      zj = r[j][2];
     
      dxij = -xj + xi;
      dyij = -yj + yi;
      dzij = -zj + zi;


      if (dxij>L2) dxij -= L;
      else if (dxij<L2m) dxij += L;
      if (dyij>L2) dyij -= L;
      else if (dyij<L2m) dyij += L;
      if (dxij>L2) dzij -= L;
      else if (dzij<L2m) dzij += L;

      // WITH LJ:
      rij2 = dxij*dxij + dyij*dyij + dzij*dzij;
      irij2 = 1./rij2;
      irij6 = irij2*irij2*irij2;
      irij12 = irij6*irij6;
       
      dU = 24*(2*irij12 - irij6)*irij2;
      
      Ep += 4*(irij12 - irij6);
      //Utilize symmetry: fij = fji:
      f[i][0] = f[i][0] + dU*dxij;
      f[i][1] = f[i][1] + dU*dyij;
      f[i][2] = f[i][2] + dU*dzij;

      f[j][0] = f[j][0] - dU*dxij;
      f[j][1] = f[j][1] - dU*dyij;
      f[j][2] = f[j][2] - dU*dzij;
    }
  }
  E = Ep + Ek;
  printf("Energies : E = %f, Ep = %f,Ek = %f\n", E, Ep, Ek);
}

void integrateVerlet(int nAtoms, int nDimensions, double L, double r[nAtoms][nDimensions], 
    double v[nAtoms][nDimensions], double f[nAtoms][nDimensions], double m, double dt){

  int i, j;

  // Verlet integration:
  
  for (i = 0; i < nAtoms; i++) {
    // First (half) step of Verlet integration:
    v[i][0] = v[i][0] + 0.5*f[i][0]*dt;
    v[i][1] = v[i][1] + 0.5*f[i][1]*dt;
    v[i][2] = v[i][2] + 0.5*f[i][2]*dt;

    // Position update of Verlet:
    r[i][0] = r[i][0] + v[i][0]*dt;
    r[i][1] = r[i][1] + v[i][1]*dt;
    r[i][2] = r[i][2] + v[i][2]*dt;

    // Periodic boundary conditions
    if ( r[i][0] > L ) r[i][0] -= L*(round(r[i][0]/L));
    else if ( r[i][0] < 0. ) r[i][0] -= L*(round(r[i][0]/L)-1);

    if ( r[i][1] > L ) r[i][1] -= L*(round(r[i][1]/L));
    else if ( r[i][1] < 0. ) r[i][1] -= L*(round(r[i][1]/L)-1);
    
    if ( r[i][2] > L ) r[i][2] -= L*(round(r[i][2]/L));
    else if ( r[i][2] < 0. ) r[i][2] -= L*(round(r[i][2]/L)-1);
  }

  updateForce(nAtoms, nDimensions, L, r, v, f);
  
  for ( i = 0; i < nAtoms; i++ ){      
    // Final step of Verlet:
    v[i][0] = v[i][0] + 0.5*f[i][0]*dt;
    v[i][1] = v[i][1] + 0.5*f[i][1]*dt;
    v[i][2] = v[i][2] + 0.5*f[i][2]*dt;
  } 

  FILE *H = fopen("../../res/lattice.xyz", "a");
  int n;
  char status[] = "Verlet works.";
  fprintf(H, "%d\n%s\n", nAtoms, status);
  for (n = 0; n < nAtoms; n++) {
    fprintf(H, "Ar %f %f %f %f %f %f\n",
        r[n][0], r[n][1], r[n][2], v[n][0], v[n][1], v[n][2]);
  }
  fclose(H);
}

int main(){

  double A = pow(10,-10);
  double sigma = 3.405;                // Length scaling [Angstrom].
  double b = 4;             // Lattice square length [Angstrom].
  int nDimensions = 3;                    // Number of spatial dimensions.
  int N = 4;                    // Number of atom position vectors per cell.
  int Nc = 8;                   // Number of cells in lattice.
  int nAtoms = N*Nc*Nc*Nc;
  double u[nDimensions][N];                           // Set of lattice vectors.
  double R[Nc][Nc][Nc][N];                  // Set of cube vectors.
  double v[nAtoms][nDimensions];                  
  double r[nAtoms][nDimensions];
  double f[nAtoms][nDimensions];

  double amu = 1.660538921*pow(10,-27); 
  double M = 39.948; // [AMU]
  double m = M*amu;
  double k = 1.3806488*pow(10,-23); 
  double eps = 119.8*k;
  double tau = sigma*sqrt(m/eps);

  double dt = tau;

  double L = Nc*b;

  // Create the lattice, and initiate particle positions and velocities.
  createLattice(b, m, nDimensions, N, u, Nc, Nc, Nc, R, nAtoms, r, v);

  //Initiate forces.
  int i,j;
  for (i = 0; i < nAtoms; i++) {
    for (j = 0; j < nDimensions; j++) {
        f[i][j] = 0;
    }
  }
 
  updateForce(nAtoms, nDimensions, L, r, v, f);

  int timeStep = 0;
  int maxTime = 100;

  while (timeStep < maxTime+1) {
    printf("Verlet Integration-step No. %d\n", timeStep);
    integrateVerlet(nAtoms, nDimensions, L, r, v, f, m, dt);
    timeStep++;
  }

  return 0;
}
