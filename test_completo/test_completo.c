/* ************************************************************************** */
/* Programa para testear el funcionamiento del Basic BoosterPack v2 (BBPv2)   */
/* para el microcontrolador MSP430G2533                                       */
/*                                                                            */
/* Autor: Jose A. Jimenez                                                     */
/* Version: 0.2                                                               */
/* ************************************************************************** */

#include "io430g2533.h"                 // MSP430G2533

#define ON          1                   // Activo
#define OFF         0                   // Desactivo
#define PULS_ON     0                   // Pulsador presionado
#define PULS_OFF    1                   // Pulsador soltado
#define LED_ON      1                   // LED encendido
#define LED_OFF     0                   // LED apagado


                                        // NO DEBERIA ESTAR INICIALIZADO A 0????
                                        // CORRECION DE ERRORES??

typedef union {                         // Flags        
                                        // tamañ campo bits
    unsigned char total;                // Todos
    struct {                        
        unsigned char f0 :1;            // Flag 0
        unsigned char f1 :1;            // Flag 1
        unsigned char f2 :1;            // Flag 2
        unsigned char f3 :1;            // Flag 3
        unsigned char f4 :1;            // Flag 4
        unsigned char f5 :1;            // Flag 5
        unsigned char f6 :1;            // Flag 6
        unsigned char f7 :1;            // Flag 7
    };
} flags;


