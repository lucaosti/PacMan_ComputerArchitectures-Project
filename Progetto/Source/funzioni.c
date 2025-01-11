#include "strutture.h"
#include "funzioni.h"

// Direzione in cui sta andando il pacman
volatile int direzione = 0; // Aumenta in modo orario con 1 = alto

int punteggio = 0; // Viene incrementato dalle pills
int punteggioVita = 0;
int tempo = 60;		 // Decrementato ogni secondo
int vite = 0;
int inPausa = 0;
extern int seed;

int PPPiazzate = 0;

// Coordinate del pacman
int pacman_X = 13;
int pacman_Y = 23;

// Power Pills
int posizionePP[NUM_PP];
int temporizzazionePP[NUM_PP];

void creaGioco(void)
{
	// Carica scritte
	GUI_Text(0, 0, (uint8_t *) "Game Over in 60", White, Black);
	GUI_Text(SCREEN_WIDTH/2+20, 0, (uint8_t *) "Score 0", White, Black);
	
	LCD_DrawLine(0, SPAZIOSCRITTE, SCREEN_WIDTH, SPAZIOSCRITTE, White);
	
	
	
	disegnaCampo(8, SPAZIOSCRITTE+5);
	
	disegnaPacMan(Yellow);
	
	enable_timer(0);
	enable_timer(1);
	enable_RIT();
	
	// Il gioco parte in pausa
	pausa();
	
	return;
}

void avanza(void)
{
	if(direzione == 0){ // Non deve avanzare
		return;
	}
	
	if(presenzaMuri() == 0){
		direzione = 0;
		return;
	}
	
	// Cancella paman
	disegnaPacMan(Black);

	// Motore di avanzamento
	switch(direzione){
		case 1:
			pacman_Y--;
			break;
		case 2:
			pacman_X++;
			if(pacman_X == 28){
				pacman_X = 0;
			}
			break;
		case 3:
			pacman_Y++;
			break;
		case 4:
			pacman_X--;
			if(pacman_X == -1){
				pacman_X = COLS-1;
			}
			break;
	}
	
	// Disegna pacman
	disegnaPacMan(Yellow);
	
	presenzaPills();
}

void diminuisciTempo(void)
{
	tempo--;
	
	char output[32];
	
  sprintf(output, "Game Over in %d  ", tempo);
  GUI_Text(0, 0, (uint8_t *)output, White, Black);
	
	if(tempo == 0){
		gameOver();
		return;
	}
	
	// Se necessario, piazzo Power Pill
	int i;
	for(i = 0; i < NUM_PP; i++){
		if(tempo == temporizzazionePP[i]){
			PPPiazzate++;
			piazzaPowerPill(i);
			break;
		}
	}
}

char presenzaMuri(void) // Succede prima che il pacman si muova effettivamente in quel punto
{
		switch(direzione){
		case 1:
			if(campo[pacman_Y-1][pacman_X] == 0 || campo[pacman_Y-1][pacman_X] == 4){
				return 0;
			}
			break;
		case 2:
			if(pacman_X == 27){
				return 1;
			}
			if(campo[pacman_Y][pacman_X+1] == 0 || campo[pacman_Y][pacman_X+1] == 4){
				return 0;
			}
			break;
		case 3:
			if(campo[pacman_Y+1][pacman_X] == 0 || campo[pacman_Y+1][pacman_X] == 4){
				return 0;
			}
			break;
		case 4:
			if(pacman_X == 0){
				return 1;
			}
			if(campo[pacman_Y][pacman_X-1] == 0 || campo[pacman_Y][pacman_X-1] == 4){
				return 0;
			}
			break;
	}
	
	return 1;
}

void presenzaPills(void)
{
	if(campo[pacman_Y][pacman_X] == 2){ // Se pills normali
		punteggio += 10;
		punteggioVita += 10;
	}
	else if(campo[pacman_Y][pacman_X] == 3){// Se power pills
		punteggio += 50;
		punteggioVita += 50;
	}
	else {
		return;
	}
	
	campo[pacman_Y][pacman_X] = 1;
	// Aggiorna tabellone
	char output[32];
  sprintf(output, "Score %d", punteggio);
	GUI_Text(SCREEN_WIDTH/2+20, 0, (uint8_t *)output, White, Black);
	
	if(punteggioVita >= 1000){
		punteggioVita -= 1000;
		nuovaVita();
	}
	
	if(punteggio == 2400+PPPiazzate*40) { // PPPiazzate serve nel caso in cui il giocatore vinca prima che siano state estratte tutte le power pills
		vittoria();
	}
}

void pausa(void)
{
	seed = LPC_RIT->RICOUNTER;
	if(inPausa == 0){
		inPausa = 1;
		disable_timer(0);
		disable_timer(1);

		GUI_Text(SCREEN_WIDTH/2-20, SCREEN_HEIGHT/2-28, (uint8_t *) "Pause", White, Black);

	}
	else{
		inPausa = 0;
		int i, j;
		for(i = 0; i < 3; i++){
			for(j = 0; j < 6; j++){
				disegnaBlocco(((11+1)+j)*DIM_BLOCK, (SPAZIOSCRITTE+5)+(13+i)*DIM_BLOCK, Black);
			}
		}
		
		enable_timer(0);
		enable_timer(1);
	}
	return;
}

