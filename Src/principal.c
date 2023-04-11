#include "DriverJeuLaser.h"


extern int LongueurSon, TableIndex;
extern short Son, SortieSon;
//CallbackSon Fonction C et ASM
void CallbackSon(void);
void CallbackSon_VertionC(){
	if (LongueurSon > TableIndex){
		int NoteSon = (&Son)[TableIndex];
		TableIndex++;
		NoteSon += 32768;
		NoteSon /= 91;
		SortieSon = NoteSon;
		PWM_Set_Value_TIM3_Ch3(NoteSon);
	}
}



int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================
	
// Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers
CLOCK_Configure();
	
// configuration de PortB.1 (PB1) en sortie push-pull
GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	
// configuration du Timer 3 pour la PWM fr�quenc� � 0.1 GHz (72M / 720)
GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
PWM_Init_ff( TIM3, 3, 720);

// configuration du Timer 4 en d�bordement 100ms
Timer_1234_Init_ff( TIM4, 6552);
Active_IT_Debordement_Timer( TIM4, 2, CallbackSon);


//============================================================================	
	
	
while	(1)
	{
	}
}