flags tareas;                           // Flags activacion tareas:
                                        //  BIT0: Tarea_0 -> Pulsador S3 presionado
                                        //  BIT1: Tarea_1 -> Pulsador S3 soltado
                                        //  BIT2: Tarea_2 -> Pulsador S4 presionado
                                        //  BIT3: Tarea_3 -> Pulsador S4 soltado
                                        //  BIT4: Tarea_4 -> Pulsador S5 presionado
                                        //  BIT5: Tarea_5 -> Pulsador S5 soltado
                                        //  BIT6: Tarea_6 -> Pulsador S6 presionado
                                        //  BIT7: Tarea_7 -> Pulsador S6 soltado


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
        
        while (tareas.total != OFF) {           // CAMBIAR if´s POR SWITCH??????
            
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
            
            
            /*
            
            // Tarea_2 -> Pulsador S4 presionado (enciende LED4)
            if (tareas.f2 == ON) {      
                tareas.f2 = OFF;        // Limpia flag Tarea_2
                P2OUT_bit.P5 = LED_ON;  // P2.5 -> Enciende LED4
            } // if (tareas.f2 == ON)
            
            // Tarea_3 -> Pulsador S4 soltado (apaga LED4)
            if (tareas.f3 == ON) {
                tareas.f3 = OFF;        // Limpia flag Tarea_3
                P2OUT_bit.P5 = LED_OFF; // P2.5 -> Apaga LED4
            } // if (tareas.f3 == ON)
            
            // Tarea_4 -> Pulsador S5 presionado (enciende LED5)
            if (tareas.f4 == ON) {      
                tareas.f4 = OFF;        // Limpia flag Tarea_4
                P2OUT_bit.P6 = LED_ON;  // P2.6 -> Enciende LED5
            } // if (tareas.f4 == ON)
            
            // Tarea_5 -> Pulsador S5 soltado (apaga LED5)
            if (tareas.f5 == ON) {
                tareas.f5 = OFF;        // Limpia flag Tarea_5
                P2OUT_bit.P6 = LED_OFF; // P2.6 -> Apaga LED5
            } // if (tareas.f5 == ON)
            
            // Tarea_6 -> Pulsador S6 presionado (enciende LED6)
            if (tareas.f6 == ON) {      
                tareas.f6 = OFF;        // Limpia flag Tarea_6
                P2OUT_bit.P7 = LED_ON;  // P2.7 -> Enciende LED6
            } // if (tareas.f6 == ON)
            
            // Tarea_7 -> Pulsador S6 soltado (apaga LED6)
            if (tareas.f7 == ON) {
                tareas.f7 = OFF;        // Limpia flag Tarea_1
                P2OUT_bit.P7 = LED_OFF; // P2.7 -> Apaga LED6
            } // if (tareas.f7 == ON)
            
            */
            
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


void Config_Perifericos() {                          // POR AHORA SOLO S3 y LED3
    
    // ----- Pulsadores [S3, S4, S5, S6] -----       Â¿P1DIR = 0xXXXX?
    
    // Pulsador S3 (p1.4)
    P1SEL_bit.P4    = 0;                // Configuramos pin como E/S
    P1SEL2_bit.P4   = 0;                //
    P1DIR_bit.P4    = 0;                // Establecido como salida
    P1REN_bit.P4    = 1;                // Habilitamos resistencia
    P1OUT_bit.P4    = 1;                // Resistencia pullup
    P1IE_bit.P4     = 1;                // Habilitamos interrupciones
    P1IES_bit.P4    = 1;                // Interrupcion cuando 1 -> 0
    P1IFG_bit.P4    = 0;                // Limpiamos flag interrupcion
    
    /*
    
    // Pulsador S4 (p2.1)
    P2SEL_bit.P1    = 0;                // Configuramos pin como E/S
    P2SEL2_bit.P1   = 0;                //
    P2DIR_bit.P1    = 0;                // Establecido como salida
    P2REN_bit.P1    = 1;                // Habilitamos resistencia
    P2OUT_bit.P1    = 1;                // Resistencia pullup
    P2IE_bit.P1     = 1;                // Habilitamos interrupciones
    P2IES_bit.P1    = 1;                // Interrupcion cuando 1 -> 0
    P2IFG_bit.P1    = 0;                // Limpiamos flag interrupcion
    
    // Pulsador S5 (p2.2)
    P2SEL_bit.P2    = 0;                // Configuramos pin como E/S
    P2SEL2_bit.P2   = 0;                //
    P2DIR_bit.P2    = 0;                // Establecido como salida
    P2REN_bit.P2    = 1;                // Habilitamos resistencia
    P2OUT_bit.P2    = 1;                // Resistencia pullup
    P2IE_bit.P2     = 1;                // Habilitamos interrupciones
    P2IES_bit.P2    = 1;                // Interrupcion cuando 1 -> 0
    P2IFG_bit.P2    = 0;                // Limpiamos flag interrupcion
    
    // Pulsador S6 (p2.3)
    P2SEL_bit.P3    = 0;                // Configuramos pin como E/S
    P2SEL2_bit.P3   = 0;                //
    P2DIR_bit.P3    = 0;                // Establecido como salida
    P2REN_bit.P3    = 1;                // Habilitamos resistencia
    P2OUT_bit.P3    = 1;                // Resistencia pullup
    P2IE_bit.P3     = 1;                // Habilitamos interrupciones
    P2IES_bit.P3    = 1;                // Interrupcion cuando 1 -> 0
    P2IFG_bit.P3    = 0;                // Limpiamos flag interrupcion

    */
    
    // ----- Leds [LED3, LED4, LED5, LED6] -----
   
    // Led LED3 (p2.4)
    P2SEL_bit.P4    = 0;                // Configuramos pin como E/S
    P2SEL2_bit.P4   = 0;                //
    P2DIR_bit.P4    = 1;                // Establecido como salida
    P2OUT_bit.P4    = 0;                // Establecemos valor inicial
    /*
    P2REN_bit.P4    = 0;                // Resistencia deshabilitada     ???????
    P2IE_bit.P4     = 0;                // Interrupciones deshabilitadas ???????
    */
    
} // Fin Config_Perifericos



/* ===== RUTINAS TRATAMIENTO INTERRUPCION ===== */

// Rutina de tratamiento del puerto 1 (P1)
#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1 () {
    
    // Pulsador S3 y LED3
    
    if (P1IES_bit.P4 == ON) {       // Si flanco de bajada (pulsador presionado)
		tareas.f0 = ON;			    // Activar Tarea_0
	} // if (P1IES_bit.P3 == ON)

	else {                          // Si flanco de subida (pulsador soltado)
		tareas.f1 = ON;			    // Activar Tarea_1
	} // else: if (P1IES_bit.P3 == ON)

	P1IES_bit.P4 = ~(P1IES_bit.P4);		// P1.4 -> Conmuta flanco
	P1IFG_bit.P4 = OFF;			// Limpia flag P1.4

	if (tareas.total != OFF) {
	// Si alguna tarea activa
		__low_power_mode_off_on_exit();	// Sale bajo consumo (LPM4)
	} // if (tareas.total != OFF)
    
} // Fin RTI_P1()





