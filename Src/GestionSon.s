	PRESERVE8
	THUMB
	include DriverJeuLaser.inc
	import LongueurSon
	import Son
	export SortieSon
	export TableIndex
		

; ====================== zone de réservation de données,  ======================================
;Section RAM (read only) :
	area    mesdata,data,readonly


;Section RAM (read write):
	area    maram,data,readwrite
TableIndex	dcd 0
SortieSon	dcw 0
		

	
; ===============================================================================================
	


		
;Section ROM code (read only) :		
	area    moncode,code,readonly
; écrire le code ici		



	EXPORT CallbackSon

;	void CallbackSon_VertionC(){
CallbackSon proc
;		if (LongueurSon > TableIndex){
	ldr r2, =TableIndex							;R0	... - R1 ... - R2 ... - R3 ...
	ldr r1, [r2]								;R0	... - R1 ... - R2 &TI - R3 ...
	ldr r0, =LongueurSon						;R0 ... - R1  TI - R2 &TI - R3 ...
	ldr r0, [r0]								;R0 &LS - R1  TI - R2 &TI - R3 ...
	cmp r0, r1									;R0  LS - R1  TI - R2 &TI - R3 ...
	ble FinCallbackSon
;			int NoteSon = (&Son)[TableIndex];
	ldr r3, =Son								;R0 ... - R1  TI - R2 &TI - R3 ...
	ldrsh r0, [r3, r1, lsl #1]					;R0 ... - R1  TI - R2 &TI - R3 &SO
;			TableIndex++;
	add r1, #1									;R0  NS - R1  TI - R2 &TI - R3 ...
	str r1,[r2]									;R0  NS - R1  TI - R2 &TI - R3 ...
;			NoteSon += 32768;
	add r0, #32768								;R0  NS - R1 ... - R2 ... - R3 ...
;			NoteSon /= 91;
	mov r1, #91									;R0  NS - R1 ... - R2 ... - R3 ...
	udiv r0, r1									;R0  NS - R1 #92 - R2 ... - R3 ...
;			SortieSon = NoteSon;
	ldr r1, =SortieSon							;R0  NS - R1 ... - R2 ... - R3 ...
	str r0, [r1]								;R0  NS - R1 &SS - R2 ... - R3 ...
;			PWM_Set_Value_TIM3_Ch3(NoteSon);
	push {lr}									;R0  NS - R1 ... - R2 ... - R3 ...
	bl PWM_Set_Value_TIM3_Ch3					;R0  NS - R1 ... - R2 ... - R3 ...
	pop {lr}									;R0 ... - R1 ... - R2 ... - R3 ...
;		}
;	}
FinCallbackSon
	bx lr
	endp
	END
		