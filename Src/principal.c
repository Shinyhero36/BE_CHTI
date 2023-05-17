#include "DriverJeuLaser.h"
#include "Affichage_Valise.h"

//Variable externe
void wait(void);
extern int LongueurSon, TableIndex;
extern short SortieSon;
extern short Son[], TabCos[], TabSin[], LeSignal[];
//CallbackSon Fonction C et ASM
void CallbackSon(void);
void CallbackSon_VertionC(){
	if (LongueurSon > TableIndex){
		int NoteSon = Son[TableIndex];
		TableIndex++;
		NoteSon += 32768;
		NoteSon /= 91;
		SortieSon = NoteSon;
		PWM_Set_Value_TIM3_Ch3(NoteSon);
	}
}
//StartSon Fonction C et ASM
void StartSon(void);
void StartSon_VertionC(){
	TableIndex = 0;
}
// DFT_ModuleAuCarre Fonction C et ASM
int DFT_ModuleAuCarre( short int * Signal64ech, char k);
int DFT_ModuleAuCarre_VertionC( short int * Signal64ech, char k){
	int re = 0, im = 0;
	for (int i=0; i < 64; i++) {
		int feur = (k * i) % 64;
		int signal = Signal64ech[i];
		re += TabCos[feur] * signal;
		im += TabSin[feur] * signal;
	}
	re = (int) (((long long) re * (long long)re) >> 32);
	im = (int) (((long long) im * (long long) im) >> 32);
	return re + im;
}

// Gestion du score Fonction C
short dma_buf[64];
char tmp_sc[4];
char score[4];
char scoring(char i, char j){
	if (tmp_sc[i] < 0){
		tmp_sc[i] += 1;
	}
	else if(tmp_sc[i] < 5){
		int tmp = DFT_ModuleAuCarre_VertionC(dma_buf, j);
		if(tmp >> 16) {
			tmp_sc[i] += 1;
		}
		else if ( tmp_sc[i] > 0){
			tmp_sc[i] -= 1;
		}
	}
	else {
		tmp_sc[i] = -40;
		score[i] += 1;
	}
}
int CAMARCHE = 0;
void CallbackSystick() {
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	char update = 0;
	update += scoring(0, 17);
	update += scoring(1, 18);
	update += scoring(2, 19);
	update += scoring(3, 20);
}
void CallBackAffich() {
	Prepare_Afficheur(1, score[0]);
	Prepare_Afficheur(2, score[1]);
	Prepare_Afficheur(3, score[2]);
	Prepare_Afficheur(4, score[3]);
	CAMARCHE = 1-CAMARCHE;
	if(CAMARCHE)
		Prepare_Set_LED(LED_LCD_V);
	else
		Prepare_Clear_LED(LED_LCD_V);
	Mise_A_Jour_Afficheurs_LED();
}

int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

// Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers
CLOCK_Configure();
	
// Configuration de systick et de son callback
Systick_Period_ff(360000);
Systick_Prio_IT(3, CallbackSystick);
SysTick_On;
SysTick_Enable_IT;

// Configuration de l'ADC
Init_TimingADC_ActiveADC_ff( ADC1, 72 );
Single_Channel_ADC( ADC1, 2 );
Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
Init_ADC1_DMA1( 0, dma_buf );
	
// configuration de PortB.1 (PB1) en sortie push-pull
GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	
// configuration du Timer 3 pour la PWM fréquencé à 0.1 GHz (72M / 720)
GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
PWM_Init_ff( TIM3, 3, 720);

// configuration du Timer 4 en débordement 100ms
Timer_1234_Init_ff( TIM4, 6552);
Active_IT_Debordement_Timer( TIM4, 2, CallbackSon);

// configuration du Timer 1 en débordement 500ms
//Timer_1234_Init_ff( TIM2, 6552);
//Active_IT_Debordement_Timer( TIM2, 9, CallBackAffich);

// Configuration de l'afficheur
Init_Affichage();
//CallBackAffich();
//Mise_A_Jour_Afficheurs_LED();



//============================================================================	
	
int i;
while	(1){
	CallBackAffich();
	for(i = 0X200000;i>0;i--);
 }
}

