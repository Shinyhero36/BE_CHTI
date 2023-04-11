#include "DriverJeuLaser.h"


extern int LongueurSon, TableIndex;
extern short SortieSon;
extern short Son[];
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


int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================
	
// Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers
CLOCK_Configure();
	
// Configuration de systick et de son callback
Systick_Period_ff(5512*6552*2);
Systick_Prio_IT(3, StartSon);
SysTick_On;
SysTick_Enable_IT;

// configuration de PortB.1 (PB1) en sortie push-pull
GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	
// configuration du Timer 3 pour la PWM fréquencé à 0.1 GHz (72M / 720)
GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
PWM_Init_ff( TIM3, 3, 720);

// configuration du Timer 4 en débordement 100ms
Timer_1234_Init_ff( TIM4, 6552);
Active_IT_Debordement_Timer( TIM4, 2, CallbackSon);



//============================================================================	
	
	
while	(1)
	{
		
	}
}

