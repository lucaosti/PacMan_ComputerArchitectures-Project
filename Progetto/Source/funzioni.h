#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include <stdio.h>
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "button_EXINT/button.h"
#include <stdlib.h>
#include <time.h>
#include "core_cm3.h"

void creaGioco(void);
void avanza(void);
void diminuisciTempo(void);
char presenzaMuri(void);
void presenzaPills(void);
void pausa(void);
void vittoria(void);
void gameOver(void);
void disegnaBlocco(int, int, int);
void disegnaPill(int, int);
void disegnaPowerPill(int, int);
void disegnaPacMan(int);
void disegnaCampo(int, int);
void estraiPowerPills(void);
void nuovaVita();
void piazzaPowerPill(int);