void vittoria(void)
{
	disable_timer(0);
	disable_timer(1);
	disable_RIT();
	
	LCD_Clear(Black);
	
	GUI_Text(SCREEN_WIDTH/2-30, SCREEN_HEIGHT/2-15, (uint8_t *) "Victory!", White, Black);
	
	pacman_X = -100;
	pacman_Y = -100;
}

void gameOver(void)
{
	disable_timer(0);
	disable_timer(1);
	disable_RIT();
	
	LCD_Clear(Black);
	
	GUI_Text(SCREEN_WIDTH/2-40, SCREEN_HEIGHT/2-15, (uint8_t *) "Game Over!", White, Black);
	
	pacman_X = -100;
	pacman_Y = -100;
}

void disegnaBlocco(int x_iniziale, int y_iniziale, int colore) // blocco da 8x8
{
	int i, j;
	for(i = 0; i < DIM_BLOCK; i++){
		for(j = 0; j < DIM_BLOCK; j++){
			LCD_SetPoint(x_iniziale+j, y_iniziale+i, colore);
		}
	}
}

void disegnaPill(int x_iniziale, int y_iniziale)
{
	x_iniziale += (DIM_BLOCK-DIM_BLOCK/4)/2;
	y_iniziale += (DIM_BLOCK-DIM_BLOCK/4)/2;
	int i, j;
	for(i = 0; i < DIM_BLOCK/4; i++){
		for(j = 0; j < DIM_BLOCK/4; j++){
			LCD_SetPoint(x_iniziale+j, y_iniziale+i, Grey);
		}
	}
}

void disegnaPowerPill(int x_iniziale, int y_iniziale)
{
	x_iniziale += DIM_BLOCK/4;
	y_iniziale += DIM_BLOCK/4;
	int i, j;
	for(i = 0; i < DIM_BLOCK/2; i++){
		for(j = 0; j < DIM_BLOCK/2; j++){
			LCD_SetPoint(x_iniziale+j, y_iniziale+i, Grey);
		}
	}
}

void disegnaPacMan(int colore)
{
	int i, j;
	for(i = 0; i < DIM_BLOCK; i++){
		for(j = 0; j < DIM_BLOCK; j++){
			if(pacman_draw[i][j] == 1){
				LCD_SetPoint(((pacman_X+1)*DIM_BLOCK)+j,(pacman_Y)*DIM_BLOCK+(SPAZIOSCRITTE+5)+i, colore);
			}
		}
	}
}

void disegnaCampo(int x_iniziale, int y_iniziale)
{
	int i, j;
	for(i = 0; i < 31; i++){
		for(j = 0; j < 28; j++){
			switch (campo[i][j]){
				case 0:
					disegnaBlocco(x_iniziale+j*DIM_BLOCK, y_iniziale+i*DIM_BLOCK, Blue);
					break;
				case 2:
					disegnaPill(x_iniziale+j*DIM_BLOCK, y_iniziale+i*DIM_BLOCK);
					break;
				default:
					break;
			}
		}
	}
}

void estraiPowerPills(void)
{
	int i, j, numero;

	srand(seed);
	
	for (i = 0; i < NUM_PP; i++) {
		do {
			numero = rand() % 60 + 1; // Genera un numero tra 1 e 60
			// Verifica se il numero è già presente nell'array
			for (j = 0; j < i; j++) {
				if (temporizzazionePP[j] == numero) {
					break;
				}
			}
		} while (j < i); // Ripete finché non trova un numero univoco
		temporizzazionePP[i] = numero;
	}
	
	for (i = 0; i < NUM_PP; i++) {
		do {
			numero = rand() % 240 + 1; // Genera un numero tra 1 e 240
			// Verifica se il numero è già presente nell'array
			for (j = 0; j < i; j++) {
				if (posizionePP[j] == numero) {
					break;
				}
			}
		} while (j < i); // Ripete finché non trova un numero univoco
		posizionePP[i] = numero;
	}
}

void nuovaVita()
{
	vite++;
	int i, j, v;
	for(v = 0; v < vite; v++){
		for(i = 0; i < DIM_BLOCK; i++){
			for(j = 0; j < DIM_BLOCK; j++){
				if(pacman_draw[i][j] == 1){
					LCD_SetPoint(((v+1)*DIM_BLOCK)+j, ((SPAZIOSCRITTE+5)+(ROWS*DIM_BLOCK))+i+(5), Yellow);
				}
			}
		}
	}
}

void piazzaPowerPill(int pos)
{
	int i, j, count = 0;
	pos = posizionePP[pos];
	while(1){
		for(i = 0; i < ROWS; i++){
			for(j = 0; j < COLS; j++){
				if(campo[i][j] == 2){
					count++;
					if(pos == count){
						campo[i][j] = 3;
						disegnaPowerPill((j+1)*DIM_BLOCK, SPAZIOSCRITTE+5+i*DIM_BLOCK);
						return;
					}
				}
			}
		}
	}
}