/*
 * main.cpp v 1.0
 * 4x4x4 Rubik's Cube Solver console interface (c) 2005 by Eric Dietz
 * notes: readme.txt  email: root@wrongway.org
 */

// includes
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
// namespace
using namespace std;

// local includes
#include "mcube.h"
#include "config.h"

// shared variables
const char *version = "1.0";
const int N = mcube::N;
mcube thecube;
bool randm = false;
bool verbal = false;
int inmode = 0;
char coltochar[7];

// declarations
void myrenderscreen(mcube c);
int chartocol(char c);
void pressenter();
void sendsolution();
int readcommandline(string cmdln);

// display cube on screen
void myrenderscreen(mcube c)
{
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[j][N+1][N+1-i]]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[0][N+1-i][N+1-j]]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[j][N+1-i][0]]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[N+1][N+1-i][j]]);
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[N+1-j][N+1-i][N+1]]);
    printf("\n");
  }
  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) printf("  ");
    printf(" ");
    for (int j = 1; j <= N; j++)
      printf(" %c", coltochar[c.cube[j][0][i]]);
    printf("\n");
  }
}

// return the number associated with a given char
int chartocol(char c)
{
  for (int i = 1; i <= 6; i++)
    if (coltochar[i] == c)
      return i;
  return 0;
}

// make user press enter to continue
void pressenter()
{
    printf("*** press enter to continue ***");
    fflush(stdout);
    fgetc(stdin);
}

// display the solution on the screen
void sendsolution()
{
  string cmd = "", txt;
  int lin = 0;
  for (int i = 1; i <= N * N; i++)
    cmd += "      ";
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      cmd.at(((i-1)*N+(j-1))      ) = coltochar[thecube.cube[j][N+1][N+1-i]    ];
      cmd.at(((i-1)*N+(j-1))+N*N  ) = coltochar[thecube.cube[0][N+1-i][N+1-j]  ];
      cmd.at(((i-1)*N+(j-1))+N*N*2) = coltochar[thecube.cube[j][N+1-i][0]      ];
      cmd.at(((i-1)*N+(j-1))+N*N*3) = coltochar[thecube.cube[N+1][N+1-i][j]    ];
      cmd.at(((i-1)*N+(j-1))+N*N*4) = coltochar[thecube.cube[N+1-j][N+1-i][N+1]];
      cmd.at(((i-1)*N+(j-1))+N*N*5) = coltochar[thecube.cube[j][0][i]          ];
    }
  }
  if (inmode == 1)
  {
    cmd.insert(N*N*5," D:");
    cmd.insert(N*N*4," B:");
    cmd.insert(N*N*3," R:");
    cmd.insert(N*N*2," F:");
    cmd.insert(N*N  ," L:");
    cmd.insert(0    , "U:");
  }
  printf("200 cube solved ok.\n");
  printf("101 version %s (%s)\n", version, mcube::version);
  printf("202 %i moves %i groups", thecube.mov[0], thecube.MOV);
  for (int i = 1; i <= thecube.MOV; i++)
    printf(" %i", thecube.mov[i]);
  printf("\n");
  printf("220 starting diagram:\n");
  //thecube.renderscreen();
  myrenderscreen(thecube);
  printf("221 diagram end.\n");
  printf("210 sending solution:\n");
  if (verbal && thecube.mov[0] > 0)
    pressenter();
  for (int i = 1; i <= thecube.mov[0]; i++)
  {
    if (verbal)
    {
      txt = "";
      switch (thecube.solution.at(i*3-3))
      {
      case 'U': txt += "top"; break;
      case 'u': txt += "inner top"; break;
      case 'd': txt += "inner bottom"; break;
      case 'D': txt += "bottom"; break;
      case 'L': txt += "left"; break;
      case 'l': txt += "inner left"; break;
      case 'r': txt += "inner right"; break;
      case 'R': txt += "right"; break;
      case 'F': txt += "front"; break;
      case 'f': txt += "inner front"; break;
      case 'b': txt += "inner back"; break;
      case 'B': txt += "back"; break;
      }
      txt += " ";
      switch (thecube.solution.at(i*3-2))
      {
      case 'L': txt += "left"; break;
      case 'R': txt += "right"; break;
      case 'U': txt += "up"; break;
      case 'D': txt += "down"; break;
      case 'C': txt += "clockwise"; break;
      case 'A': txt += "anticlockwise"; break;
      }
      printf("%s\n", txt.c_str());
      lin++;
      if (lin >= 24)
      {
        lin = 0;
        pressenter();
      }
    }
    else
    {
      printf("%c%c, ", thecube.solution.at(i*3-3), thecube.solution.at(i*3-2));
    }
  }
  if (!verbal)
    printf("\n");
  if (verbal && lin > 0)
    pressenter();
  printf("211 completed solution.\n");
  thecube.dosolution();
