#ifndef __EXCON_H__
#define __EXCON_H__

//////////////////////////////////////////////////////////////////////////
// Extra_Console_Ability_Function(ECAF)
//
// ver 1.00 (¿ÀÈÄ 1:20 2011-11-21)
//


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

// Color Ref
#define CR_BLACK 0
#define CR_DBLUE 1
#define	CR_DGREEN 2
#define CR_DCYAN 3
#define CR_DRED 4
#define CR_DPURPLE 5
#define CR_GOLD 6
#define CR_GRAY 7
#define CR_DGRAY 8
#define CR_BLUE 9
#define CR_GREEN 10
#define CR_CYAN 11
#define CR_RED 12
#define CR_PURPLE 13
#define CR_YELLOW 14
#define CR_WHITE 15

// Cursor Type
#define CT_HIDE 0
#define CT_SOLID 1
#define CT_NORMAL 2

void Cprintf(char *String, int FontCR, int BackCR);
void ChangeCR(int FontCR, int BackCR);
void ConClr();
void MoveCur(int x, int y);
int Random_Int(int Min, int Max);
void BipSound(int Pitch, int Duration, boolean UseSound);

#endif
