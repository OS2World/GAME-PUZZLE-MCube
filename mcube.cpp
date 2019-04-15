/*
 * mcube.cpp v 20051216 - Fri 2005 Dec 16
 * 4x4x4 Rubik's Cube Solver class (c) 2005 by Eric Dietz
 * notes: readme.txt  email: root@wrongway.org
 */

// includes
#include <stdlib.h>
#include <stdio.h>
#include <string>
// namespace
using namespace std;

// local includes
#include "mcube.h"

// defines

// mcube class definition

// constructor
mcube::mcube()
{
  // values for shorten:
  // SHORTEN_NONE, SHORTEN_STRIP_SOME, SHORTEN_STRIP_ALL,
  // SHORTEN_STRIP_ROTATE_SOME, SHORTEN_STRIP_ROTATE_ALL
  // cube rotations means trying to solve the cube with every possible different
  // color on top and on front.  This will greatly affect the speed of solvecube().
  shorten = SHORTEN_STRIP_ROTATE_ALL;
  erval = 0;
  warnval = 0;
  solution = "";
  resetcube();
  numcubes++;
}
// destructor
mcube::~mcube()
{
  numcubes--;
}
// version string
const char* mcube::version = "20051216";
// number of instantiated cubes
int mcube::numcubes = 0;
// overloaded == comparison
const bool mcube::operator==(const mcube &q)
{
  for (int i = 1; i <= N; ++i)
    for (int j = 1; j <= N; ++j)
      if
      (q.cube[i][N+1][j] != cube[i][N+1][j] ||
       q.cube[0][i][j]   != cube[0][i][j]   ||
       q.cube[i][j][0]   != cube[i][j][0]   ||
       q.cube[N+1][i][j] != cube[N+1][i][j] ||
       q.cube[i][j][N+1] != cube[i][j][N+1] ||
       q.cube[i][0][j]   != cube[i][0][j]  )
        return false;
  return true;
}
// overloaded != comparison
const bool mcube::operator!=(const mcube &q)
{
  return !operator==(q);
}
// pointer to cube array
int *mcube::face(int x, int y, int z)
{
  if (x < 0 || x > N+1 || y < 0 || y > N+1 || z < 0 || z > N+1)
    return NULL;
  return &cube[x][y][z];
}
// display cube diagram
const void mcube::renderscreen()
{
  /*
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[j][N+1][N+1-i]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[0][N+1-i][N+1-j]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[j][N+1-i][0]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[N+1][N+1-i][j]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[N+1-j][N+1-i][N+1]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %i", cube[j][0][i]);
    printf("\n");
  }
  */
  printf(
"\
          %i %i %i %i\n\
          %i %i %i %i\n\
          %i %i %i %i\n\
          %i %i %i %i\n\
 %i %i %i %i  %i %i %i %i  %i %i %i %i  %i %i %i %i\n\
 %i %i %i %i  %i %i %i %i  %i %i %i %i  %i %i %i %i\n\
 %i %i %i %i  %i %i %i %i  %i %i %i %i  %i %i %i %i\n\
 %i %i %i %i  %i %i %i %i  %i %i %i %i  %i %i %i %i\n\
          %i %i %i %i\n\
          %i %i %i %i\n\
          %i %i %i %i\n\
          %i %i %i %i\n\
",
cube[1][5][4], cube[2][5][4], cube[3][5][4], cube[4][5][4],
cube[1][5][3], cube[2][5][3], cube[3][5][3], cube[4][5][3],
cube[1][5][2], cube[2][5][2], cube[3][5][2], cube[4][5][2],
cube[1][5][1], cube[2][5][1], cube[3][5][1], cube[4][5][1],
cube[0][4][4], cube[0][4][3], cube[0][4][2], cube[0][4][1],
cube[1][4][0], cube[2][4][0], cube[3][4][0], cube[4][4][0],
cube[5][4][1], cube[5][4][2], cube[5][4][3], cube[5][4][4],
cube[4][4][5], cube[3][4][5], cube[2][4][5], cube[1][4][5],
cube[0][3][4], cube[0][3][3], cube[0][3][2], cube[0][3][1],
cube[1][3][0], cube[2][3][0], cube[3][3][0], cube[4][3][0],
cube[5][3][1], cube[5][3][2], cube[5][3][3], cube[5][3][4],
cube[4][3][5], cube[3][3][5], cube[2][3][5], cube[1][3][5],
cube[0][2][4], cube[0][2][3], cube[0][2][2], cube[0][2][1],
cube[1][2][0], cube[2][2][0], cube[3][2][0], cube[4][2][0],
cube[5][2][1], cube[5][2][2], cube[5][2][3], cube[5][2][4],
cube[4][2][5], cube[3][2][5], cube[2][2][5], cube[1][2][5],
cube[0][1][4], cube[0][1][3], cube[0][1][2], cube[0][1][1],
cube[1][1][0], cube[2][1][0], cube[3][1][0], cube[4][1][0],
cube[5][1][1], cube[5][1][2], cube[5][1][3], cube[5][1][4],
cube[4][1][5], cube[3][1][5], cube[2][1][5], cube[1][1][5],
cube[1][0][1], cube[2][0][1], cube[3][0][1], cube[4][0][1],
cube[1][0][2], cube[2][0][2], cube[3][0][2], cube[4][0][2],
cube[1][0][3], cube[2][0][3], cube[3][0][3], cube[4][0][3],
cube[1][0][4], cube[2][0][4], cube[3][0][4], cube[4][0][4]
  );
}
// return solvedness of cube
const bool mcube::issolved()
{
  int c[7], d;
  c[1] = cube[1][N+1][1]; c[2] = cube[0][1][1];
  c[3] = cube[1][1][0];   c[4] = cube[N+1][1][1];
  c[5] = cube[1][1][N+1]; c[6] = cube[1][0][1];
  for (int i = 1; i <= 6; i++) {
    d = 0;
    for (int j = 1; j <= 6; j++)
      if (c[i] == j)
        d++;
    if (d != 1)
      return false;
  }
  for (int i = 1; i <= N; i++)
    for (int j = 1; j <= N; j++)
      if
      (cube[i][N+1][j] != c[1] ||
       cube[0][i][j]   != c[2] ||
       cube[i][j][0]   != c[3] ||
       cube[N+1][i][j] != c[4] ||
       cube[i][j][N+1] != c[5] ||
       cube[i][0][j]   != c[6])
        return false;
  return true;
}
// reset the cube
const void mcube::resetcube()
{
  solution = "";
  for (int i = 0; i <= MOV; i++)
    mov[i] = 0;
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        cube[i][j][k] = 0;
  for (int i = 1; i <= N; i++)
    for (int j = 1; j <= N; j++)
    {
      cube[i][N+1][j] = 1;
      cube[0][i][j]   = 2;
      cube[i][j][0]   = 3;
      cube[N+1][i][j] = 4;
      cube[i][j][N+1] = 5;
      cube[i][0][j]   = 6;
    }
  erval = 0;
  warnval = 0;
  fx = 0; fy = 0; fz = 0;
  inited = true;
}
// temporary copier function
void mcube::copytemp(int c[N+2][N+2][N+2], int t[N+2][N+2][N+2])
{
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        t[i][j][k] = c[i][j][k];
}
// rotate given slice left
const void mcube::slice_l(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[i][0][j] = temp[N+1-j][0][i];
    else if (s == N)
      for (int j = 1; j <= N; j++)
        cube[i][N+1][j] = temp[N+1-j][N+1][i];
    cube[i][s][0]   = temp[N+1][s][i];
    cube[i][s][N+1] = temp[0][s][i];
    cube[0][s][i]   = temp[N+1-i][s][0];
    cube[N+1][s][i] = temp[N+1-i][s][N+1];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][1][j] = temp[N+1-j][1][i];
        cube[i][1][j]--;
        if (cube[i][1][j] < 0) cube[i][1][j] += 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][N][j] = temp[N+1-j][N][i];
        cube[i][N][j]++;
        if (cube[i][N][j] > 3) cube[i][N][j] -= 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[i][s][1] = temp[N][s][i];
      cube[i][s][N] = temp[1][s][i];
      cube[1][s][i] = temp[N+1-i][s][1];
      cube[N][s][i] = temp[N+1-i][s][N];
    }
  }
  */
}
// rotate given slice right
const void mcube::slice_r(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[i][0][j] = temp[j][0][N+1-i];
    else if (s == N)
      for (int j = 1; j <= N; j++)
        cube[i][N+1][j] = temp[j][N+1][N+1-i];
    cube[i][s][0]   = temp[0][s][N+1-i];
    cube[i][s][N+1] = temp[N+1][s][N+1-i];
    cube[0][s][i]   = temp[i][s][N+1];
    cube[N+1][s][i] = temp[i][s][0];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][1][j] = temp[j][1][N+1-i];
        cube[i][1][j]++;
        if (cube[i][1][j] > 3) cube[i][1][j] -= 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][N][j] = temp[j][N][N+1-i];
        cube[i][N][j]--;
        if (cube[i][N][j] < 0) cube[i][N][j] += 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[i][s][1] = temp[1][s][N+1-i];
      cube[i][s][N] = temp[N][s][N+1-i];
      cube[1][s][i] = temp[i][s][N];
      cube[N][s][i] = temp[i][s][1];
    }
  }
  */
}
// rotate given slice up
const void mcube::slice_u(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[0][i][j] = temp[0][j][N+1-i];
    if (s == N)
      for (int j = 1; j <= N; j++)
        cube[N+1][i][j] = temp[N+1][j][N+1-i];
    cube[s][i][0]   = temp[s][0][N+1-i];
    cube[s][i][N+1] = temp[s][N+1][N+1-i];
    cube[s][0][i]   = temp[s][i][N+1];
    cube[s][N+1][i] = temp[s][i][0];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[1][i][j] = temp[1][j][N+1-i];
        cube[1][i][j]--;
        if (cube[1][i][j] < 0) cube[1][i][j] += 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[N][i][j] = temp[N][j][N+1-i];
        cube[N][i][j]++;
        if (cube[N][i][j] > 3) cube[N][i][j] -= 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[s][i][1] = temp[s][1][N+1-i];
      cube[s][i][N] = temp[s][N][N+1-i];
      cube[s][1][i] = temp[s][i][N];
      cube[s][N][i] = temp[s][i][1];
    }
  }
  */
}
// rotate given slice down
const void mcube::slice_d(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[0][i][j] = temp[0][N+1-j][i];
    if (s == N)
      for (int j = 1; j <= N; j++)
        cube[N+1][i][j] = temp[N+1][N+1-j][i];
    cube[s][i][0]   = temp[s][N+1][i];
    cube[s][i][N+1] = temp[s][0][i];
    cube[s][0][i]   = temp[s][N+1-i][0];
    cube[s][N+1][i] = temp[s][N+1-i][N+1];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[1][i][j] = temp[1][N+1-j][i];
        cube[1][i][j]++;
        if (cube[1][i][j] > 3) cube[1][i][j] -= 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[N][i][j] = temp[N][N+1-j][i];
        cube[N][i][j]--;
        if (cube[N][i][j] < 0) cube[N][i][j] += 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[s][i][1] = temp[s][N][i];
      cube[s][i][N] = temp[s][1][i];
      cube[s][1][i] = temp[s][N+1-i][1];
      cube[s][N][i] = temp[s][N+1-i][N];
    }
  }
  */
}
// rotate given slice clockwise
const void mcube::slice_c(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[i][j][0] = temp[N+1-j][i][0];
    if (s == N)
      for (int j = 1; j <= N; j++)
        cube[i][j][N+1] = temp[N+1-j][i][N+1];
    cube[i][0][s]   = temp[N+1][i][s];
    cube[i][N+1][s] = temp[0][i][s];
    cube[0][i][s]   = temp[N+1-i][0][s];
    cube[N+1][i][s] = temp[N+1-i][N+1][s];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][j][1] = temp[N+1-j][i][1];
        cube[i][j][1]++;
        if (cube[i][j][1] > 3) cube[i][j][1] -= 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][j][N] = temp[N+1-j][i][N];
        cube[i][j][N]--;
        if (cube[i][j][N] < 0) cube[i][j][N] += 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[i][1][s] = temp[N][i][s];
      cube[i][N][s] = temp[1][i][s];
      cube[1][i][s] = temp[N+1-i][1][s];
      cube[N][i][s] = temp[N+1-i][N][s];
    }
  }
  */
}
// rotate given slice counterclockwise
const void mcube::slice_a(int s)
{
  if (s < 1 || s > N) return;
  int temp[N+2][N+2][N+2];
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        temp[i][j][k] = cube[i][j][k];
  //copytemp(cube, temp);
  for (int i = 1; i <= N; i++)
  {
    if (s == 1)
      for (int j = 1; j <= N; j++)
        cube[i][j][0] = temp[j][N+1-i][0];
    if (s == N)
      for (int j = 1; j <= N; j++)
        cube[i][j][N+1] = temp[j][N+1-i][N+1];
    cube[i][0][s]   = temp[0][N+1-i][s];
    cube[i][N+1][s] = temp[N+1][N+1-i][s];
    cube[0][i][s]   = temp[i][N+1][s];
    cube[N+1][i][s] = temp[i][0][s];
  }
  /*
  if (s == 1) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][j][1] = temp[j][N+1-i][1];
        cube[i][j][1]--;
        if (cube[i][j][1] < 0) cube[i][j][1] += 4;
      }
    }
  }
  else if (s == N) {
    for (int i = 2; i <= N-1; i++) {
      for (int j = 2; j <= N-1; j++) {
        cube[i][j][N] = temp[j][N+1-i][N];
        cube[i][j][N]++;
        if (cube[i][j][N] > 3) cube[i][j][N] -= 4;
      }
    }
  }
  else {
    for (int i = 2; i <= N-1; i++) {
      cube[i][1][s] = temp[1][N+1-i][s];
      cube[i][N][s] = temp[N][N+1-i][s];
      cube[1][i][s] = temp[i][N][s];
      cube[N][i][s] = temp[i][1][s];
    }
  }
  */
}
// cube rotator aliases
const void mcube::UL() { slice_l(N  ); }// rotate top slice left
const void mcube::UR() { slice_r(N  ); }// rotate top slice right
const void mcube::DL() { slice_l(1  ); }// rotate bottom slice left
const void mcube::DR() { slice_r(1  ); }// rotate bottom slice right
const void mcube::LU() { slice_u(1  ); }// rotate left slice up
const void mcube::LD() { slice_d(1  ); }// rotate left slice down
const void mcube::RU() { slice_u(N  ); }// rotate right slice up
const void mcube::RD() { slice_d(N  ); }// rotate right slice down
const void mcube::FC() { slice_c(1  ); }// rotate front slice clockwise
const void mcube::FA() { slice_a(1  ); }// rotate front slice counterclockwise
const void mcube::BC() { slice_c(N  ); }// rotate rear slice clockwise (from front view)
const void mcube::BA() { slice_a(N  ); }// rotate rear slice counterclockwise (from front view)
const void mcube::uL() { slice_l(N-1); }// rotate inner top slice left
const void mcube::uR() { slice_r(N-1); }// rotate inner top slice right
const void mcube::dL() { slice_l(2  ); }// rotate inner bottom slice left
const void mcube::dR() { slice_r(2  ); }// rotate inner bottom slice right
const void mcube::lU() { slice_u(2  ); }// rotate inner left slice up
const void mcube::lD() { slice_d(2  ); }// rotate inner left slice down
const void mcube::rU() { slice_u(N-1); }// rotate inner right slice up
const void mcube::rD() { slice_d(N-1); }// rotate inner right slice down
const void mcube::fC() { slice_c(2  ); }// rotate inner front slice clockwise
const void mcube::fA() { slice_a(2  ); }// rotate inner front slice counterclockwise
const void mcube::bC() { slice_c(N-1); }// rotate inner rear slice clockwise (from front view)
const void mcube::bA() { slice_a(N-1); }// rotate inner rear slice counterclockwise (from front view)
const void mcube::CL() { for (int i = 1; i <= N; i++) slice_l(i); } // rotate whole cube left
const void mcube::CR() { for (int i = 1; i <= N; i++) slice_r(i); } // rotate whole cube right
const void mcube::CU() { for (int i = 1; i <= N; i++) slice_u(i); } // rotate whole cube up
const void mcube::CD() { for (int i = 1; i <= N; i++) slice_d(i); } // rotate whole cube down
const void mcube::CC() { for (int i = 1; i <= N; i++) slice_c(i); } // rotate whole cube clockwise
const void mcube::CA() { for (int i = 1; i <= N; i++) slice_a(i); } // rotate whole cube counterclockwise
const void mcube::U2() { UL(); UL(); }  // rotate top slice twice
const void mcube::D2() { DR(); DR(); }  // rotate bottom slice twice
const void mcube::L2() { LD(); LD(); }  // rotate left slice twice
const void mcube::R2() { RU(); RU(); }  // rotate right slice twice
const void mcube::F2() { FC(); FC(); }  // rotate front slice twice
const void mcube::B2() { BA(); BA(); }  // rotate rear slice twice
const void mcube::u2() { uL(); uL(); }  // rotate inner top slice twice
const void mcube::d2() { dR(); dR(); }  // rotate inner bottom slice twice
const void mcube::l2() { lD(); lD(); }  // rotate inner left slice twice
const void mcube::r2() { rU(); rU(); }  // rotate inner right slice twice
const void mcube::f2() { fC(); fC(); }  // rotate inner front slice twice
const void mcube::b2() { bA(); bA(); }  // rotate inner rear slice twice
// end of cube rotator aliases
// scramble the cube
const void mcube::scramblecube()
{
  int axis_direction, slice;
  // come up with a better calculation for a good number of random moves based on cube size later
  int nummoves = (N-2)*25+10;
  nummoves += rand() % nummoves;
  resetcube();
  for (int i = 1; i <= nummoves; i++)
  {
    axis_direction = rand() % 6 + 1;  // which axis and direction
    slice = rand() % N + 1;           // which slice
    switch (axis_direction)
    {
      case 1: slice_l(slice); break;
      case 2: slice_r(slice); break;
      case 3: slice_u(slice); break;
      case 4: slice_d(slice); break;
      case 5: slice_c(slice); break;
      case 6: slice_a(slice); break;
    }
  }
}
// do a series of moves
const void mcube::domoves(string s)
{
  string a = "";
  for (int i = 1; i <= (int)(s.length() / 3); i++) {
    a = s.substr(i * 3 - 3, 2);
    if      (a == "UL") UL();
    else if (a == "UR") UR();
    else if (a == "DL") DL();
    else if (a == "DR") DR();
    else if (a == "LU") LU();
    else if (a == "LD") LD();
    else if (a == "RU") RU();
    else if (a == "RD") RD();
    else if (a == "FC") FC();
    else if (a == "FA") FA();
    else if (a == "BC") BC();
    else if (a == "BA") BA();
    else if (a == "uL") uL();
    else if (a == "uR") uR();
    else if (a == "dL") dL();
    else if (a == "dR") dR();
    else if (a == "lU") lU();
    else if (a == "lD") lD();
    else if (a == "rU") rU();
    else if (a == "rD") rD();
    else if (a == "fC") fC();
    else if (a == "fA") fA();
    else if (a == "bC") bC();
    else if (a == "bA") bA();
    else if (a == "CL") CL();
    else if (a == "CR") CR();
    else if (a == "CU") CU();
    else if (a == "CD") CD();
    else if (a == "CC") CC();
    else if (a == "CA") CA();
    else if (a == "U2") U2();
    else if (a == "D2") D2();
    else if (a == "L2") L2();
    else if (a == "R2") R2();
    else if (a == "F2") F2();
    else if (a == "B2") B2();
    else if (a == "u2") u2();
    else if (a == "d2") d2();
    else if (a == "l2") l2();
    else if (a == "r2") r2();
    else if (a == "f2") f2();
    else if (a == "b2") b2();
  }
}
// execute solution
const void mcube::dosolution()
{
  domoves(solution);
}
// find given center on top
const int mcube::findcenter_u(int a)
{
  if      (cube[2][5][2] == a)
  { fx = 2; fy = 5; fz = 2; return POSY; }
  else if (cube[2][5][3] == a)
  { fx = 2; fy = 5; fz = 3; return POSY; }
  else if (cube[3][5][2] == a)
  { fx = 3; fy = 5; fz = 2; return POSY; }
  else if (cube[3][5][3] == a)
  { fx = 3; fy = 5; fz = 3; return POSY; }
  return 0;
}
// find given center on bottom
const int mcube::findcenter_d(int a)
{
  if      (cube[2][0][2] == a)
  { fx = 2; fy = 0; fz = 2; return NEGY; }
  else if (cube[2][0][3] == a)
  { fx = 2; fy = 0; fz = 3; return NEGY; }
  else if (cube[3][0][2] == a)
  { fx = 3; fy = 0; fz = 2; return NEGY; }
  else if (cube[3][0][3] == a)
  { fx = 3; fy = 0; fz = 3; return NEGY; }
  return 0;
}
// find given center on left
const int mcube::findcenter_l(int a)
{
  if      (cube[0][2][2] == a)
  { fx = 0; fy = 2; fz = 2; return NEGX; }
  else if (cube[0][2][3] == a)
  { fx = 0; fy = 2; fz = 3; return NEGX; }
  else if (cube[0][3][2] == a)
  { fx = 0; fy = 3; fz = 2; return NEGX; }
  else if (cube[0][3][3] == a)
  { fx = 0; fy = 3; fz = 3; return NEGX; }
  return 0;
}
// find given center on right
const int mcube::findcenter_r(int a)
{
  if      (cube[5][2][2] == a)
  { fx = 5; fy = 2; fz = 2; return POSX; }
  else if (cube[5][2][3] == a)
  { fx = 5; fy = 2; fz = 3; return POSX; }
  else if (cube[5][3][2] == a)
  { fx = 5; fy = 3; fz = 2; return POSX; }
  else if (cube[5][3][3] == a)
  { fx = 5; fy = 3; fz = 3; return POSX; }
  return 0;
}
// find given center on front
const int mcube::findcenter_f(int a)
{
  if      (cube[2][2][0] == a)
  { fx = 2; fy = 2; fz = 0; return NEGZ; }
  else if (cube[2][3][0] == a)
  { fx = 2; fy = 3; fz = 0; return NEGZ; }
  else if (cube[3][2][0] == a)
  { fx = 3; fy = 2; fz = 0; return NEGZ; }
  else if (cube[3][3][0] == a)
  { fx = 3; fy = 3; fz = 0; return NEGZ; }
  return 0;
}
// find given center on back
const int mcube::findcenter_b(int a)
{
  if      (cube[2][2][5] == a)
  { fx = 2; fy = 2; fz = 5; return POSZ; }
  else if (cube[2][3][5] == a)
  { fx = 2; fy = 3; fz = 5; return POSZ; }
  else if (cube[3][2][5] == a)
  { fx = 3; fy = 2; fz = 5; return POSZ; }
  else if (cube[3][3][5] == a)
  { fx = 3; fy = 3; fz = 5; return POSZ; }
  return 0;
}
// find given center not on top
const int mcube::findcenter_not_u(int a)
{
  int x;
  if ((x = findcenter_l(a))) return x;
  if ((x = findcenter_f(a))) return x;
  if ((x = findcenter_r(a))) return x;
  if ((x = findcenter_b(a))) return x;
  if ((x = findcenter_d(a))) return x;
  return 0;
}
// find given center not on bottom
const int mcube::findcenter_not_d(int a)
{
  int x;
  if ((x = findcenter_b(a))) return x;
  if ((x = findcenter_r(a))) return x;
  if ((x = findcenter_f(a))) return x;
  if ((x = findcenter_l(a))) return x;
  if ((x = findcenter_u(a))) return x;
  return 0;
}
// find given center not on left
const int mcube::findcenter_not_l(int a)
{
  int x;
  if ((x = findcenter_u(a))) return x;
  if ((x = findcenter_f(a))) return x;
  if ((x = findcenter_d(a))) return x;
  if ((x = findcenter_b(a))) return x;
  if ((x = findcenter_r(a))) return x;
  return 0;
}
// find given center not on right
const int mcube::findcenter_not_r(int a)
{
  int x;
  if ((x = findcenter_b(a))) return x;
  if ((x = findcenter_d(a))) return x;
  if ((x = findcenter_f(a))) return x;
  if ((x = findcenter_u(a))) return x;
  if ((x = findcenter_l(a))) return x;
  return 0;
}
// find given center not on front
const int mcube::findcenter_not_f(int a)
{
  int x;
  if ((x = findcenter_l(a))) return x;
  if ((x = findcenter_u(a))) return x;
  if ((x = findcenter_r(a))) return x;
  if ((x = findcenter_d(a))) return x;
  if ((x = findcenter_b(a))) return x;
  return 0;
}
// find given center not on back
const int mcube::findcenter_not_b(int a)
{
  int x;
  if ((x = findcenter_d(a))) return x;
  if ((x = findcenter_r(a))) return x;
  if ((x = findcenter_u(a))) return x;
  if ((x = findcenter_l(a))) return x;
  if ((x = findcenter_f(a))) return x;
  return 0;
}
// find given center
const int mcube::findcenter(int a)
{
  int x;
  if ((x = findcenter_u(a))) return x;
  if ((x = findcenter_l(a))) return x;
  if ((x = findcenter_f(a))) return x;
  if ((x = findcenter_r(a))) return x;
  if ((x = findcenter_b(a))) return x;
  if ((x = findcenter_d(a))) return x;
  return 0;
}
// find free center on top
const int mcube::find_not_center_u(int a)
{
  if      (cube[2][5][2] != a)
  { fx = 2; fy = 5; fz = 2; return POSY; }
  else if (cube[2][5][3] != a)
  { fx = 2; fy = 5; fz = 3; return POSY; }
  else if (cube[3][5][2] != a)
  { fx = 3; fy = 5; fz = 2; return POSY; }
  else if (cube[3][5][3] != a)
  { fx = 3; fy = 5; fz = 3; return POSY; }
  return 0;
}
// find free center on bottom
const int mcube::find_not_center_d(int a)
{
  if      (cube[2][0][2] != a)
  { fx = 2; fy = 0; fz = 2; return NEGY; }
  else if (cube[2][0][3] != a)
  { fx = 2; fy = 0; fz = 3; return NEGY; }
  else if (cube[3][0][2] != a)
  { fx = 3; fy = 0; fz = 2; return NEGY; }
  else if (cube[3][0][3] != a)
  { fx = 3; fy = 0; fz = 3; return NEGY; }
  return 0;
}
// find free center on left
const int mcube::find_not_center_l(int a)
{
  if      (cube[0][2][2] != a)
  { fx = 0; fy = 2; fz = 2; return NEGX; }
  else if (cube[0][2][3] != a)
  { fx = 0; fy = 2; fz = 3; return NEGX; }
  else if (cube[0][3][2] != a)
  { fx = 0; fy = 3; fz = 2; return NEGX; }
  else if (cube[0][3][3] != a)
  { fx = 0; fy = 3; fz = 3; return NEGX; }
  return 0;
}
// find free center on right
const int mcube::find_not_center_r(int a)
{
  if      (cube[5][2][2] != a)
  { fx = 5; fy = 2; fz = 2; return POSX; }
  else if (cube[5][2][3] != a)
  { fx = 5; fy = 2; fz = 3; return POSX; }
  else if (cube[5][3][2] != a)
  { fx = 5; fy = 3; fz = 2; return POSX; }
  else if (cube[5][3][3] != a)
  { fx = 5; fy = 3; fz = 3; return POSX; }
  return 0;
}
// find free center on front
const int mcube::find_not_center_f(int a)
{
  if      (cube[2][2][0] != a)
  { fx = 2; fy = 2; fz = 0; return NEGZ; }
  else if (cube[2][3][0] != a)
  { fx = 2; fy = 3; fz = 0; return NEGZ; }
  else if (cube[3][2][0] != a)
  { fx = 3; fy = 2; fz = 0; return NEGZ; }
  else if (cube[3][3][0] != a)
  { fx = 3; fy = 3; fz = 0; return NEGZ; }
  return 0;
}
// find free center on back
const int mcube::find_not_center_b(int a)
{
  if      (cube[2][2][5] != a)
  { fx = 2; fy = 2; fz = 5; return POSZ; }
  else if (cube[2][3][5] != a)
  { fx = 2; fy = 3; fz = 5; return POSZ; }
  else if (cube[3][2][5] != a)
  { fx = 3; fy = 2; fz = 5; return POSZ; }
  else if (cube[3][3][5] != a)
  { fx = 3; fy = 3; fz = 5; return POSZ; }
  return 0;
}
// find given left edge
const int mcube::findedge_l(int a, int b)
{
  // for speed I just broke this into a series of if's instead of for's. it can
  // be generalized for NxNxN by replacing [2] with [x  ] and [3] with [N-x],
  // and adding a parameter for which depth edge to look for (x).
  // IMHO: an elseif keyword would be nice.....
  // also, because of the way parity works, although there is 2 of every edge on
  // a 4x4x4 cube, they cannot swap rows without also flipping, which makes them
  // unique; therefore we have findedge_l and _r for the "left" and "right" edge
  if      (cube[2][5][4] == a && cube[2][4][5] == b) // top back
  { fx = 2; fy = 5; fz = 4; return POSY; }
  else if (cube[3][4][5] == a && cube[3][5][4] == b)
  { fx = 3; fy = 4; fz = 5; return POSZ; }
  else if (cube[3][5][1] == a && cube[3][4][0] == b) // top front
  { fx = 3; fy = 5; fz = 1; return POSY; }
  else if (cube[2][4][0] == a && cube[2][5][1] == b)
  { fx = 2; fy = 4; fz = 0; return NEGZ; }
  else if (cube[1][5][2] == a && cube[0][4][2] == b) // top left
  { fx = 1; fy = 5; fz = 2; return POSY; }
  else if (cube[0][4][3] == a && cube[1][5][3] == b)
  { fx = 0; fy = 4; fz = 3; return NEGX; }
  else if (cube[4][5][3] == a && cube[5][4][3] == b) // top right
  { fx = 4; fy = 5; fz = 3; return POSY; }
  else if (cube[5][4][2] == a && cube[4][5][2] == b)
  { fx = 5; fy = 4; fz = 2; return POSX; }
  else if (cube[2][0][1] == a && cube[2][1][0] == b) // bottom front
  { fx = 2; fy = 0; fz = 1; return NEGY; }
  else if (cube[3][1][0] == a && cube[3][0][1] == b)
  { fx = 3; fy = 1; fz = 0; return NEGZ; }
  else if (cube[3][0][4] == a && cube[3][1][5] == b) // bottom back
  { fx = 3; fy = 0; fz = 4; return NEGY; }
  else if (cube[2][1][5] == a && cube[2][0][4] == b)
  { fx = 2; fy = 1; fz = 5; return POSZ; }
  else if (cube[1][0][3] == a && cube[0][1][3] == b) // bottom left
  { fx = 1; fy = 0; fz = 3; return NEGY; }
  else if (cube[0][1][2] == a && cube[1][0][2] == b)
  { fx = 0; fy = 1; fz = 2; return NEGX; }
  else if (cube[4][0][2] == a && cube[5][1][2] == b) // bottom right
  { fx = 4; fy = 0; fz = 2; return NEGY; }
  else if (cube[5][1][3] == a && cube[4][0][3] == b)
  { fx = 5; fy = 1; fz = 3; return POSX; }
  else if (cube[1][2][0] == a && cube[0][2][1] == b) // front left
  { fx = 1; fy = 2; fz = 0; return NEGZ; }
  else if (cube[0][3][1] == a && cube[1][3][0] == b)
  { fx = 0; fy = 3; fz = 1; return NEGX; }
  else if (cube[4][3][0] == a && cube[5][3][1] == b) // front right
  { fx = 4; fy = 3; fz = 0; return NEGZ; }
  else if (cube[5][2][1] == a && cube[4][2][0] == b)
  { fx = 5; fy = 2; fz = 1; return POSX; }
  else if (cube[1][3][5] == a && cube[0][3][4] == b) // back left
  { fx = 1; fy = 3; fz = 5; return POSZ; }
  else if (cube[0][2][4] == a && cube[1][2][5] == b)
  { fx = 0; fy = 2; fz = 4; return NEGX; }
  else if (cube[4][2][5] == a && cube[5][2][4] == b) // back right
  { fx = 4; fy = 2; fz = 5; return POSZ; }
  else if (cube[5][3][4] == a && cube[4][3][5] == b)
  { fx = 5; fy = 3; fz = 4; return POSX; }
  return 0;
}
// find given right edge
const int mcube::findedge_r(int a, int b)
{
  // for speed I just broke this into a series of if's instead of for's
  // also see note on NxNxN generalization from findedge_l
  if      (cube[3][5][4] == a && cube[3][4][5] == b) // top back
  { fx = 3; fy = 5; fz = 4; return POSY; }
  else if (cube[2][4][5] == a && cube[2][5][4] == b)
  { fx = 2; fy = 4; fz = 5; return POSZ; }
  else if (cube[2][5][1] == a && cube[2][4][0] == b) // top front
  { fx = 2; fy = 5; fz = 1; return POSY; }
  else if (cube[3][4][0] == a && cube[3][5][1] == b)
  { fx = 3; fy = 4; fz = 0; return NEGZ; }
  else if (cube[1][5][3] == a && cube[0][4][3] == b) // top left
  { fx = 1; fy = 5; fz = 3; return POSY; }
  else if (cube[0][4][2] == a && cube[1][5][2] == b)
  { fx = 0; fy = 4; fz = 2; return NEGX; }
  else if (cube[4][5][2] == a && cube[5][4][2] == b) // top right
  { fx = 4; fy = 5; fz = 2; return POSY; }
  else if (cube[5][4][3] == a && cube[4][5][3] == b)
  { fx = 5; fy = 4; fz = 3; return POSX; }
  else if (cube[3][0][1] == a && cube[3][1][0] == b) // bottom front
  { fx = 3; fy = 0; fz = 1; return NEGY; }
  else if (cube[2][1][0] == a && cube[2][0][1] == b)
  { fx = 2; fy = 1; fz = 0; return NEGZ; }
  else if (cube[2][0][4] == a && cube[2][1][5] == b) // bottom back
  { fx = 2; fy = 0; fz = 4; return NEGY; }
  else if (cube[3][1][5] == a && cube[3][0][4] == b)
  { fx = 3; fy = 1; fz = 5; return POSZ; }
  else if (cube[1][0][2] == a && cube[0][1][2] == b) // bottom left
  { fx = 1; fy = 0; fz = 2; return NEGY; }
  else if (cube[0][1][3] == a && cube[1][0][3] == b)
  { fx = 0; fy = 1; fz = 3; return NEGX; }
  else if (cube[4][0][3] == a && cube[5][1][3] == b) // bottom right
  { fx = 4; fy = 0; fz = 3; return NEGY; }
  else if (cube[5][1][2] == a && cube[4][0][2] == b)
  { fx = 5; fy = 1; fz = 2; return POSX; }
  else if (cube[1][3][0] == a && cube[0][3][1] == b) // front left
  { fx = 1; fy = 3; fz = 0; return NEGZ; }
  else if (cube[0][2][1] == a && cube[1][2][0] == b)
  { fx = 0; fy = 2; fz = 1; return NEGX; }
  else if (cube[4][2][0] == a && cube[5][2][1] == b) // front right
  { fx = 4; fy = 2; fz = 0; return NEGZ; }
  else if (cube[5][3][1] == a && cube[4][3][0] == b)
  { fx = 5; fy = 3; fz = 1; return POSX; }
  else if (cube[1][2][5] == a && cube[0][2][4] == b) // back left
  { fx = 1; fy = 2; fz = 5; return POSZ; }
  else if (cube[0][3][4] == a && cube[1][3][5] == b)
  { fx = 0; fy = 3; fz = 4; return NEGX; }
  else if (cube[4][3][5] == a && cube[5][3][4] == b) // back right
  { fx = 4; fy = 3; fz = 5; return POSZ; }
  else if (cube[5][2][4] == a && cube[4][2][5] == b)
  { fx = 5; fy = 2; fz = 4; return POSX; }
  return 0;
}
// find given edge
const int mcube::findedge(int a, int b)
{
  int x;
  if ((x = findedge_l(a, b))) return x;
  if ((x = findedge_r(a, b))) return x;
  return 0;
}
// find given corner in clockwise order
const int mcube::findcorner_c(int a, int b, int c)
{
  // for speed I just broke this into a series of if's instead of for's
  if      (cube[1][5][1] == a && cube[1][4][0] == b && cube[0][4][1] == c) // top left front
  { fx = 1; fy = 5; fz = 1; return POSY; }
  else if (cube[1][4][0] == a && cube[0][4][1] == b && cube[1][5][1] == c)
  { fx = 1; fy = 4; fz = 0; return NEGZ; }
  else if (cube[0][4][1] == a && cube[1][5][1] == b && cube[1][4][0] == c)
  { fx = 0; fy = 4; fz = 1; return NEGX; }
  else if (cube[4][5][1] == a && cube[5][4][1] == b && cube[4][4][0] == c) // top right front
  { fx = 4; fy = 5; fz = 1; return POSY; }
  else if (cube[5][4][1] == a && cube[4][4][0] == b && cube[4][5][1] == c)
  { fx = 5; fy = 4; fz = 1; return POSX; }
  else if (cube[4][4][0] == a && cube[4][5][1] == b && cube[5][4][1] == c)
  { fx = 4; fy = 4; fz = 0; return NEGZ; }
  else if (cube[1][5][4] == a && cube[0][4][4] == b && cube[1][4][5] == c) // top left back
  { fx = 1; fy = 5; fz = 4; return POSY; }
  else if (cube[0][4][4] == a && cube[1][4][5] == b && cube[1][5][4] == c)
  { fx = 0; fy = 4; fz = 4; return NEGX; }
  else if (cube[1][4][5] == a && cube[1][5][4] == b && cube[0][4][4] == c)
  { fx = 1; fy = 4; fz = 5; return POSZ; }
  else if (cube[4][5][4] == a && cube[4][4][5] == b && cube[5][4][4] == c) // top right back
  { fx = 4; fy = 5; fz = 4; return POSY; }
  else if (cube[4][4][5] == a && cube[5][4][4] == b && cube[4][5][4] == c)
  { fx = 4; fy = 4; fz = 5; return POSZ; }
  else if (cube[5][4][4] == a && cube[4][5][4] == b && cube[4][4][5] == c)
  { fx = 5; fy = 4; fz = 4; return POSX; }
  else if (cube[1][0][1] == a && cube[0][1][1] == b && cube[1][1][0] == c) // bottom left front
  { fx = 1; fy = 0; fz = 1; return NEGY; }
  else if (cube[0][1][1] == a && cube[1][1][0] == b && cube[1][0][1] == c)
  { fx = 0; fy = 1; fz = 1; return NEGX; }
  else if (cube[1][1][0] == a && cube[1][0][1] == b && cube[0][1][1] == c)
  { fx = 1; fy = 1; fz = 0; return NEGZ; }
  else if (cube[4][0][1] == a && cube[4][1][0] == b && cube[5][1][1] == c) // bottom right front
  { fx = 4; fy = 0; fz = 1; return NEGY; }
  else if (cube[4][1][0] == a && cube[5][1][1] == b && cube[4][0][1] == c)
  { fx = 4; fy = 1; fz = 0; return NEGZ; }
  else if (cube[5][1][1] == a && cube[4][0][1] == b && cube[4][1][0] == c)
  { fx = 5; fy = 1; fz = 1; return POSX; }
  else if (cube[1][0][4] == a && cube[1][1][5] == b && cube[0][1][4] == c) // bottom left back
  { fx = 1; fy = 0; fz = 4; return NEGY; }
  else if (cube[1][1][5] == a && cube[0][1][4] == b && cube[1][0][4] == c)
  { fx = 1; fy = 1; fz = 5; return POSZ; }
  else if (cube[0][1][4] == a && cube[1][0][4] == b && cube[1][1][5] == c)
  { fx = 0; fy = 1; fz = 4; return NEGX; }
  else if (cube[4][0][4] == a && cube[5][1][4] == b && cube[4][1][5] == c) // bottom right back
  { fx = 4; fy = 0; fz = 4; return NEGY; }
  else if (cube[5][1][4] == a && cube[4][1][5] == b && cube[4][0][4] == c)
  { fx = 5; fy = 1; fz = 4; return POSX; }
  else if (cube[4][1][5] == a && cube[4][0][4] == b && cube[5][1][4] == c)
  { fx = 4; fy = 1; fz = 5; return POSZ; }
  return 0;
}
// find given corner in counterclockwise order
const int mcube::findcorner_a(int a, int b, int c)
{
  // for speed I just broke this into a series of if's instead of for's
  if      (cube[1][5][1] == a && cube[0][4][1] == b && cube[1][4][0] == c) // top left front
  { fx = 1; fy = 5; fz = 1; return POSY; }
  else if (cube[1][4][0] == a && cube[1][5][1] == b && cube[0][4][1] == c)
  { fx = 1; fy = 4; fz = 0; return NEGZ; }
  else if (cube[0][4][1] == a && cube[1][4][0] == b && cube[1][5][1] == c)
  { fx = 0; fy = 4; fz = 1; return NEGX; }
  else if (cube[4][5][1] == a && cube[4][4][0] == b && cube[5][4][1] == c) // top right front
  { fx = 4; fy = 5; fz = 1; return POSY; }
  else if (cube[5][4][1] == a && cube[4][5][1] == b && cube[4][4][0] == c)
  { fx = 5; fy = 4; fz = 1; return POSX; }
  else if (cube[4][4][0] == a && cube[5][4][1] == b && cube[4][5][1] == c)
  { fx = 4; fy = 4; fz = 0; return NEGZ; }
  else if (cube[1][5][4] == a && cube[1][4][5] == b && cube[0][4][4] == c) // top left back
  { fx = 1; fy = 5; fz = 4; return POSY; }
  else if (cube[0][4][4] == a && cube[1][5][4] == b && cube[1][4][5] == c)
  { fx = 0; fy = 4; fz = 4; return NEGX; }
  else if (cube[1][4][5] == a && cube[0][4][4] == b && cube[1][5][4] == c)
  { fx = 1; fy = 4; fz = 5; return POSZ; }
  else if (cube[4][5][4] == a && cube[5][4][4] == b && cube[4][4][5] == c) // top right back
  { fx = 4; fy = 5; fz = 4; return POSY; }
  else if (cube[4][4][5] == a && cube[4][5][4] == b && cube[5][4][4] == c)
  { fx = 4; fy = 4; fz = 5; return POSZ; }
  else if (cube[5][4][4] == a && cube[4][4][5] == b && cube[4][5][4] == c)
  { fx = 5; fy = 4; fz = 4; return POSX; }
  else if (cube[1][0][1] == a && cube[1][1][0] == b && cube[0][1][1] == c) // bottom left front
  { fx = 1; fy = 0; fz = 1; return NEGY; }
  else if (cube[0][1][1] == a && cube[1][0][1] == b && cube[1][1][0] == c)
  { fx = 0; fy = 1; fz = 1; return NEGX; }
  else if (cube[1][1][0] == a && cube[0][1][1] == b && cube[1][0][1] == c)
  { fx = 1; fy = 1; fz = 0; return NEGZ; }
  else if (cube[4][0][1] == a && cube[5][1][1] == b && cube[4][1][0] == c) // bottom right front
  { fx = 4; fy = 0; fz = 1; return NEGY; }
  else if (cube[4][1][0] == a && cube[4][0][1] == b && cube[5][1][1] == c)
  { fx = 4; fy = 1; fz = 0; return NEGZ; }
  else if (cube[5][1][1] == a && cube[4][1][0] == b && cube[4][0][1] == c)
  { fx = 5; fy = 1; fz = 1; return POSX; }
  else if (cube[1][0][4] == a && cube[0][1][4] == b && cube[1][1][5] == c) // bottom left back
  { fx = 1; fy = 0; fz = 4; return NEGY; }
  else if (cube[1][1][5] == a && cube[1][0][4] == b && cube[0][1][4] == c)
  { fx = 1; fy = 1; fz = 5; return POSZ; }
  else if (cube[0][1][4] == a && cube[1][1][5] == b && cube[1][0][4] == c)
  { fx = 0; fy = 1; fz = 4; return NEGX; }
  else if (cube[4][0][4] == a && cube[4][1][5] == b && cube[5][1][4] == c) // bottom right back
  { fx = 4; fy = 0; fz = 4; return NEGY; }
  else if (cube[5][1][4] == a && cube[4][0][4] == b && cube[4][1][5] == c)
  { fx = 5; fy = 1; fz = 4; return POSX; }
  else if (cube[4][1][5] == a && cube[5][1][4] == b && cube[4][0][4] == c)
  { fx = 4; fy = 1; fz = 5; return POSZ; }
  return 0;
}
// find given corner
const int mcube::findcorner(int a, int b, int c)
{
  int x;
  if ((x = findcorner_c(a, b, c))) return x;
  if ((x = findcorner_a(a, b, c))) return x;
  return 0;
}
// convert standard string to metric
const string mcube::std_to_metr(string s)
{
  string a = s;
  char slice_std, slice_metr='\0', dir, dir_prime='\0';
  for (int i = 1; i <= (int)(s.length() / 3); i++)
  {
    slice_std = s.at(i * 3 - 3);
    dir = s.at(i * 3 - 2);
    if (slice_std == 'C')
      continue;
    if (dir == '2')
    {
      if
      (slice_std == 'U' || slice_std == 'u' || slice_std == 'd' || slice_std == 'D')
        dir_prime = 'L';
      else if
      (slice_std == 'L' || slice_std == 'l' || slice_std == 'r' || slice_std == 'R')
        dir_prime = 'U';
      else if
      (slice_std == 'F' || slice_std == 'f' || slice_std == 'b' || slice_std == 'B')
        dir_prime = 'C';
    }
    if
    (slice_std == 'D' || slice_std == 'L' || slice_std == 'F')
      slice_metr = '1';
    else if
    (slice_std == 'd' || slice_std == 'l' || slice_std == 'f')
      slice_metr = '2';
    else if
    (slice_std == 'u' || slice_std == 'r' || slice_std == 'b')
      slice_metr = '3';
    else if
    (slice_std == 'U' || slice_std == 'R' || slice_std == 'B')
      slice_metr = '4';
    a.at(i * 3 - 3) = slice_metr;
    if (dir == '2')
    {
      a.at(i * 3 - 2) = dir_prime;
      a.insert(i * 3, "  .");
      a.at(i * 3) = slice_metr;
      a.at(i * 3 + 1) = dir_prime;
      i++;
    }
  }
  return a;
}
// convert metric string to standard
const string mcube::metr_to_std(string s)
{
  string a = s;
  char slice_std='\0', slice_metr, dir;
  for (int i = 1; i <= (int)(s.length() / 3); i++)
  {
    slice_metr = s.at(i * 3 - 3);
    dir = s.at(i * 3 - 2);
    if (slice_metr == 'C')
      continue;
    switch (dir)
    {
    case 'L':
    case 'R':
      switch (slice_metr)
      {
        case '1': slice_std = 'D'; break;
        case '2': slice_std = 'd'; break;
        case '3': slice_std = 'u'; break;
        case '4': slice_std = 'U'; break;
      }
      break;
    case 'U':
    case 'D':
      switch (slice_metr)
      {
        case '1': slice_std = 'L'; break;
        case '2': slice_std = 'l'; break;
        case '3': slice_std = 'r'; break;
        case '4': slice_std = 'R'; break;
      }
      break;
    case 'C':
    case 'A':
      switch (slice_metr)
      {
        case '1': slice_std = 'F'; break;
        case '2': slice_std = 'f'; break;
        case '3': slice_std = 'b'; break;
        case '4': slice_std = 'B'; break;
      }
      break;
    } 
    a.at(i * 3 - 3) = slice_std;
  }
  return a;
}
// convert standard string to relative
const string mcube::std_to_rel(string s)
{
  string a = s;
  char slice, dir_std, dir_rel='\0';
  for (int i = 1; i <= (int)(s.length() / 3); i++)
  {
    slice = s.at(i * 3 - 3);
    dir_std = s.at(i * 3 - 2);
    if (slice == 'C')
      continue;
    switch (dir_std)
    {
      case 'L': dir_rel = '+'; break;
      case 'R': dir_rel = '-'; break;
      case 'U': dir_rel = '-'; break;
      case 'D': dir_rel = '+'; break;
      case 'C': dir_rel = '+'; break;
      case 'A': dir_rel = '-'; break;
      case '2': dir_rel = '2'; break;
    }
    a.at(i * 3 - 2) = dir_rel;
  }
  return a;
}
// convert relative string to standard
const string mcube::rel_to_std(string s)
{
  string a = s;
  char slice, dir_std='\0', dir_rel;
  for (int i = 1; i <= (int)(s.length() / 3); i++)
  {
    slice = s.at(i * 3 - 3);
    dir_rel = s.at(i * 3 - 2);
    if (slice == 'C')
      continue;
    switch (slice)
    {
    case 'U':
    case 'u':
    case 'd':
    case 'D':
      switch (dir_rel)
      {
        case '+': dir_std = 'L'; break;
        case '-': dir_std = 'R'; break;
      }
      break;
    case 'L':
    case 'l':
    case 'r':
    case 'R':
      switch (dir_rel)
      {
        case '+': dir_std = 'D'; break;
        case '-': dir_std = 'U'; break;
      }
      break;
    case 'F':
    case 'f':
    case 'b':
    case 'B':
      switch (dir_rel)
      {
        case '+': dir_std = 'C'; break;
        case '-': dir_std = 'A'; break;
      }
      break;
    }
    if (dir_rel == '2')
      dir_std = '2';
    a.at(i * 3 - 2) = dir_std;
  }
  return a;
}
// convert metric string to relative
const string mcube::metr_to_rel(string s)
{
  return std_to_rel(metr_to_std(s)); // the lazy way
}
// convert relative string to metric
const string mcube::rel_to_metr(string s)
{
  return std_to_metr(rel_to_std(s)); // the lazy way
}
// allow the use of the half turn
const string mcube::usehalfturns(string s, int b=0)
{
  // note: ALL this routine does is change things like LU.LU. to L2.
  // it does NOT change LU.LU.LU. to LD. nor does it strip out things
  // like L2.L2.  Use concise() for that.
  string a = s;
  char slice, dir, slice_next, dir_next;
  for (int i = 1; i <= (int)(a.length() / 3) - 1; i++)
  {
    slice = a.at(i * 3 - 3);
    dir = a.at(i * 3 - 2);
    slice_next = a.at(i * 3);
    dir_next = a.at(i * 3 + 1);
    if (slice == 'C' || dir == '2') continue;
    if (slice == slice_next && dir == dir_next)
    {
      a.at(i * 3 - 2) = '2';
      a.erase(i * 3, 3);
      // change mov[] if necessary
      if (b) {
        shortenmov(i);
      }
      //
      i--;
    }
  }
  return a;
}
// strip out redundant moves from a solution string
const string mcube::concise(string s, int b=0)
{
  int strips = 0;
  // total revamp, Nov/04
  string a = std_to_metr(s);
  int redundancies;
  // step 1: interpolate whole cube moves, e.g., CL,LU,FC, to FA,RU,
  {
    char slice, dir;
    int sUD, sLR, sFB;
    char dL, dR, dU, dD, dC, dA;
    for (int i = 1; i <= (int)(a.length() / 3); i++)
    {
      slice = a.at(i * 3 - 3);
      if (slice == 'C')
      {
        dir = a.at(i * 3 - 2);
        sUD = 1; sLR = 1; sFB = 1;
        dL = 'L'; dR = 'R'; dU = 'U'; dD = 'D'; dC = 'C'; dA = 'A';
        switch (dir)
        {
        case 'L':
          sFB = -1; dU = 'A'; dD = 'C'; dC = 'U'; dA = 'D'; break;
        case 'R':
          sLR = -1; dU = 'C'; dD = 'A'; dC = 'D'; dA = 'U'; break;
        case 'U':
          sUD = -1; dL = 'C'; dR = 'A'; dC = 'R'; dA = 'L'; break;
        case 'D':
          sFB = -1; dL = 'A'; dR = 'C'; dC = 'L'; dA = 'R'; break;
        case 'C':
          sUD = -1; dL = 'D'; dR = 'U'; dU = 'L'; dD = 'R'; break;
        case 'A':
          sLR = -1; dL = 'U'; dR = 'D'; dU = 'R'; dD = 'L'; break;
        }
        a.erase(i * 3 - 3, 3);
        // change mov[] if necessary
        if (b) {
          strips++; if (strips > b) shortenmov(i);
        }
        //
        for (int j = i; j <= (int)(a.length() / 3); j++)
        {
          slice = a.at(j * 3 - 3);
          dir = a.at(j * 3 - 2);
          switch (dir)
          {
          case 'L':
            if (sUD == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dL; break;
          case 'R':
            if (sUD == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dR; break;
          case 'U':
            if (sLR == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dU; break;
          case 'D':
            if (sLR == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dD; break;
          case 'C':
            if (sFB == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dC; break;
          case 'A':
            if (sFB == -1 && slice != 'C') slice = ((N+1-(slice-'0'))+'0');
            dir = dA; break;
          }
          a.at(j * 3 - 3) = slice;
          a.at(j * 3 - 2) = dir;
        }
        i--;
      }
    }
  }
  // the next steps are nested to make this condensation iterative
  redundancies = 1;
  while (redundancies)
  {
    redundancies = 0;
    // step 2: re-order parallel slice groups, e.g., LU,RU,LD,RU, to LU,LD,RU,RU,
    {
      char slice, dir, slice_next, dir_next, dir_reverse='\0';
      for (int i = 1; i <= (int)(a.length() / 3) - 2; i++)
      {
        slice = a.at(i * 3 - 3);
        dir = a.at(i * 3 - 2);
        slice_next = a.at(i * 3);
        dir_next = a.at(i * 3 + 1);
        if (slice_next == slice) continue;
        switch (dir)
        {
          case 'L': dir_reverse = 'R'; break;
          case 'R': dir_reverse = 'L'; break;
          case 'U': dir_reverse = 'D'; break;
          case 'D': dir_reverse = 'U'; break;
          case 'C': dir_reverse = 'A'; break;
          case 'A': dir_reverse = 'C'; break;
        }
        for
        (int j = i + 2;
         j <= (int)(a.length() / 3) && (dir_next == dir || dir_next == dir_reverse);
         j++ )
        {
          slice_next = a.at(j * 3 - 3);
          dir_next = a.at(j * 3 - 2);
          if
          (slice_next == slice && (dir_next == dir || dir_next == dir_reverse))
          {
            for (int k = j; k >= i + 2; k--)
            {
              a.at(k * 3 - 3) = a.at(k * 3 - 6);
              a.at(k * 3 - 2) = a.at(k * 3 - 5);
            }
            a.at(i * 3) = slice_next;
            a.at(i * 3 + 1) = dir_next;
            i++;
          }
        }
      }
    }
    // step 3: change 3/4 turns to 1/4 turn, e.g., LU,LU,LU, to LD,
    {
      char slice, dir, slice_next, dir_next, slice_after, dir_after, dir_reverse='\0';
      for (int i = 1; i <= (int)(a.length() / 3) - 2; i++)
      {
        slice = a.at(i * 3 - 3);
        dir = a.at(i * 3 - 2);
        slice_next = a.at(i * 3);
        dir_next = a.at(i * 3 + 1);
        slice_after = a.at(i * 3 + 3);
        dir_after = a.at(i * 3 + 4);
        if
        ((slice == slice_next) && (slice == slice_after) &&
         (dir == dir_next) && (dir == dir_after) )
        {
          switch (dir)
          {
            case 'L': dir_reverse = 'R'; break;
            case 'R': dir_reverse = 'L'; break;
            case 'U': dir_reverse = 'D'; break;
            case 'D': dir_reverse = 'U'; break;
            case 'C': dir_reverse = 'A'; break;
            case 'A': dir_reverse = 'C'; break;
          }
          a.at(i * 3 - 2) = dir_reverse;
          a.erase(i * 3, 6);
          // change mov[] if necessary
          if (b) {
            strips++; if (strips > b) shortenmov(i);
            strips++; if (strips > b) shortenmov(i);
          }
          //
          i -= 2; if (i < 0) i = 0;
          redundancies++;
        }
      }
    }
    // step 4: remove contradicting moves, e.g., LU,LD,
    {
      char slice, dir, slice_next, dir_next, dir_reverse='\0';
      for (int i = 1; i <= (int)(a.length() / 3) - 1; i++)
      {
        slice = a.at(i * 3 - 3);
        dir = a.at(i * 3 - 2);
        slice_next = a.at(i * 3);
        dir_next = a.at(i * 3 + 1);
        switch (dir)
        {
          case 'L': dir_reverse = 'R'; break;
          case 'R': dir_reverse = 'L'; break;
          case 'U': dir_reverse = 'D'; break;
          case 'D': dir_reverse = 'U'; break;
          case 'C': dir_reverse = 'A'; break;
          case 'A': dir_reverse = 'C'; break;
        }
        if (slice == slice_next && dir_next == dir_reverse)
        {
          a.erase(i * 3 - 3, 6);
          // change mov[] if necessary
          if (b) {
            strips++; if (strips > b) shortenmov(i);
            strips++; if (strips > b) shortenmov(i);
          }
          //
          i -= 2; if (i < 0) i = 0;
          redundancies++;
        }
      }
    }
    // now try again until no redundancies are found.
  }
  // something to consider adding to this algorithm:
  // seek out patterns like "UL.uL.dL.DL." and change them to "CL." and start over, or
  // changing "UL.uL.dL." to "CL.DR." and starting over...
  return metr_to_std(a);
}
// shorten mov[]
const void mcube::shortenmov(int m)
{
  int tot = 0, cur = 0;
  for (int i = 1; i <= MOV; i++) {
    tot += mov[i];
    while (tot >= m && cur < 1) {
      mov[i]--;
      tot--;
      cur++;
    }
  }
}
// solve the cube
const int mcube::solvecube()
{
  // possible return codes:
  // 0, ERR_NOTINITED, ERR_MISPAINTED, ERR_NONDESCRIPT,
  // ERR_PARITY_EDGE_FLIP, ERR_PARITY_CORNER_ROTATION,
  // ERR_PARITY_CORNER_BACKWARD, ERR_PARITY_CENTER_SWAP
  if (!inited) // make sure cube is initialized
    return (erval = ERR_NOTINITED);
  erval = 0;
  int tcube[N+2][N+2][N+2];
  int tmov[MOV+1], curmoves;
  int U=0, D=0, L=0, R=0, F=0, B=0;
  int cen[7], parity[2];
  string tsolution = "";
  string prefix = "";
  string cursolution;
  // clear mov[] and solution
  solution = "";
  for (int i = 0; i <= MOV; i++)
    mov[i] = 0;
  for (int i = 1; i <= MOV; i++)
    tmov[i] = 0;
  tmov[0] = -1;
  // step 1(ish) back up original cube
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        tcube[i][j][k] = cube[i][j][k];
  // make sure there is the right number of each color
  for (int i = 0; i <= 6; i++)
    cen[i] = 0;
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        if (cube[i][j][k] >= 1 && cube[i][j][k] <= 6)
          cen[cube[i][j][k]]++;
  for (int i = 1; i <= 6; i++)
    if (cen[i] != N * N)
      return (erval = ERR_MISPAINTED);
  // set up interpolation table so U,D,L,R,F,B=1,2,3,4,5,6
  for (int i = 1; i <= 6; i++)
    cen[i] = 0;
  U = cube[1][N+1][1];
  L = cube[0][N][1];
  F = cube[1][N][0];
  for (int i = 1; i <= 6; i++)
  {
    if (i != U && i != L && i != F)
    {
      if      (findcorner_c(F,U,i)) R = i;
      else if (findcorner_c(U,L,i)) B = i;
      else if (findcorner_c(L,F,i)) D = i;
      else if (findcorner_a(F,U,i)) R = i; // normally there'd just be findcorner_c,
      else if (findcorner_a(U,L,i)) B = i; // but I added _a to find backward corner
      else if (findcorner_a(L,F,i)) D = i; // parity errors...
    }
  }
  // make sure we have different colors for every face
  if (U == L || L == F || F == U || R == 0 || D == 0 || B == 0)
    return (erval = ERR_MISPAINTED);
  cen[U] = 1; cen[L] = 2; cen[F] = 3;
  cen[R] = 4; cen[B] = 5; cen[D] = 6;
  // apply interpolation
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      cube[i][N+1][j] = cen[cube[i][N+1][j]];
      cube[0][i][j]   = cen[cube[0][i][j]];
      cube[i][j][0]   = cen[cube[i][j][0]];
      cube[N+1][i][j] = cen[cube[N+1][i][j]];
      cube[i][j][N+1] = cen[cube[i][j][N+1]];
      cube[i][0][j]   = cen[cube[i][0][j]];
    }
  }
  // step 2(ish) make sure all cubelets are present (e.g., cube all there)
  /*
  for (int i = 2; i <= 5; i++)
  {
    if
    (!findedge_l(1, i) ||
     !findedge_r(1, i) ||
     !findedge_l(6, i) ||
     !findedge_r(6, i) ||
     !findedge_l(i, (i<5?i+1:i-3)) ||
     !findedge_r(i, (i<5?i+1:i-3)) ||
     !findcorner_c(1, (i<5?i+1:i-3), i) ||
     !findcorner_c(6, i, (i<5?i+1:i-3)) )
      // don't want to return just yet coz of color interpolation
      erval = ERR_MISPAINTED;
    // note: we don't need to count the number of centers of each color...
  }
  */
  // revamped version designed to be able to find "edge flip parity" problems
  // first check centers...
  for (int i = 1; i <= 6; i++)
    cen[i] = 0;
  for (int i = 2; i <= N-1; i++)
  {
    for (int j = 2; j <= N-1; j++)
    {
      cen[cube[i][N+1][j]]++;
      cen[cube[0][i][j]]++;
      cen[cube[i][j][0]]++;
      cen[cube[N+1][i][j]]++;
      cen[cube[i][j][N+1]]++;
      cen[cube[i][0][j]]++;
    }
  }
  for (int i = 1; i <= 6 && !erval; i++)
    if (cen[i] != (N-2) * (N-2))
      erval = ERR_MISPAINTED;
  // now check corners (more complicated)...
  parity[0] = 0;
  for (int i = 2; i <= 5 && !erval; i++)
  {
    if (!findcorner_c(1, (i<5?i+1:i-3), i)) {
      if (findcorner_a(1, (i<5?i+1:i-3), i)) parity[0]++;
      else erval = ERR_MISPAINTED;
    }
    if (!findcorner_c(6, i, (i<5?i+1:i-3))) {
      if (findcorner_a(6, i, (i<5?i+1:i-3))) parity[0]++;
      else erval = ERR_MISPAINTED;
    }
  }
  // now check edges (more complicated)...
  parity[1] = 0;
  for (int i = 2; i <= 5 && !erval; i++)
  {
    int tx, ty, tz;
    if (!findedge_l(1, i)) {
      if (findedge_r(1, i)) {
        tx = fx; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_r(1, i)) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = 1;
      }
      else erval = ERR_MISPAINTED;
    }
    if (!findedge_r(1, i)) {
      if (findedge_l(1, i)) {
        tx = fz; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_l(1, i)) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = 1;
      }
      else erval = ERR_MISPAINTED;
    }
    if (!findedge_l(6, i)) {
      if (findedge_r(6, i)) {
        tx = fz; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_r(6, i)) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = 6;
      }
      else erval = ERR_MISPAINTED;
    }
    if (!findedge_r(6, i)) {
      if (findedge_l(6, i)) {
        tx = fz; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_l(6, i)) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = 6;
      }
      else erval = ERR_MISPAINTED;
    }
    if (!findedge_l(i, (i<5?i+1:i-3))) {
      if (findedge_r(i, (i<5?i+1:i-3))) {
        tx = fx; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_r(i, (i<5?i+1:i-3))) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = i;
      }
      else erval = ERR_MISPAINTED;
    }
    if (!findedge_r(i, (i<5?i+1:i-3))) {
      if (findedge_l(i, (i<5?i+1:i-3))) {
        tx = fx; ty = fy; tz = fz;
        cube[tx][ty][tz] = 0;
        if (findedge_l(i, (i<5?i+1:i-3))) parity[1]++;
        else erval = ERR_MISPAINTED;
        cube[tx][ty][tz] = i;
      }
      else erval = ERR_MISPAINTED;
    }
  }
  if      (parity[0] > 0 && !erval)
    erval = ERR_PARITY_CORNER_BACKWARD;
  else if (parity[1] > 0 && !erval)
    erval = ERR_PARITY_EDGE_FLIP;
  for (int i = 0; i <= N+1; i++)
    for (int j = 0; j <= N+1; j++)
      for (int k = 0; k <= N+1; k++)
        cube[i][j][k] = tcube[i][j][k];
  // the cube appears solvable (unless there's a parity error); try solution
  if (erval) return erval;
  // step 3(ish) start from each possible orientation to find shortest solution
  for (int h = 1; h <= 6; h++)
  {
    for (int g = 1; g <= 4; g++)
    {
      for (int i = 0; i <= N+1; i++)
        for (int j = 0; j <= N+1; j++)
          for (int k = 0; k <= N+1; k++)
            tcube[i][j][k] = cube[i][j][k];
      // step 4(ish) set up interpolation table so U,D,L,R,F,B=1,2,3,4,5,6
      for (int i = 1; i <= 6; i++)
        cen[i] = 0;
      U = cube[1][N+1][1];
      L = cube[0][N][1];
      F = cube[1][N][0];
      for (int i = 1; i <= 6; i++)
      {
        if (i != U && i != L && i != F)
        {
          if      (findcorner_c(F,U,i)) R = i;
          else if (findcorner_c(L,F,i)) D = i;
          else if (findcorner_c(U,L,i)) B = i;
        }
      }
      // now set the matrix
      cen[U] = 1; cen[L] = 2; cen[F] = 3;
      cen[R] = 4; cen[B] = 5; cen[D] = 6;
      // apply interpolation
      for (int i = 1; i <= N; i++)
      {
        for (int j = 1; j <= N; j++)
        {
          cube[i][N+1][j] = cen[cube[i][N+1][j]];
          cube[0][i][j]   = cen[cube[0][i][j]];
          cube[i][j][0]   = cen[cube[i][j][0]];
          cube[N+1][i][j] = cen[cube[N+1][i][j]];
          cube[i][j][N+1] = cen[cube[i][j][N+1]];
          cube[i][0][j]   = cen[cube[i][0][j]];
        }
      }
      if (!erval)
      {
        // step 5(ish) (FINALLLLLY): find a solution
        cursolution = findsolution();
        // strip redundancies from solution...
        if (shorten >= SHORTEN_STRIP_ALL)
        {
          string allmov = prefix + cursolution;
          int prelen = (int)prefix.length() / 3;
          if (prelen == 0) prelen = -1;
          cursolution = concise(allmov, prelen);
        }
        // see if the current solution is shorter than best solution
        curmoves = (int)cursolution.length() / 3;
        if (curmoves < tmov[0] || tmov[0] < 0)
        {
          tsolution = cursolution;
          for (int i = 1; i <= MOV; i++)
            tmov[i] = mov[i];
          tmov[0] = curmoves;
        }
      }
      for (int i = 0; i <= N+1; i++)
        for (int j = 0; j <= N+1; j++)
          for (int k = 0; k <= N+1; k++)
            cube[i][j][k] = tcube[i][j][k];
      // now try the next orientation
      if (shorten < SHORTEN_STRIP_ROTATE_ALL) break;
      prefix += "CL."; CL();
    }
    if (shorten < SHORTEN_STRIP_ROTATE_SOME) break;
    if (h % 2) {
      prefix += "CU."; CU();
    }
    else {
      prefix += "CA."; CA();
    }
  }
  // step 6(ish) set the members appropriately and return
  if (erval) return erval;
  for (int i = 0; i <= MOV; i++)
    mov[i] = tmov[i];
  solution = tsolution;
  return 0;
}
// find a solution for a prepared cube
const string mcube::findsolution()
{
  string s = "";
  int step = 0;
  // step 1: top centers
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int rot = 0;
    while (!erval && findcenter_not_u(1))
    {
      if      (cube[2][5][2] != 1) ;
      else if (cube[2][5][3] != 1)
      { a += "CR."; CR(); rot=(rot+1)%4; }
      else if (cube[3][5][2] != 1)
      { a += "CL."; CL(); rot=(rot+3)%4; }
      else if (cube[3][5][3] != 1)
      { a += "CL.CL."; CL(); CL(); rot=(rot+2)%4; }
      switch (findcenter_not_u(1))
      {
      case 0:
        break;
      case NEGY:
        if      (fx == 2 && fz == 2) ;
        else if (fx == 2 && fz == 3)
        { a += "DR."; DR(); }
        else if (fx == 3 && fz == 2)
        { a += "DL."; DL(); }
        else if (fx == 3 && fz == 3)
        { a += "DL.DL."; DL(); DL(); }
        a += "lD.dR.LD.dR.lU.";
        lD(); dR(); LD(); dR(); lU();
        break;
      case NEGX:
        if      (fy == 2 && fz == 2) ;
        else if (fy == 2 && fz == 3)
        { a += "LU."; LU(); }
        else if (fy == 3 && fz == 2)
        { a += "LD."; LD(); }
        else if (fy == 3 && fz == 3)
        { a += "LD.LD."; LD(); LD(); }
        a += "rD.fC.rU.";
        rD(); fC(); rU();
        break;
      case NEGZ:
        if      (fx == 2 && fy == 2) ;
        else if (fx == 2 && fy == 3)
        { a+= "FA."; FA(); }
        else if (fx == 3 && fy == 2)
        { a += "FC."; FC(); }
        else if (fx == 3 && fy == 3)
        { a += "FA.FA."; FA(); FA(); }
        a += "bC.lU.bA.";
        bC(); lU(); bA();
        break;
      case POSX:
        if      (fy == 2 && fz == 2)
        { a += "RU."; RU(); }
        else if (fy == 2 && fz == 3)
        { a += "RD.RD."; RD(); RD(); }
        else if (fy == 3 && fz == 2) ;
        else if (fy == 3 && fz == 3)
        { a += "RD."; RD(); }
        a += "rU.fA.rD.";
        rU(); fA(); rD();
        break;
      case POSZ:
        if      (fx == 2 && fy == 2)
        { a += "BC."; BC(); }
        else if (fx == 2 && fy == 3) ;
        else if (fx == 3 && fy == 2)
        { a += "BA.BA."; BA(); BA(); }
        else if (fx == 3 && fy == 3)
        { a += "BA."; BA(); }
        a += "bC.lU.bA.";
        bC(); lU(); bA();
        break;
      }
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    switch (rot)
    {
    case 0: break;
    case 1: a += "CL."; CL(); break;
    case 2: a += "CR.CR."; CR(); CR(); break;
    case 3: a += "CR."; CR(); break;
    }
    //if (iter > 1) warnval |= (0x1 << (step - 1));
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 2: bottom centers
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int rot = 0;
    while (!erval && findcenter_not_d(6))
    {
      if      (cube[2][0][2] != 6) ;
      else if (cube[2][0][3] != 6)
      { a += "CR."; CR(); rot=(rot+1)%4; }
      else if (cube[3][0][2] != 6)
      { a += "CL."; CL(); rot=(rot+3)%4; }
      else if (cube[3][0][3] != 6)
      { a += "CL.CL."; CL(); CL(); rot=(rot+2)%4; }
      switch (findcenter_not_d(6))
      {
      case 0:
        break;
      case NEGX:
        if      (fy == 2 && fz == 2)
        { a += "LD."; LD(); }
        else if (fy == 2 && fz == 3) ;
        else if (fy == 3 && fz == 2)
        { a += "LU.LU."; LU(); LU(); }
        else if (fy == 3 && fz == 3)
        { a += "LU."; LU(); }
        a += "fC.LU.fA.";
        fC(); LU(); fA();
        break;
      case NEGZ:
        if      (fx == 2 && fy == 2)
        { a += "FA."; FA(); }
        else if (fx == 2 && fy == 3)
        { a += "FC.FC."; FC(); FC(); }
        else if (fx == 3 && fy == 2) ;
        else if (fx == 3 && fy == 3)
        { a += "FC."; FC(); }
        a += "lU.FC.lD.";
        lU(); FC(); lD();
        break;
      case POSX:
        if      (fy == 2 && fz == 2)
        { a += "RU.RU."; RU(); RU(); }
        else if (fy == 2 && fz == 3)
        { a += "RD."; RD(); }
        else if (fy == 3 && fz == 2)
        { a += "RU."; RU(); }
        else if (fy == 3 && fz == 3) ;
        a += "fA.RD.fC.";
        fA(); RD(); fC();
        break;
      case POSZ:
        if      (fx == 2 && fy == 2)
        { a += "BC.BC."; BC(); BC(); }
        else if (fx == 2 && fy == 3)
        { a += "BC."; BC(); }
        else if (fx == 3 && fy == 2)
        { a += "BA."; BA(); }
        else if (fx == 3 && fy == 3) ;
        a += "lD.BA.lU.";
        lD(); BA(); lU();
        break;
      }
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    switch (rot)
    {
    case 0: break;
    case 1: a += "CL."; CL(); break;
    case 2: a += "CR.CR."; CR(); CR(); break;
    case 3: a += "CR."; CR(); break;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 3: top corners
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    while
    (!erval &&
     !((findcorner_c(1,3,2) == POSY && fx == 1 && fz == 1) &&
       (findcorner_c(1,4,3) == POSY && fx == 4 && fz == 1) &&
       (findcorner_c(1,5,4) == POSY && fx == 4 && fz == 4) &&
       (findcorner_c(1,2,5) == POSY && fx == 1 && fz == 4) ) )
    {
      for (int i = 1; i <= 4; i++)
      {
        switch (findcorner_c(1, (i<4?i+2:i-2), (i+1)))
        {
        case POSY:
          if      (fx == 1 && fz == 1) ;
          else if (fx == 1 && fz == 4) {
            a += "BA.DR.BC.FA.DL.FC.";
            BA(); DR(); BC(); FA(); DL(); FC();
          }
          else if (fx == 4 && fz == 1) {
            a += "RD.DL.RU.LD.DR.LU.";
            RD(); DL(); RU(); LD(); DR(); LU();
          }
          else if (fx == 4 && fz == 4) {
            a += "BC.DL.BA.DL.LD.DR.LU.";
            BC(); DL(); BA(); DL(); LD(); DR(); LU();
          }
          break;
        case NEGY:
          if      (fx == 1 && fz == 1) ;
          else if (fx == 1 && fz == 4) {
            a += "DR.";
            DR();
          }
          else if (fx == 4 && fz == 1) {
            a += "DL.";
            DL();
          }
          else if (fx == 4 && fz == 4) {
            a += "DL.DL.";
            DL(); DL();
          }
          a += "FA.DR.FC.DL.LD.DL.LU.";
          FA(); DR(); FC(); DL(); LD(); DL(); LU();
          break;
        case NEGX:
          if      (fy == 1 && fz == 1) {
            a += "LD.DR.LU.";
            LD(); DR(); LU();
          }
          else if (fy == 1 && fz == 4) {
            a += "DR.FA.DL.FC.";
            DR(); FA(); DL(); FC();
          }
          else if (fy == 4 && fz == 1) {
            a += "LD.DR.LU.DL.LD.DR.LU.";
            LD(); DR(); LU(); DL(); LD(); DR(); LU();
          }
          else if (fy == 4 && fz == 4) {
            a += "LU.DL.LD.DL.LD.DL.LU.";
            LU(); DL(); LD(); DL(); LD(); DL(); LU();
          }
          break;
        case NEGZ:
          if      (fx == 1 && fy == 1) {
            a += "FA.DL.FC.";
            FA(); DL(); FC();
          }
          else if (fx == 1 && fy == 4) {
            a += "FA.DL.FC.DR.FA.DL.FC.";
            FA(); DL(); FC(); DR(); FA(); DL(); FC();
          }
          else if (fx == 4 && fy == 1) {
            a += "DL.LD.DR.LU.";
            DL(); LD(); DR(); LU();
          }
          else if (fx == 4 && fy == 4) {
            a += "FC.DR.FA.DR.FA.DR.FC.";
            FC(); DR(); FA(); DR(); FA(); DR(); FC();
          }
          break;
        case POSX:
          if      (fy == 1 && fz == 1) {
            a += "LD.DL.LU.";
            LD(); DL(); LU();
          }
          else if (fy == 1 && fz == 4) {
            a += "DR.FA.DR.FC.";
            DR(); FA(); DR(); FC();
          }
          else if (fy == 4 && fz == 1) {
            a += "RD.LD.DL.RU.LU.";
            RD(); LD(); DL(); RU(); LU();
          }
          else if (fy == 4 && fz == 4) {
            a += "RU.DR.RD.FA.DR.FC.";
            RU(); DR(); RD(); FA(); DR(); FC();
          }
          break;
        case POSZ:
          if      (fx == 1 && fy == 1) {
            a += "FA.DR.FC.";
            FA(); DR(); FC();
          }
          else if (fx == 1 && fy == 4) {
            a += "BA.FA.DR.BC.FC.";
            BA(); FA(); DR(); BC(); FC();
          }
          else if (fx == 4 && fy == 1) {
            a += "DL.LD.DL.LU.";
            DL(); LD(); DL(); LU();
          }
          else if (fx == 4 && fy == 4) {
            a += "BC.DL.BA.LD.DL.LU.";
            BC(); DL(); BA(); LD(); DL(); LU();
          }
          break;
        }
        a += "CL."; CL();
      }
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 4: bottom corners positioning
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int good, corn[4];
    good = 0; for (int i = 0; i < 4; i++) corn[i] = 0;
    findcorner_c(6,2,3); if (fx <= 1 && fz <= 1) { corn[0]=1; good++; }
    findcorner_c(6,3,4); if (fx >= 4 && fz <= 1) { corn[1]=1; good++; }
    findcorner_c(6,4,5); if (fx >= 4 && fz >= 4) { corn[2]=1; good++; }
    findcorner_c(6,5,2); if (fx <= 1 && fz >= 4) { corn[3]=1; good++; }
    for (int i = 1; i <= 4 && good < 2; i++)
    {
      a += "DR."; DR();
      good = 0; for (int j = 0; j < 4; j++) corn[j] = 0;
      findcorner_c(6,2,3); if (fx <= 1 && fz <= 1) { corn[0]=1; good++; }
      findcorner_c(6,3,4); if (fx >= 4 && fz <= 1) { corn[1]=1; good++; }
      findcorner_c(6,4,5); if (fx >= 4 && fz >= 4) { corn[2]=1; good++; }
      findcorner_c(6,5,2); if (fx <= 1 && fz >= 4) { corn[3]=1; good++; }
    }
    //if (good < 2) erval = ERR_NONDESCRIPT;
    while (!erval && good < 4)
    {
      for (int i = 0; i < 4; i++)
      {
        if (corn[i] && corn[i<3?i+1:i-3])
        {
          switch (i)
          {
          case 0: a += "CR.CR."; CR(); CR(); break;
          case 1: a += "CR."; CR(); break;
          case 2: break;
          case 3: a += "CL."; CL(); break;
          }
          a += "RD.DL.RU.FC.DR.FA.RD.DR.RU.DR.DR.";
          RD(); DL(); RU(); FC(); DR(); FA(); RD(); DR(); RU(); DR(); DR();
          switch (i)
          {
          case 0: a += "CL.CL."; CL(); CL(); break;
          case 1: a += "CL."; CL(); break;
          case 2: break;
          case 3: a += "CR."; CR(); break;
          }
          break;
        }
        else if (corn[i] && corn[i<2?i+2:i-2])
        {
          switch (i)
          {
          case 0: break;
          case 1: a += "CL."; CL(); break;
          case 2: break;
          case 3: a += "CR."; CR(); break;
          }
          a += "RD.DL.RU.FC.DR.FA.RD.DR.RU.DR.CL."
               "RD.DL.RU.FC.DR.FA.RD.DR.RU.DR.DR.CR.";
          RD(); DL(); RU(); FC(); DR(); FA(); RD(); DR(); RU(); DR(); CL();
          RD(); DL(); RU(); FC(); DR(); FA(); RD(); DR(); RU(); DR(); DR(); CR();
          switch (i)
          {
          case 0: break;
          case 1: a += "CR."; CR(); break;
          case 2: break;
          case 3: a += "CL."; CL(); break;
          }
          break;
        }
      }
      good = 0; for (int i = 0; i < 4; i++) corn[i] = 0;
      findcorner_c(6,2,3); if (fx <= 1 && fz <= 1) { corn[0]=1; good++; }
      findcorner_c(6,3,4); if (fx >= 4 && fz <= 1) { corn[1]=1; good++; }
      findcorner_c(6,4,5); if (fx >= 4 && fz >= 4) { corn[2]=1; good++; }
      findcorner_c(6,5,2); if (fx <= 1 && fz >= 4) { corn[3]=1; good++; }
      for (int i = 1; i <= 4 && good < 2; i++)
      {
        a += "DR."; DR();
        good = 0; for (int j = 0; j < 4; j++) corn[j] = 0;
        findcorner_c(6,2,3); if (fx <= 1 && fz <= 1) { corn[0]=1; good++; }
        findcorner_c(6,3,4); if (fx >= 4 && fz <= 1) { corn[1]=1; good++; }
        findcorner_c(6,4,5); if (fx >= 4 && fz >= 4) { corn[2]=1; good++; }
        findcorner_c(6,5,2); if (fx <= 1 && fz >= 4) { corn[3]=1; good++; }
      }
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 5: bottom corners orienting
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int good, corn[4];
    good = 0;
    if ((corn[0] = findcorner_c(6,2,3)) == NEGY) good++;
    if ((corn[1] = findcorner_c(6,3,4)) == NEGY) good++;
    if ((corn[2] = findcorner_c(6,4,5)) == NEGY) good++;
    if ((corn[3] = findcorner_c(6,5,2)) == NEGY) good++;
    while (!erval && good < 4)
    {
      if      (good == 3)
      {
        erval = ERR_PARITY_CORNER_ROTATION;
        break;
      }
      switch (good)
      {
      case 0:
        {
        int f = 0;
        for (int i = 0; i < 4; i++)
        {
          if (corn[i] == corn[i>0?i-1:i+3] || corn[i] == -(corn[i>0?i-1:i+3]))
          {
            f = 1;
            switch (i)
            {
            case 0: a += "CR.CR."; CR(); CR(); break;
            case 1: a += "CR."; CR(); break;
            case 2: break;
            case 3: a += "CL."; CL(); break;
            }
            if (cube[5][1][1] == 6)
            {
              a += "RD.DL.DL.RU.DR.RD.DR.RU."
                   "LD.DR.DR.LU.DL.LD.DL.LU.";
              RD(); DL(); DL(); RU(); DR(); RD(); DR(); RU();
              LD(); DR(); DR(); LU(); DL(); LD(); DL(); LU();
           }
            else // if (cube[4][1][0] == 6)
            {
              a += "LD.DR.LU.DR.LD.DL.DL.LU."
                   "RD.DL.RU.DL.RD.DR.DR.RU.";
              LD(); DR(); LU(); DR(); LD(); DL(); DL(); LU();
              RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU();
            }
            switch (i)
            {
            case 0: a += "CL.CL."; CL(); CL(); break;
            case 1: a += "CL."; CL(); break;
            case 2: break;
            case 3: a += "CR."; CR(); break;
            }
            break;
          }
        }
        if (!f)
        {
          erval = ERR_PARITY_CORNER_ROTATION;
          /*
          // the slightly lazy way.....
          a += "RD.DL.RU.DL.RD.DR.DR.RU.DR.DR.";
          RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU(); DR(); DR();
          */
        }
        } // ya i know... just so we could squeeze int f inside a case.
        break;
      case 1:
        for (int i = 0; i < 4; i++)
        {
          if (corn[i] == NEGY)
          {
            switch (i)
            {
            case 0: break;
            case 1: a += "CL."; CL(); break;
            case 2: a += "CR.CR."; CR(); CR(); break;
            case 3: a += "CR."; CR(); break;
            }
            if (cube[4][1][0] == 6)
            {
              a += "RD.DL.RU.DL.RD.DR.DR.RU.DR.DR.";
              RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU(); DR(); DR();
            }
            else // if (cube[5][1][1] == 6)
            {
              a += "DL.DL.RD.DL.DL.RU.DR.RD.DR.RU.";
              DL(); DL(); RD(); DL(); DL(); RU(); DR(); RD(); DR(); RU();
            }
            switch (i)
            {
            case 0: break;
            case 1: a += "CR."; CR(); break;
            case 2: a += "CL.CL."; CL(); CL(); break;
            case 3: a += "CL."; CL(); break;
            }
            break;
          }
        }
        break;
      case 2:
        for (int i = 0; i < 4; i++)
        {
          if (corn[i] == NEGY && corn[i>0?i-1:i+3] == NEGY)
          {
            switch (i)
            {
            case 0: break;
            case 1: a += "CL."; CL(); break;
            case 2: a += "CR.CR."; CR(); CR(); break;
            case 3: a += "CR."; CR(); break;
            }
            if (cube[5][1][1] == 6)
            {
              a += "RD.DL.DL.RU.DR.RD.DR.RU."
                   "LD.DR.DR.LU.DL.LD.DL.LU.";
              RD(); DL(); DL(); RU(); DR(); RD(); DR(); RU();
              LD(); DR(); DR(); LU(); DL(); LD(); DL(); LU();
            }
            else // if (cube[4][1][0] == 6)
            {
              a += "LD.DR.LU.DR.LD.DL.DL.LU."
                   "RD.DL.RU.DL.RD.DR.DR.RU.";
              LD(); DR(); LU(); DR(); LD(); DL(); DL(); LU();
              RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU();
            }
            switch (i)
            {
            case 0: break;
            case 1: a += "CR."; CR(); break;
            case 2: a += "CL.CL."; CL(); CL(); break;
            case 3: a += "CL."; CL(); break;
            }
            break;
          }
          else if (corn[i] == NEGY && corn[i<2?i+2:i-2] == NEGY)
          {
            switch (i)
            {
            case 0: break;
            case 1: a += "CL."; CL(); break;
            case 2: break;
            case 3: a += "CR."; CR(); break;
            }
            if (cube[4][1][0] == 6)
            {
              a += "CL.LD.DR.LU.DR.LD.DL.DL.LU."
                   "RD.DL.RU.DL.RD.DR.DR.RU."
                   "CR.LD.DR.LU.DR.LD.DL.DL.LU."
                   "RD.DL.RU.DL.RD.DR.DR.RU.";
              CL(); LD(); DR(); LU(); DR(); LD(); DL(); DL(); LU();
              RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU();
              CR(); LD(); DR(); LU(); DR(); LD(); DL(); DL(); LU();
              RD(); DL(); RU(); DL(); RD(); DR(); DR(); RU();
            }
            else // if (cube[5][1][1] == 6)
            {
              a += "RD.DL.DL.RU.DR.RD.DR.RU."
                   "LD.DR.DR.LU.DL.LD.DL.LU.CL."
                   "RD.DL.DL.RU.DR.RD.DR.RU."
                   "LD.DR.DR.LU.DL.LD.DL.LU.CR.";
              RD(); DL(); DL(); RU(); DR(); RD(); DR(); RU();
              LD(); DR(); DR(); LU(); DL(); LD(); DL(); LU(); CL();
              RD(); DL(); DL(); RU(); DR(); RD(); DR(); RU();
              LD(); DR(); DR(); LU(); DL(); LD(); DL(); LU(); CR();
            }
            switch (i)
            {
            case 0: break;
            case 1: a += "CR."; CR(); break;
            case 2: break;
            case 3: a += "CL."; CL(); break;
            }
            break;
          }
        }
        break;
      }
      good = 0;
      if ((corn[0] = findcorner_c(6,2,3)) == NEGY) good++;
      if ((corn[1] = findcorner_c(6,3,4)) == NEGY) good++;
      if ((corn[2] = findcorner_c(6,4,5)) == NEGY) good++;
      if ((corn[3] = findcorner_c(6,5,2)) == NEGY) good++;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 6: 3/4 top edge pairs
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int fnd;
    int good;
    good = 0;
    if
    (cube[4][5][2] == 1 && cube[5][4][2] == 4 &&
     cube[4][5][3] == 1 && cube[5][4][3] == 4 &&
     cube[2][5][4] == 1 && cube[2][4][5] == 5 &&
     cube[3][5][4] == 1 && cube[3][4][5] == 5 &&
     cube[1][5][2] == 1 && cube[0][4][2] == 2 &&
     cube[1][5][3] == 1 && cube[0][4][3] == 2 )
      good = 1;
    while (!erval && good < 1)
    {
      a += "CL."; CL();
      for (int i = 1; i <= 3; i++)
      {
        fnd = findedge_l(1,(i<3?i+3:i-1));
        switch (fnd)
        {
        case POSY:
          if      (fx == 1) {
            a += "LD.dR.LU.dR.FC.dL.FA.";
            LD(); dR(); LU(); dR(); FC(); dL(); FA();
          }
          else if (fz == 1) ;
          else if (fx == 4) {
            a += "RD.uL.RU.uL.FA.uR.FC.";
            RD(); uL(); RU(); uL(); FA(); uR(); FC();
          }
          else if (fz == 4) {
            a += "BA.dL.dL.BC.dR.FC.dL.FA.";
            BA(); dL(); dL(); BC(); dR(); FC(); dL(); FA();
          }
          break;
        case NEGY:
          if      (fx == 1) {
            a += "DR.DR.RU.uL.RD.uL.FA.uR.FC.DL.DL.";
            DR(); DR(); RU(); uL(); RD(); uL(); FA(); uR(); FC(); DL(); DL();
          }
          else if (fz == 1) {
            a += "DR.RU.uL.RD.uL.FA.uR.FC.DL.";
            DR(); RU(); uL(); RD(); uL(); FA(); uR(); FC(); DL();
          }
          else if (fx == 4) {
            a += "RU.uL.RD.uL.FA.uR.FC.";
            RU(); uL(); RD(); uL(); FA(); uR(); FC();
          }
          else if (fz == 4) {
            a += "DL.RU.uL.RD.uL.FA.uR.FC.DR.";
            DL(); RU(); uL(); RD(); uL(); FA(); uR(); FC(); DR();
          }
          break;
        case NEGX:
          if      (fz == 1) {
            a += "uL.FA.uR.FC.";
            uL(); FA(); uR(); FC();
          }
          else if (fz == 4) {
            a += "dL.FC.dL.FA.";
            dL(); FC(); dL(); FA();
          }
          else if (fy == 4) {
            a += "LU.dL.LD.FC.dL.FA.";
            LU(); dL(); LD(); FC(); dL(); FA();
          }
          else if (fy == 1) {
            a += "DR.DR.RD.uR.RU.FA.uR.FC.DL.DL.";
            DR(); DR(); RD(); uR(); RU(); FA(); uR(); FC(); DL(); DL();
          }
          break;
        case NEGZ:
          if      (fx == 1) {
            a += "dR.dR.FC.dL.FA.";
            dR(); dR(); FC(); dL(); FA();
          }
          else if (fx == 4) {
            a += "uL.uL.FA.uR.FC.";
            uL(); uL(); FA(); uR(); FC();
          }
          else if (fy == 4) {
            a += "FC.uR.FA.uR.FA.uR.FC.";
            FC(); uR(); FA(); uR(); FA(); uR(); FC();
          }
          else if (fy == 1) {
            a += "FA.uR.FC.uR.FA.uR.FC.";
            FA(); uR(); FC(); uR(); FA(); uR(); FC();
          }
          break;
        case POSX:
          if      (fz == 1) {
            a += "dR.FC.dL.FA.";
            dR(); FC(); dL(); FA();
          }
          else if (fz == 4) {
            a += "uR.FA.uR.FC.";
            uR(); FA(); uR(); FC();
          }
          else if (fy == 4) {
            a += "RU.uR.RD.FA.uR.FC.";
            RU(); uR(); RD(); FA(); uR(); FC();
          }
          else if (fy == 1) {
            a += "RD.uR.RU.FA.uR.FC.";
            RD(); uR(); RU(); FA(); uR(); FC();
          }
          break;
        case POSZ:
          if      (fx == 1) {
            a += "FA.uR.FC.";
            FA(); uR(); FC();
          }
          else if (fx == 4) {
            a += "FC.dL.FA.";
            FC(); dL(); FA();
          }
          else if (fy == 4) {
            a += "BC.dL.BA.dR.FC.dL.FA.";
            BC(); dL(); BA(); dR(); FC(); dL(); FA();
          }
          else if (fy == 1) {
            a += "DL.RD.uR.RU.FA.uR.FC.DR.";
            DL(); RD(); uR(); RU(); FA(); uR(); FC(); DR();
          }
          break;
        }
        fnd = findedge_r(1,(i<3?i+3:i-1));
        switch (fnd)
        {
        case POSY:
          if      (fx == 1) {
            a += "LD.uR.LU.uR.FC.uL.FA.";
            LD(); uR(); LU(); uR(); FC(); uL(); FA();
          }
          else if (fz == 1) ;
          else if (fx == 4) {
            a += "RD.dL.RU.dL.FA.dR.FC.";
            RD(); dL(); RU(); dL(); FA(); dR(); FC();
          }
          else if (fz == 4) {
            a += "BC.dR.dR.BA.dL.FA.dR.FC.";
            BC(); dR(); dR(); BA(); dL(); FA(); dR(); FC();
          }
          break;
        case NEGY:
          if      (fx == 1) {
            a += "DR.DR.RU.dL.RD.dL.FA.dR.FC.DL.DL.";
            DR(); DR(); RU(); dL(); RD(); dL(); FA(); dR(); FC(); DL(); DL();
          }
          else if (fz == 1) {
            a += "DR.RU.dL.RD.dL.FA.dR.FC.DL.";
            DR(); RU(); dL(); RD(); dL(); FA(); dR(); FC(); DL();
          }
          else if (fx == 4) {
            a += "RU.dL.RD.dL.FA.dR.FC.";
            RU(); dL(); RD(); dL(); FA(); dR(); FC();
          }
          else if (fz == 4) {
            a += "DL.RU.dL.RD.dL.FA.dR.FC.DR.";
            DL(); RU(); dL(); RD(); dL(); FA(); dR(); FC(); DR();
          }
          break;
        case NEGX:
          if      (fz == 1) {
            a += "dL.FA.dR.FC.";
            dL(); FA(); dR(); FC();
          }
          else if (fz == 4) {
            a += "uL.FC.uL.FA.";
            uL(); FC(); uL(); FA();
          }
          else if (fy == 4) {
            a += "LU.uL.LD.FC.uL.FA.";
            LU(); uL(); LD(); FC(); uL(); FA();
          }
          else if (fy == 1) {
            a += "DR.DR.RD.dR.RU.FA.dR.FC.DL.DL.";
            DR(); DR(); RD(); dR(); RU(); FA(); dR(); FC(); DL(); DL();
          }
          break;
        case NEGZ:
          if      (fx == 1) {
            a += "uR.uR.FC.uL.FA.";
            uR(); uR(); FC(); uL(); FA();
          }
          else if (fx == 4) {
            a += "dL.dL.FA.dR.FC.";
            dL(); dL(); FA(); dR(); FC();
          }
          else if (fy == 4) {
            a += "FA.uL.FC.uL.FC.uL.FA.";
            FA(); uL(); FC(); uL(); FC(); uL(); FA();
          }
          else if (fy == 1) {
            a += "FC.uL.FA.uL.FC.uL.FA.";
            FC(); uL(); FA(); uL(); FC(); uL(); FA();
          }
          break;
        case POSX:
          if      (fz == 1) {
            a += "uR.FC.uL.FA.";
            uR(); FC(); uL(); FA();
          }
          else if (fz == 4) {
            a += "dR.FA.dR.FC.";
            dR(); FA(); dR(); FC();
          }
          else if (fy == 4) {
            a += "RU.dR.RD.FA.dR.FC.";
            RU(); dR(); RD(); FA(); dR(); FC();
          }
          else if (fy == 1) {
            a += "RD.dR.RU.FA.dR.FC.";
            RD(); dR(); RU(); FA(); dR(); FC();
          }
          break;
        case POSZ:
          if      (fx == 1) {
            a += "FA.dR.FC.";
            FA(); dR(); FC();
          }
          else if (fx == 4) {
            a += "FC.uL.FA.";
            FC(); uL(); FA();
          }
          else if (fy == 4) {
            a += "BA.dR.BC.dL.FA.dR.FC.";
            BA(); dR(); BC(); dL(); FA(); dR(); FC();
          }
          else if (fy == 1) {
            a += "DL.RD.dR.RU.FA.dR.FC.DR.";
            DL(); RD(); dR(); RU(); FA(); dR(); FC(); DR();
          }
          break;
        }
        a += "CL."; CL();
      }
      if
      (cube[4][5][2] == 1 && cube[5][4][2] == 4 &&
       cube[4][5][3] == 1 && cube[5][4][3] == 4 &&
       cube[2][5][4] == 1 && cube[2][4][5] == 5 &&
       cube[3][5][4] == 1 && cube[3][4][5] == 5 &&
       cube[1][5][2] == 1 && cube[0][4][2] == 2 &&
       cube[1][5][3] == 1 && cube[0][4][3] == 2 )
        good = 1;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 7: bottom edges
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int fnd;
    int good;
    good = 0;
    if
    (cube[1][0][2] == 6 && cube[0][1][2] == 2 &&
     cube[1][0][3] == 6 && cube[0][1][3] == 2 &&
     cube[2][0][1] == 6 && cube[2][1][0] == 3 &&
     cube[3][0][1] == 6 && cube[2][1][0] == 3 &&
     cube[4][0][2] == 6 && cube[5][1][2] == 4 &&
     cube[4][0][3] == 6 && cube[5][1][3] == 4 &&
     cube[2][0][4] == 6 && cube[2][1][5] == 5 &&
     cube[3][0][4] == 6 && cube[3][1][5] == 5 )
      good = 1;
    while (!erval && good < 1)
    {
      for (int i = 1; i <= 4; i++)
      {
        fnd = findedge_l(6,(i<4?i+2:i-2));
        switch (fnd)
        {
        case POSY:
          a += "FA.uL.FC.FC.uR.FA.";
          FA(); uL(); FC(); FC(); uR(); FA();
          break;
        case NEGY:
          if      (fx == 1) {
            a += "DR.FA.dR.FC.DL.FA.dL.FC.";
            DR(); FA(); dR(); FC(); DL(); FA(); dL(); FC();
          }
          else if (fz == 1) ;
          else if (fx == 4) {
            a += "DL.FA.dR.FC.DR.FA.dL.FC.";
            DL(); FA(); dR(); FC(); DR(); FA(); dL(); FC();
          }
          else if (fz == 4) {
            a += "DR.DR.FA.dR.FC.DL.DL.FA.dL.FC.";
            DR(); DR(); FA(); dR(); FC(); DL(); DL(); FA(); dL(); FC();
          }
          break;
        case NEGX:
          if      (fz == 1) {
            a += "uL.FC.uR.FA.";
            uL(); FC(); uR(); FA();
          }
          else if (fz == 4) {
            a += "dL.FA.dL.FC.";
            dL(); FA(); dL(); FC();
          }
          else if (fy == 1) {
            a += "DR.FC.dL.FA.DL.dL.FA.dL.FC.";
            DR(); FC(); dL(); FA(); DL(); dL(); FA(); dL(); FC();
          }
          break;
        case NEGZ:
          if      (fx == 1) {
            a += "dR.dR.FA.dL.FC.";
            dR(); dR(); FA(); dL(); FC();
          }
          else if (fx == 4) {
            a += "uL.uL.FC.uR.FA.";
            uL(); uL(); FC(); uR(); FA();
          }
          else if (fy == 4) {
            a += "FA.dL.FC.dL.FA.dL.FC.";
            FA(); dL(); FC(); dL(); FA(); dL(); FC();
          }
          else if (fy == 1) {
            a += "FC.dL.FA.dL.FA.dL.FC.";
            FC(); dL(); FA(); dL(); FA(); dL(); FC();
          }
          break;
        case POSX:
          if      (fz == 1) {
            a += "dR.FA.dL.FC.";
            dR(); FA(); dL(); FC();
          }
          else if (fz == 4) {
            a += "uR.FC.uR.FA.";
            uR(); FC(); uR(); FA();
          }
          else if (fy == 1) {
            a += "DL.FC.dL.FA.DR.dL.FA.dL.FC.";
            DL(); FC(); dL(); FA(); DR(); dL(); FA(); dL(); FC();
          }
          break;
        case POSZ:
          if      (fx == 1) {
            a += "FC.uR.FA.";
            FC(); uR(); FA();
          }
          else if (fx == 4) {
            a += "FA.dL.FC.";
            FA(); dL(); FC();
          }
          else if (fy == 1) {
            a += "DL.DL.FC.dL.FA.DR.DR.dL.FA.dL.FC.";
            DL(); DL(); FC(); dL(); FA(); DR(); DR(); dL(); FA(); dL(); FC();
          }
          break;
        }
        fnd = findedge_r(6,(i<4?i+2:i-2));
        switch (fnd)
        {
        case POSY:
          a += "FC.uR.FA.FA.uL.FC.";
          FC(); uR(); FA(); FA(); uL(); FC();
          if (cube[2][4][0] == 6 && cube[2][5][1] == (i<4?i+2:i-2)) {

          }
          break;
        case NEGY:
          if      (fx == 1) {
            a += "DR.FC.dL.FA.DL.FC.dR.FA.";
            DR(); FC(); dL(); FA(); DL(); FC(); dR(); FA();
          }
          else if (fz == 1) ;
          else if (fx == 4) {
            a += "DL.FC.dL.FA.DR.FC.dR.FA.";
            DL(); FC(); dL(); FA(); DR(); FC(); dR(); FA();
          }
          else if (fz == 4) {
            a += "DR.DR.FC.dL.FA.DL.DL.FC.dR.FA.";
            DR(); DR(); FC(); dL(); FA(); DL(); DL(); FC(); dR(); FA();
          }
          break;
        case NEGX:
          if      (fz == 1) {
            a += "dL.FC.dR.FA.";
            dL(); FC(); dR(); FA();
          }
          else if (fz == 4) {
            a += "uL.FA.uL.FC.";
            uL(); FA(); uL(); FC();
          }
          else if (fy == 1) {
            a += "DR.FA.dR.FC.DL.dR.FC.dR.FA.";
            DR(); FA(); dR(); FC(); DL(); dR(); FC(); dR(); FA();
          }
          break;
        case NEGZ:
          if      (fx == 1) {
            a += "uR.uR.FA.uL.FC.";
            uR(); uR(); FA(); uL(); FC();
          }
          else if (fx == 4) {
            a += "dL.dL.FC.dR.FA.";
            dL(); dL(); FC(); dR(); FA();
          }
          else if (fy == 4) {
            a += "FC.dR.FA.dR.FC.dR.FA.";
            FC(); dR(); FA(); dR(); FC(); dR(); FA();
          }
          else if (fy == 1) {
            a += "FA.dR.FC.dR.FC.dR.FA.";
            FA(); dR(); FC(); dR(); FC(); dR(); FA();
          }
          break;
        case POSX:
          if      (fz == 1) {
            a += "uR.FA.uL.FC.";
            uR(); FA(); uL(); FC();
          }
          else if (fz == 4) {
            a += "dR.FC.dR.FA.";
              dR(); FC(); dR(); FA();
          }
          else if (fy == 1) {
            a += "DL.FA.dR.FC.DR.dR.FC.dR.FA.";
            DL(); FA(); dR(); FC(); DR(); dR(); FC(); dR(); FA();
          }
          break;
        case POSZ:
          if      (fx == 1) {
            a += "FC.dR.FA.";
            FC(); dR(); FA();
          }
          else if (fx == 4) {
            a += "FA.uL.FC.";
            FA(); uL(); FC();
          }
          else if (fy == 1) {
            a += "DL.DL.FA.dR.FC.DR.DR.dR.FC.dR.FA.";
            DL(); DL(); FA(); dR(); FC(); DR(); DR(); dR(); FC(); dR(); FA();
          }
          break;
        }
        a += "CL.UR."; CL(); UR();
      }
      if
      (cube[1][0][2] == 6 && cube[0][1][2] == 2 &&
       cube[1][0][3] == 6 && cube[0][1][3] == 2 &&
       cube[2][0][1] == 6 && cube[2][1][0] == 3 &&
       cube[3][0][1] == 6 && cube[2][1][0] == 3 &&
       cube[4][0][2] == 6 && cube[5][1][2] == 4 &&
       cube[4][0][3] == 6 && cube[5][1][3] == 4 &&
       cube[2][0][4] == 6 && cube[2][1][5] == 5 &&
       cube[3][0][4] == 6 && cube[3][1][5] == 5 )
        good = 1;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 8: keyhole top edge pair
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int l_fnd, r_fnd, lx, ly, lz, rx, ry, rz;
    int good;
    good = 0;
    if
    (cube[2][5][1] == 1 && cube[2][4][0] == 3 &&
     cube[3][5][1] == 1 && cube[3][4][0] == 3 )
      good = 1;
    while (!erval && good < 1)
    {
      l_fnd = findedge_l(1,3);
      lx = fx; ly = fy; lz = fz;
      r_fnd = findedge_r(1,3);
      rx = fx; ry = fy; rz = fz;
      if (ly == 2 && ry == 2)
      {
        for
        (int i = 1; i <= 4 &&
         !((lx <= 1 && lz <= 1 && rx >= 4) ||
           (rx <= 1 && rz <= 1 && lx >= 4) );
         i++ )
        {
          a += "dL."; dL();
          GETEDGE_L(1,3,lx,ly,lz,l_fnd);
          GETEDGE_R(1,3,rx,ry,rz,r_fnd);
        }
        a += "RU.RU.uL.uL.RU.RU.";
        RU(); RU(); uL(); uL(); RU(); RU();
        GETEDGE_L(1,3,lx,ly,lz,l_fnd);
        GETEDGE_R(1,3,rx,ry,rz,r_fnd);
      }
      else if (ly == 3 && ry == 3)
      {
        for
        (int i = 1; i <= 4 &&
         !((lx <= 1 && lz <= 1 && rx >= 4) ||
           (rx <= 1 && rz <= 1 && lx >= 4) );
         i++ )
        {
          a += "uL."; uL();
          GETEDGE_L(1,3,lx,ly,lz,l_fnd);
          GETEDGE_R(1,3,rx,ry,rz,r_fnd);
        }
        a += "RU.RU.dL.dL.RU.RU.";
        RU(); RU(); dL(); dL(); RU(); RU();
        GETEDGE_L(1,3,lx,ly,lz,l_fnd);
        GETEDGE_R(1,3,rx,ry,rz,r_fnd);
      }
      else if (ly >= 4 || ry >= 4)
      {
        if (ly == 2)
          for (int i = 1; i <= 4 && lz <= 1; i++)
          {
            a += "dL."; dL();
            GETEDGE_L(1,3,lx,ly,lz,l_fnd);
          }
        if (ly == 3)
          for (int i = 1; i <= 4 && lz <= 1; i++)
          {
            a += "uL."; uL();
            GETEDGE_L(1,3,lx,ly,lz,l_fnd);
          }
        if (ry == 2)
          for (int i = 1; i <= 4 && rz <= 1; i++)
          {
            a += "dL."; dL();
            GETEDGE_R(1,3,rx,ry,rz,r_fnd);
          }
        if (ry == 3)
          for (int i = 1; i <= 4 && rz <= 1; i++)
          {
            a += "uL."; uL();
            GETEDGE_R(1,3,rx,ry,rz,r_fnd);
          }
        a += "lU.rU.FA.lD.rD.FA.FA.lU.rU.FA.lD.rD.";
        lU(); rU(); FA(); lD(); rD(); FA(); FA(); lU(); rU(); FA(); lD(); rD();
        GETEDGE_L(1,3,lx,ly,lz,l_fnd);
        GETEDGE_R(1,3,rx,ry,rz,r_fnd);
      }
      if (ly == 2 && ry == 3)
      {
        for (int i = 1; i <= 4 && !(lx <= 1 && lz <= 1); i++)
        {
          a += "dL."; dL();
          GETEDGE_L(1,3,lx,ly,lz,l_fnd);
        }
        for (int i = 1; i <= 4 && !(rx <= 1 && rz <= 1); i++)
        {
          a += "uL."; uL();
          GETEDGE_R(1,3,rx,ry,rz,r_fnd);
        }
      }
      else if (ly == 3 && ry == 2)
      {
        for (int i = 1; i <= 4 && !(lx <= 1 && lz <= 1); i++)
        {
          a += "uL."; uL();
          GETEDGE_L(1,3,lx,ly,lz,l_fnd);
        }
        for (int i = 1; i <= 4 && !(rx <= 1 && rz <= 1); i++)
        {
          a += "dL."; dL();
          GETEDGE_R(1,3,rx,ry,rz,r_fnd);
        }
      }
      if
      (((ly == 2 && ry == 3) || (ly == 3 && ry == 2)) &&
       (lx <= 1 && lz <= 1 && lx == rx && lz == rz) )
      {
        if (r_fnd == NEGZ)
        {
          a += "lU.rU.FC.lD.rD.FC.FC.lU.rU.FC.lD.rD.";
          lU(); rU(); FC(); lD(); rD(); FC();
          FC(); lU(); rU(); FC(); lD(); rD();
        }
        else if (r_fnd == NEGX)
        {
          a += "uR.dR.lU.rU.FA.lD.rD.FA.FA.lU.rU.FA.lD.rD.";
          uR(); dR(); lU(); rU(); FA(); lD(); rD();
          FA(); FA(); lU(); rU(); FA(); lD(); rD();
        }
      }
      if
      (cube[2][5][1] == 1 && cube[2][4][0] == 3 &&
       cube[3][5][1] == 1 && cube[3][4][0] == 3 )
        good = 1;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 9: 2nd row edges
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int r_fnd, good;
    good = 0;
    if (cube[0][3][1] == 2 && cube[1][3][0] == 3) good++;
    if (cube[4][3][0] == 3 && cube[5][3][1] == 4) good++;
    if (cube[5][3][4] == 4 && cube[4][3][5] == 5) good++;
    if (cube[1][3][5] == 5 && cube[0][3][4] == 2) good++;
    while (!erval && good < 4)
    {
      r_fnd = findedge_r(4,3);
      if (fy == 2)
      {
        for (int i = 1; i <= 4 && fx < 4; i++)
        {
          a += "dL."; dL();
          r_fnd = findedge_r(4,3);
        }
        a += "RU.RU.uL.uL.RD.RD.";
        RU(); RU(); uL(); uL(); RD(); RD();
        r_fnd = findedge_r(4,3);
      }
      switch (r_fnd)
      {
      case POSX: break;
      case POSZ: a += "uL."; uL(); break;
      case NEGX: a += "uL.uL."; uL(); uL(); break;
      case NEGZ: a += "uR."; uR(); break;
      }
      r_fnd = findedge_r(3,2);
      if (!(fx == 1 && fy == 3 && fz == 0))
      {
        for (int i = 1; i <= 4 && fy == 3; i++)
        {
          a += "BA.BA.dL.dL.BA.BA.";
          BA(); BA(); dL(); dL(); BA(); BA();
          r_fnd = findedge_r(3,2);
        }
        for (int i = 1; i <= 4 && !(fx >= 4 && fz <= 1); i++)
        {
          a += "dL."; dL();
          r_fnd = findedge_r(3,2);
        }
        a += "LU.LU.dL.dL.LU.LU.";
        LU(); LU(); dL(); dL(); LU(); LU();
      }
      r_fnd = findedge_r(2,5);
      if (!(fx == 0 && fy == 3 && fz == 4))
      {
        for (int i = 1; i <= 4 && fy == 3; i++)
        {
          a += "BC.BC.dL.dL.BC.BC.";
          BC(); BC(); dL(); dL(); BC(); BC();
          r_fnd = findedge_r(2,5);
        }
        for (int i = 1; i <= 4 && !(fx <= 1 && fz <= 1); i++)
        {
          a += "dL."; dL();
          r_fnd = findedge_r(2,5);
        }
        a += "BC.BC.dL.dL.BC.BC.";
        BC(); BC(); dL(); dL(); BC(); BC();
      }
      r_fnd = findedge_r(5,4);
      if (!(fx == 4 && fy == 3 && fz == 5))
      {
        for (int i = 1; i <= 4 && !(fx <= 1 && fz >= 4); i++)
        {
          a += "dL."; dL();
          r_fnd = findedge_r(5,4);
        }
        a += "uR.dR.BC.dL.BA.LD.LD.BC.dR.BA.LD.LD.uL.dL.";
        uR(); dR(); BC(); dL(); BA(); LD(); LD();
        BC(); dR(); BA(); LD(); LD(); uL(); dL();
      }
      good = 0;
      if (cube[0][3][1] == 2 && cube[1][3][0] == 3) good++;
      if (cube[4][3][0] == 3 && cube[5][3][1] == 4) good++;
      if (cube[5][3][4] == 4 && cube[4][3][5] == 5) good++;
      if (cube[1][3][5] == 5 && cube[0][3][4] == 2) good++;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 10: 3rd row edges
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int good, ok[4];
    good = 0;
    while (!erval && good < 4)
    {
      switch (findedge_l(4,3))
      {
      case POSX: break;
      case POSZ: a += "dL."; dL(); break;
      case NEGX: a += "dL.dL."; dL(); dL(); break;
      case NEGZ: a += "dR."; dR(); break;
      }
      good = 0;
      for (int i = 0; i < 4; i++) ok[i] = 0;
      if (cube[0][2][1] == 2 && cube[1][2][0] == 3) { ok[0] = 1; good++; }
      if (cube[4][2][0] == 3 && cube[5][2][1] == 4) { ok[1] = 1; good++; }
      if (cube[5][2][4] == 4 && cube[4][2][5] == 5) { ok[2] = 1; good++; }
      if (cube[1][2][5] == 5 && cube[0][2][4] == 2) { ok[3] = 1; good++; }
      if (good == 1)
      {
        a += "FC.FC.RU.RU.FA.dL.FC.RU.RU.FA.dR.FA.";
        FC(); FC(); RU(); RU(); FA(); dL();
        FC(); RU(); RU(); FA(); dR(); FA();
      }
      else if (good == 2)
      {
        if (ok[3] == 1)
        {
          a += "dR.dR.RU.RU.BA.BA.RD.dL.RU.BA.BA.RD.dR.RD.dR."
               "dR.LD.LD.FC.FC.LU.dL.LD.FC.FC.LU.dR.LU.dR.dR."
               "dR.dR.RU.RU.BA.BA.RD.dL.RU.BA.BA.RD.dR.RD.dR.";
          dR(); dR(); RU(); RU(); BA(); BA(); RD(); dL();
          RU(); BA(); BA(); RD(); dR(); RD(); dR();
          dR(); LD(); LD(); FC(); FC(); LU(); dL();
          LD(); FC(); FC(); LU(); dR(); LU(); dR(); dR();
          dR(); dR(); RU(); RU(); BA(); BA(); RD(); dL();
          RU(); BA(); BA(); RD(); dR(); RD(); dR();
        }
        else if (ok[2] == 1)
        {
          a += "dR.dR.RU.RU.BA.BA.RD.dL.RU.BA.BA.RD.dR.RD.dR.";
          dR(); dR(); RU(); RU(); BA(); BA(); RD(); dL();
          RU(); BA(); BA(); RD(); dR(); RD(); dR();
        }
        else if (ok[0] == 1)
        {
          a += "dR.LD.LD.FC.FC.LU.dL.LD.FC.FC.LU.dR.LU.dR.dR.";
          dR(); LD(); LD(); FC(); FC(); LU(); dL();
          LD(); FC(); FC(); LU(); dR(); LU(); dR(); dR();
        }
      }
      good = 0;
      if (cube[0][2][1] == 2 && cube[1][2][0] == 3) good++;
      if (cube[4][2][0] == 3 && cube[5][2][1] == 4) good++;
      if (cube[5][2][4] == 4 && cube[4][2][5] == 5) good++;
      if (cube[1][2][5] == 5 && cube[0][2][4] == 2) good++;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  // step 11: side centers
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    int good, ok[4], col, a_fnd, b_fnd, ax, bx, ay, by, az, bz;
    good = 0;
    good += ok[0] = findcenter_not_l(2) ? 0 : 1;
    good += ok[1] = findcenter_not_f(3) ? 0 : 1;
    good += ok[2] = findcenter_not_r(4) ? 0 : 1;
    good += ok[3] = findcenter_not_b(5) ? 0 : 1;
    while (!erval && good < 4)
    {
      for (int i = 1; i <= 4; i++)
      {
        col = (i<4?i+2:i-2);
        for (int j = 1; j <= 4 && (a_fnd=findcenter_not_f(col)); j++)
        {
          ax = fx; ay = fy; az = fz;
          if (a_fnd == POSZ)
          {
            if ((b_fnd = find_not_center_l(col>2?col-1:col+3)) == 0)
            {
              b_fnd = find_not_center_r(col<5?col+1:col-3);
              if (b_fnd)
              {
                bx = fx; by = fy; bz = fz;
              }
              else
              {
                bx = 5; by = 2; bz = 2;
              }
              if (bz == 2 && by == 2) ;
              else if (bz == 2 && by == 3) { a += "RD."; RD(); }
              else if (bz == 3 && by == 2) { a += "RU."; RU(); }
              else if (bz == 3 && by == 3) { a += "RU.RU."; RU(); RU(); }
              if (ax == 2 && ay == 2) ;
              else if (ax == 2 && ay == 3) { a += "BA."; BA(); }
              else if (ax == 3 && ay == 2) { a += "BC."; BC(); }
              else if (ax == 3 && ay == 3) { a += "BA.BA."; BA(); BA(); }
              a += "BC.BC.lU.fC.lD.BC.BC.lU.fA.lD.";
              BC(); BC(); lU(); fC(); lD(); BC(); BC(); lU(); fA(); lD();
              if (ax == 2 && ay == 2) ;
              else if (ax == 2 && ay == 3) { a += "BC."; BC(); }
              else if (ax == 3 && ay == 2) { a += "BA."; BA(); }
              else if (ax == 3 && ay == 3) { a += "BC.BC."; BC(); BC(); }
              if (bz == 2 && by == 2) ;
              else if (bz == 2 && by == 3) { a += "RU."; RU(); }
              else if (bz == 3 && by == 2) { a += "RD."; RD(); }
              else if (bz == 3 && by == 3) { a += "RD.RD."; RD(); RD(); }
              a_fnd = findcenter_r(col);
              ax = fx; ay = fy; az = fz;
            }
            else
            {
              bx = fx; by = fy; bz = fz;
              if (bz == 2 && by == 3) ;
              else if (bz == 2 && by == 2) { a += "LU."; LU(); }
              else if (bz == 3 && by == 3) { a += "LD."; LD(); }
              else if (bz == 2 && by == 2) { a += "LD.LD."; LD(); LD(); }
              if (ax == 3 && ay == 3) ;
              else if (ax == 3 && ay == 2) { a += "BA."; BA(); }
              else if (ax == 2 && ay == 3) { a += "BC."; BC(); }
              else if (ax == 2 && ay == 2) { a += "BA.BA."; BA(); BA(); }
              a += "BC.BC.rD.fC.rU.BC.BC.rD.fA.rU.";
              BC(); BC(); rD(); fC(); rU(); BC(); BC(); rD(); fA(); rU();
              if (ax == 3 && ay == 3) ;
              else if (ax == 3 && ay == 2) { a += "BC."; BC(); }
              else if (ax == 2 && ay == 3) { a += "BA."; BA(); }
              else if (ax == 2 && ay == 2) { a += "BC.BC."; BC(); BC(); }
              if (bz == 2 && by == 3) ;
              else if (bz == 2 && by == 2) { a += "LD."; LD(); }
              else if (bz == 3 && by == 3) { a += "LU."; LU(); }
              else if (bz == 2 && by == 2) { a += "LU.LU."; LU(); LU(); }
              a_fnd = findcenter_l(col);
              ax = fx; ay = fy; az = fz;
            }
          }
          if (a_fnd == NEGX)
          {
            b_fnd = find_not_center_f(col);
            bx = fx; by = fy; bz = fz;
            if (bx == 3 && by == 3) ;
            else if (bx == 3 && by == 2) { a += "FA."; FA(); }
            else if (bx == 2 && by == 3) { a += "FC."; FC(); }
            else if (bx == 2 && by == 2) { a += "FC.FC."; FC(); FC(); }
            if (az == 3 && ay == 3) ;
            else if (az == 3 && ay == 2) { a += "LD."; LD(); }
            else if (az == 2 && ay == 3) { a += "LU."; LU(); }
            else if (az == 2 && ay == 2) { a += "LD.LD."; LD(); LD(); }
            a += "LU.LU.bC.rU.bA.LU.LU.bC.rD.bA.";
            LU(); LU(); bC(); rU(); bA(); LU(); LU(); bC(); rD(); bA();
            if (az == 3 && ay == 3) ;
            else if (az == 3 && ay == 2) { a += "LU."; LU(); }
            else if (az == 2 && ay == 3) { a += "LD."; LD(); }
            else if (az == 2 && ay == 2) { a += "LU.LU."; LU(); LU(); }
            if (bx == 3 && by == 3) ;
            else if (bx == 3 && by == 2) { a += "FC."; FC(); }
            else if (bx == 2 && by == 3) { a += "FA."; FA(); }
            else if (bx == 2 && by == 2) { a += "FA.FA."; FA(); FA(); }
          }
          else if (a_fnd == POSX)
          {
            b_fnd = find_not_center_f(col);
            bx = fx; by = fy; bz = fz;
            if (bx == 2 && by == 2) ;
            else if (bx == 2 && by == 3) { a += "FA."; FA(); }
            else if (bx == 3 && by == 2) { a += "FC."; FC(); }
            else if (bx == 3 && by == 3) { a += "FC.FC."; FC(); FC(); }
            if (az == 3 && ay == 2) ;
            else if (az == 3 && ay == 3) { a += "RU."; RU(); }
            else if (az == 2 && ay == 2) { a += "RD."; RD(); }
            else if (az == 2 && ay == 3) { a += "RU.RU."; RU(); RU(); }
            a += "RD.RD.bC.lD.bA.RD.RD.bC.lU.bA.";
            RD(); RD(); bC(); lD(); bA(); RD(); RD(); bC(); lU(); bA();
            if (az == 3 && ay == 2) ;
            else if (az == 3 && ay == 3) { a += "RD."; RD(); }
            else if (az == 2 && ay == 2) { a += "RU."; RU(); }
            else if (az == 2 && ay == 3) { a += "RD.RD."; RD(); RD(); }
            if (bx == 2 && by == 2) ;
            else if (bx == 2 && by == 3) { a += "FC."; FC(); }
            else if (bx == 3 && by == 2) { a += "FA."; FA(); }
            else if (bx == 3 && by == 3) { a += "FA.FA."; FA(); FA(); }
          }
        }
        a += "CL."; CL();
      }
      good = 0;
      good += ok[0] = findcenter_not_l(2) ? 0 : 1;
      good += ok[1] = findcenter_not_f(3) ? 0 : 1;
      good += ok[2] = findcenter_not_r(4) ? 0 : 1;
      good += ok[3] = findcenter_not_b(5) ? 0 : 1;
      iter++;
      if (iter >= ITER_THRESHOLD) erval = ERR_NONDESCRIPT;
    }
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  /*
  // step X: center rotation
  {
    step++;
    string a = "";
    int iter = 0;
    // fix
    //
    // check for trouble
    if (erval)
      return "";
    if (shorten >= SHORTEN_STRIP_SOME)
      a = concise(a);
    mov[step] = a.length() / 3;
    s += a;
  }
  */
  if (!issolved())
  {
    erval = ERR_NONDESCRIPT;
    return "";
  }
  return s;
}

// end of mcube class definition

//