#ifdef SHOWENDINGDIAGRAM
  printf("220 ending diagram:\n");
  //thecube.renderscreen();
  myrenderscreen(thecube);
  printf("221 diagram end.\n");
#endif // SHOWENDINGDIAGRAM
  if (thecube.warnval)
    printf("301 WARNING: %i - possible efficiency bug found.\n", thecube.warnval);
  if (randm)
    printf("203 command-line: mcube %s\n", cmd.c_str());
  printf("201 terminating successfully.\n");
}

// take the command-line and make a cube of it
int readcommandline(string cmdln)
{
  string cmd = cmdln;
  int numused = 0, isused;
#ifdef SENDVERBALSOLUTION
  verbal = true;
#endif // SENDVERBALSOLUTION
  thecube.resetcube();
  for (int i = 1; i <= 6; i++)
    coltochar[i] = '\0';
  if
  (cmd == "random"
  // || cmd == "" // remark on completion
   )
  {
    // 1,2,3,4,5,6 or W,R,B,O,G,Y or something else?
    /*
    for (int i = 1; i <= 6; i++)
      coltochar[i] = i + '0';
    */
    coltochar[1] = 'W'; coltochar[2] = 'R'; coltochar[3] = 'B';
    coltochar[4] = 'O'; coltochar[5] = 'G'; coltochar[6] = 'Y';
    thecube.scramblecube();
    randm = true;
    inmode = 1;
    return 0;
  }
  if ((int)cmd.find(" ") >= 0)
  {
    // NEW routine to deal with optional "U: D: L: R: F: B: " command lines
    int u,d,l,r,f,b,max=0,min=0;
    u = cmd.find("U:"); if (u < 0) u = cmd.find("u:");
    d = cmd.find("D:"); if (d < 0) d = cmd.find("d:");
    l = cmd.find("L:"); if (l < 0) l = cmd.find("l:");
    r = cmd.find("R:"); if (r < 0) r = cmd.find("r:");
    f = cmd.find("F:"); if (f < 0) f = cmd.find("f:");
    b = cmd.find("B:"); if (b < 0) b = cmd.find("b:");
    if (u < min) min = u; if (u > max) max = u;
    if (d < min) min = d; if (d > max) max = d;
    if (l < min) min = l; if (l > max) max = l;
    if (r < min) min = r; if (r > max) max = r;
    if (f < min) min = f; if (f > max) max = f;
    if (b < min) min = b; if (b > max) max = b;
    if (min < 0 || max > (int)cmd.length() - N * N - 2)
      return 1;
    cmd  = cmdln.substr(u+2,N*N);
    cmd += cmdln.substr(l+2,N*N);
    cmd += cmdln.substr(f+2,N*N);
    cmd += cmdln.substr(r+2,N*N);
    cmd += cmdln.substr(b+2,N*N);
    cmd += cmdln.substr(d+2,N*N);
  }
  if (cmd.length() < N * N * 6)
    return 1;
  // NEW routine to allow ANY 6 chars to suffice for the cube!!=)
  for (int i = 0; i < N * N * 6; i++)
  {
    isused = 0;
    for (int j = 1; j <= 6; j++)
      if (coltochar[j] == cmd.at(i))
        isused = 1;
    if (!isused)
    {
      numused++;
      if (numused > 6)
        return 1;
      coltochar[numused] = cmd.at(i);
    }
  }
  if (numused < 6)
    return 1;
  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      thecube.cube[j][N+1][N+1-i]     = chartocol(cmd.at(((i-1)*N+(j-1))      ));
      thecube.cube[0][N+1-i][N+1-j]   = chartocol(cmd.at(((i-1)*N+(j-1))+N*N  ));
      thecube.cube[j][N+1-i][0]       = chartocol(cmd.at(((i-1)*N+(j-1))+N*N*2));
      thecube.cube[N+1][N+1-i][j]     = chartocol(cmd.at(((i-1)*N+(j-1))+N*N*3));
      thecube.cube[N+1-j][N+1-i][N+1] = chartocol(cmd.at(((i-1)*N+(j-1))+N*N*4));
      thecube.cube[j][0][i]           = chartocol(cmd.at(((i-1)*N+(j-1))+N*N*5));
    }
  }
  return 0;
}

