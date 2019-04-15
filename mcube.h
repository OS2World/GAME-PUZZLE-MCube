/*
 * mcube.h v 20051216
 * 4x4x4 Rubik's Cube Solver class (c) 2005 by Eric Dietz
 * notes: readme.txt  email: root@wrongway.org
 */

#ifndef _MCUBE_H_
#define _MCUBE_H_

// includes
#include <string>
// namespace
using namespace std;

// defines

// mcube class declaration
class mcube
{
public:
  mcube();                              // constructor
  virtual ~mcube();                     // destructor
  static const char* version;           // version string
  static const int N=4;                 // cube size (NxNxN)
  static const int MOV=11;              // number of move groups
  static int numcubes;                  // number of instantiated cubes
  const bool operator==(const mcube &q);// overloaded == comparison
  const bool operator!=(const mcube &q);// overloaded != comparison
  int *face(int x, int y, int z);       // pointer to cube array
  const void renderscreen();            // display cube diagram
  const bool issolved();                // return solvedness of cube
  const void resetcube();               // reset the cube
  void copytemp(int c[N+2][N+2][N+2], int t[N+2][N+2][N+2]);    // temporary copier function
  const void slice_l(int s);            // rotate given slice left
  const void slice_r(int s);            // rotate given slice right
  const void slice_u(int s);            // rotate given slice up
  const void slice_d(int s);            // rotate given slice down
  const void slice_c(int s);            // rotate given slice clockwise
  const void slice_a(int s);            // rotate given slice counterclockwise
  const void UL();                      // rotate top slice left
  const void UR();                      // rotate top slice right
  const void DL();                      // rotate bottom slice left
  const void DR();                      // rotate bottom slice right
  const void LU();                      // rotate left slice up
  const void LD();                      // rotate left slice down
  const void RU();                      // rotate right slice up
  const void RD();                      // rotate right slice down
  const void FC();                      // rotate front slice clockwise
  const void FA();                      // rotate front slice counterclockwise
  const void BC();                      // rotate rear slice clockwise (from front view)
  const void BA();                      // rotate rear slice counterclockwise (from front view)
  const void uL();                      // rotate inner top slice left
  const void uR();                      // rotate inner top slice right
  const void dL();                      // rotate inner bottom slice left
  const void dR();                      // rotate inner bottom slice right
  const void lU();                      // rotate inner left slice up
  const void lD();                      // rotate inner left slice down
  const void rU();                      // rotate inner right slice up
  const void rD();                      // rotate inner right slice down
  const void fC();                      // rotate inner front slice clockwise
  const void fA();                      // rotate inner front slice counterclockwise
  const void bC();                      // rotate inner rear slice clockwise (from front view)
  const void bA();                      // rotate inner rear slice counterclockwise (from front view)
  const void CL();                      // rotate whole cube left
  const void CR();                      // rotate whole cube right
  const void CU();                      // rotate whole cube up
  const void CD();                      // rotate whole cube down
  const void CC();                      // rotate whole cube clockwise
  const void CA();                      // rotate whole cube counterclockwise
  const void U2();                      // rotate top slice twice
  const void D2();                      // rotate bottom slice twice
  const void L2();                      // rotate left slice twice
  const void R2();                      // rotate right slice twice
  const void F2();                      // rotate front slice twice
  const void B2();                      // rotate rear slice twice
  const void u2();                      // rotate inner top slice twice
  const void d2();                      // rotate inner bottom slice twice
  const void l2();                      // rotate inner left slice twice
  const void r2();                      // rotate inner right slice twice
  const void f2();                      // rotate inner front slice twice
  const void b2();                      // rotate inner rear slice twice
  const void scramblecube();            // scramble the cube
  const void domoves(string s);         // do a series of moves
  const void dosolution();              // execute solution
  const int findcenter_u(int a);        // find given center on top
  const int findcenter_d(int a);        // find given center on bottom
  const int findcenter_l(int a);        // find given center on left
  const int findcenter_r(int a);        // find given center on right
  const int findcenter_f(int a);        // find given center on front
  const int findcenter_b(int a);        // find given center on back
  const int findcenter_not_u(int a);    // find given center not on top
  const int findcenter_not_d(int a);    // find given center not on bottom
  const int findcenter_not_l(int a);    // find given center not on left
  const int findcenter_not_r(int a);    // find given center not on right
  const int findcenter_not_f(int a);    // find given center not on front
  const int findcenter_not_b(int a);    // find given center not on back
  const int findcenter(int a);          // find given center
  const int find_not_center_u(int a);   // find free center on top
  const int find_not_center_d(int a);   // find free center on bottom
  const int find_not_center_l(int a);   // find free center on left
  const int find_not_center_r(int a);   // find free center on right
  const int find_not_center_f(int a);   // find free center on front
  const int find_not_center_b(int a);   // find free center on back
  const int findedge_l(int a, int b);   // find given left edge
  const int findedge_r(int a, int b);   // find given right edge
  const int findedge(int a, int b);     // find given edge
  const int findcorner_c(int a, int b, int c);  // find given corner in clockwise order
  const int findcorner_a(int a, int b, int c);  // find given corner in counterclockwise order
  const int findcorner(int a, int b, int c);    // find given corner
  static const string std_to_metr(string s);    // convert standard string to metric
  static const string metr_to_std(string s);    // convert metric string to standard
  static const string std_to_rel(string s);     // convert standard string to relative
  static const string rel_to_std(string s);     // convert relative string to standard
  static const string metr_to_rel(string s);    // convert metric string to relative
  static const string rel_to_metr(string s);    // convert relative string to metric
  const string usehalfturns(string s, int b);   // allow the use of the half turn
  const string concise(string s, int b);        // remove redundancies from solution
  const void shortenmov(int m);         // shorten mov[]
  const int solvecube();                // solve the cube
  int cube[N+2][N+2][N+2];              // cube array
  int fx, fy, fz;                       // coordinates for find* functions
  int shorten;                          // how short to make the solution
  bool inited;                          // whether the cube is inited
  int mov[MOV+1];                       // number of moves in solution
  int erval;                            // possible error values
  int warnval;                          // possible warning values
  string solution;                      // solution string
  static const int NEGX=-1;             // negative x axis
  static const int POSX=1;              // positive x axis
  static const int NEGY=-2;             // negative y axis
  static const int POSY=2;              // positive y axis
  static const int NEGZ=-3;             // negative z axis
  static const int POSZ=3;              // positive z axis
  static const int ERR_NOTINITED=1;     // cube not inited
  static const int ERR_MISPAINTED=2;    // not all cubelets present
  static const int ERR_NONDESCRIPT=3;   // unknown error
  static const int ERR_PARITY_EDGE_FLIP=4;      // edge flip parity
  static const int ERR_PARITY_CORNER_ROTATION=5;// corner rotation parity
  static const int ERR_PARITY_CORNER_BACKWARD=6;// corner backward parity
  static const int ERR_PARITY_CENTER_SWAP=7;    // center swap parity
  static const int SHORTEN_NONE=0;              // no condensing
  static const int SHORTEN_STRIP_SOME=1;        // strip some
  static const int SHORTEN_STRIP_ALL=2;         // strip all
  static const int SHORTEN_STRIP_ROTATE_SOME=3; // strip and rotate some
  static const int SHORTEN_STRIP_ROTATE_ALL=4;  // strip and rotate all
  static const int ITER_THRESHOLD=10;   // iterative error threshold
protected:
private:
  const string findsolution();          // find a solution for a prepared cube
};

// macros
#define DOMOVES(a, b) domoves(b); a += b        // for those of you who like to do DOMOVES(a, "UL.LU.");
#define GETEDGE_L(a,b,x,y,z,f) f = findedge_l(a, b); x = fx; y = fy; z = fz     // don't ask
#define GETEDGE_R(a,b,x,y,z,f) f = findedge_r(a, b); x = fx; y = fy; z = fz     // yeah
#define GETCORNER_C(a,b,c,x,y,z,f) f = findcorner_c(a, b, c); x = fx; y = fy; z = fz    // more...
#define GETCORNER_A(a,b,c,x,y,z,f) f = findcorner_a(a, b, c); x = fx; y = fy; z = fz    // ...

// end of mcube class declaration

#endif /* _MCUBE_H_ */

//
