/* ******************************************************************************/
/*                                                                              */
/* Programa para testear el funcionamiento del pulsador S3 y el led LED3 del    */
/* Basic BoosterPack v2 (BBPv2) para el microcontrolador MSP430G2533            */
/*                                                                              */
/* Autor: Jose A. Jimenez                                                       */
/* Version: 1.0                                                                 */
/*                                                                              */
/* ******************************************************************************/

#include "io430g2533.h"                 // MSP430G2533

#define ON          1                   // Activo
#define OFF         0                   // Desactivo
#define PULS_ON     0                   // Pulsador presionado
#define PULS_OFF    1                   // Pulsador soltado
#define LED_ON      1                   // LED encendido
#define LED_OFF     0                   // LED apagado



typedef union {                         // Flags        
                                        
    unsigned char total;                // Todos
    struct {                        
        unsigned char f0 :1;            // Flag 0
        unsigned char f1 :1;            // Flag 1
    };
} flags;


flags tareas;                           // Flags activacion tareas:
                                        //  BIT0: Tarea_0 -> Pulsador S3 presionado
                                        //  BIT1: Tarea_1 -> Pulsador S3 soltado



/* ===== Prototipos ===== */

void Config_uC();

void Config_Perifericos();



/* ===== MAIN ===== */

int main () {
    
    // Configuracion uControlador
    Config_uC();
    
    // Configuracion de perifericos
    Config_Perifericos();
    
    // Inicializamos variables
    tareas.total = 0;
    
    // Habilitamos interrupciones
    __enable_interrupt();
    
    // Bucle principal
    for (;;) {
    
        // Bajo consumo (LPM4)
        __low_power_mode_4();
        
        while (tareas.total != OFF) {
            
            // Tarea_0 -> Pulsador S3 presionado (enciende LED3)
            if (tareas.f0 == ON) {      
                tareas.f0 = OFF;        // Limpia flag Tarea_0
                P2OUT_bit.P4 = LED_ON;  // P2.4 -> Enciende LED3
            } // if (tareas.f0 == ON)
            
            // Tarea_1 -> Pulsador S3 soltado (apaga LED3)
            if (tareas.f1 == ON) {
                tareas.f1 = OFF;        // Limpia flag Tarea_1
                P2OUT_bit.P4 = LED_OFF; // P2.4 -> Apaga LED3
            } // if (tareas.f1 == ON)
            
        } // Fin while (tareas.total)
    
    } // Fin for (;;)
    
    //return 0;   ?????
} //Fin MAIN



/* ===== FUNCIONES ===== */

void Config_uC() {
    
    WDTCTL = WDTPW + WDTHOLD;        // Detiene watchdog
    DCOCTL = 0;                      // Frecuencia DCO (1MHz)
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
} // Fin Config_uC()


void Config_Perifericos() {
    
    // ----- Pulsadores [S3, S4, S5, S6] -----
    
    // Pulsador S3 (p1.4)
    P1SEL_bit.P4    = 0;                // Configuramos pin como E/S
    P1SEL2_bit.P4   = 0;                //
    P1DIR_bit.P4    = 0;                // Establecido como salida
    P1REN_bit.P4    = 1;                // Habilitamos resistencia
    P1OUT_bit.P4    = 1;                // Resistencia pullup
    P1IE_bit.P4     = 1;                // Habilitamos interrupciones
    P1IES_bit.P4    = 1;                // Interrupcion cuando 1 -> 0
    P1IFG_bit.P4    = 0;                // Limpiamos flag interrupcion
    
        
    // ----- Leds [LED3, LED4, LED5, LED6] -----
   
    // Led LED3 (p2.4)
    P2SEL_bit.P4    = 0;                // Configuramos pin como E/S
    P2SEL2_bit.P4   = 0;                //
    P2DIR_bit.P4    = 1;                // Establecido como salida
    P2OUT_bit.P4    = 0;                // Establecemos valor inicial
    
} // Fin Config_Perifericos



/* ===== RUTINAS TRATAMIENTO INTERRUPCION ===== */

// Rutina de tratamiento del puerto 1 (P1)
#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1 () {
    
    if (P1IES_bit.P4 == ON) {       	// Si flanco de bajada (pulsador presionado)
		tareas.f0 = ON;			   		// Activar Tarea_0
	} // if (P1IES_bit.P3 == ON)

	else {                          	// Si flanco de subida (pulsador soltado)
		tareas.f1 = ON;			    	// Activar Tarea_1
	} // else: if (P1IES_bit.P3 == ON)

	P1IES_bit.P4 = ~(P1IES_bit.P4);		// P1.4 -> Conmuta flanco
	P1IFG_bit.P4 = OFF;					// Limpia flag P1.4

	if (tareas.total != OFF) {
	// Si alguna tarea activa
		__low_power_mode_off_on_exit();	// Sale bajo consumo (LPM4)
	} // if (tareas.total != OFF)
    
} // Fin RTI_P1()