// entry point
int main(int argc, char *argv[])
{
  string cmdln = "";
  if      (argc >= 7)
  {
    inmode = 1;
    cmdln = argv[1];
    for (int i = 2; i <= 6; i++)
    {
      cmdln += " ";
      cmdln += argv[i];
    }
  }
  else if (argc >= 2)
  {
    inmode = 0;
    cmdln = argv[1];
  }
  srand(time(NULL)); // UNremark upon completion
  int in = 0, out = 0, retcode = 0;
  in = readcommandline(cmdln);
  if (in == 0)
  {
    //thecube.shorten = mcube::SHORTEN_STRIP_ALL; // remark on completion
    out = thecube.solvecube();
    if (out == 0)
    {
      sendsolution();
    }
    else
    {
      printf("500 ERROR: solver failed for the following reason:\n");
      if      (out == mcube::ERR_NOTINITED)
        printf("511 ERROR: mysterious not-inited error - something is fishy.\n");
      else if (out == mcube::ERR_MISPAINTED)
        printf("512 ERROR: cubelet error - incorrect cubelets - cube mispainted.\n");
      else if (out == mcube::ERR_NONDESCRIPT)
        printf("513 ERROR: nondescript error from findsolution() - cube misassembled.\n");
      else if (out == mcube::ERR_PARITY_EDGE_FLIP)
        printf("514 ERROR: parity error - edge flip - cube misassembled.\n");
      else if (out == mcube::ERR_PARITY_CORNER_ROTATION)
        printf("515 ERROR: parity error - corner rotation - cube misassembled.\n");
      else if (out == mcube::ERR_PARITY_CORNER_BACKWARD)
        printf("516 ERROR: parity error - backward corner - cube mispainted.\n");
      else if (out == mcube::ERR_PARITY_CENTER_SWAP)
        printf("517 ERROR: parity error - center swap - cube misassembled.\n");
      else
        printf("502 ERROR: unexpected error - solvecube() returned %i.\n", out);
      printf("101 version %s (%s)\n", version, mcube::version);
      printf("220 your diagram:\n");
      //thecube.renderscreen();
      myrenderscreen(thecube);
      printf("221 diagram end.\n");
      printf("501 terminating unsuccessfully.\n");
      retcode = 1;
    }
  }
  else
  {
    printf("500 ERROR: solver failed for the following reason:\n");
    printf("510 ERROR: non-protocol input entered.\n");
    printf("101 version %s (%s)\n", version, mcube::version);
    printf(
"\
400 syntax:\n\
mcube <cube-layout>\n\
mcube random\n\
example command-lines:\n\
  mcube U:1111111111111111 L:2222333333332222 F:5555222222225555 R:4444555555554444 B:3333444444443333 D:6666666666666666\n\
  mcube WWWWWWWWWWWWWWWWBBBBOOOOOOOOBBBBRRRRBBBBBBBBRRRRGGGGRRRRRRRRGGGGOOOOGGGGGGGGOOOOYYYYYYYYYYYYYYYY\n\
  mcube random\n\
notes:\n\
  - hint: try using the random parameter to get a feel for how to input cubes.\n\
  - read readme.txt (included with this distribution) for protocol details.\n\
  - use a terminal with scrollback feature to use this program! (or pipe it)\n\
mcube by Eric Dietz - root@wrongway.org\n\
401 end of notes.\n\
"
    );
#ifdef SHOWBADCOMMANDLINE
    if (!(cmdln == "" || cmdln == "help" ))
    {
      printf("unrecognized command-line input:\n");
      if (cmdln.length() <= N * N * 6 * 2)
        printf("%s\n", cmdln.c_str());
    }
#endif // SHOWBADCOMMANDLINE
    printf("501 terminating unsuccessfully.\n");
    retcode = -1;
  }
#ifdef WAITFORENTERTOEND
  printf("press enter to continue...\n");
  fgetc(stdin);
#endif // WAITFORENTERTOEND
  return retcode;
}

//
