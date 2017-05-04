/* ************************************************************************** */
/* Programa para testear el funcionamiento del Basic BoosterPack v2 (BBPv2)   */
/* para el microcontrolador MSP430G2533                                       */
/*                                                                            */
/* Autor: Jose A. Jimémez                                                     */
/* Version: 0.1                                                               */
/* ************************************************************************** */

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
                                        //  BIT0: Tarea_2 -> Pulsador S4 presionado
                                        //  BIT1: Tarea_3 -> Pulsador S4 soltado
                                        //  BIT0: Tarea_4 -> Pulsador S5 presionado
                                        //  BIT1: Tarea_5 -> Pulsador S5 soltado
                                        //  BIT0: Tarea_6 -> Pulsador S6 presionado
                                        //  BIT1: Tarea_7 -> Pulsador S6 soltado


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
    
    // ----- Pulsadores [S3, S4, S5, S6] -----       ¿P1DIR = 0xXXXX?
    
    // Pulsador S3
    P1DIR_bit.P4 = OFF;              // P1.4 -> Entrada (S3)
    P1REN_bit.P4 = ON;               // P1.4 -> Resistencia activa
    P1OUT_bit.P4 = ON;               // P1.4 -> Resistencia pullup
    P1IES_bit.P4 = ON;               // P1.4 -> Flanco bajada
    P1IFG_bit.P3 = OFF;              // P1.4 -> Limpia flag P1.4
    P1IE_bit.P3 = ON;                // Habilita interrupcion P1.4
    
    // Pulsador S4
    P2DIR_bit.P1 = OFF;              // P2.1 -> Entrada (S4)
    P2REN_bit.P1 = ON;               // P2.1 -> Resistencia activa
    P2OUT_bit.P1 = ON;               // P2.1 -> Resistencia pullup
    P2IES_bit.P1 = ON;               // P2.1 -> Flanco bajada
    P2IFG_bit.P1 = OFF;              // P2.1 -> Limpia flag P2.1
    P2IE_bit.P1 = ON;                // Habilita interrupcion P2.1
    
    // Pulsador S5
    P2DIR_bit.P2 = OFF;              // P2.2 -> Entrada (S5)
    P2REN_bit.P2 = ON;               // P2.2 -> Resistencia activa
    P2OUT_bit.P2 = ON;               // P2.2 -> Resistencia pullup
    P2IES_bit.P2 = ON;               // P2.2 -> Flanco bajada
    P2IFG_bit.P2 = OFF;              // P2.2 -> Limpia flag P2.2
    P2IE_bit.P2 = ON;                // Habilita interrupcion P2.2
    
    // Pulsador S6
    P2DIR_bit.P3 = OFF;              // P2.3 -> Entrada (S6)
    P2REN_bit.P3 = ON;               // P2.3 -> Resistencia activa
    P2OUT_bit.P3 = ON;               // P2.3 -> Resistencia pullup
    P2IES_bit.P3 = ON;               // P2.3 -> Flanco bajada
    P2IFG_bit.P3 = OFF;              // P2.3 -> Limpia flag P2.3
    P2IE_bit.P3 = ON;                // Habilita interrupcion P2.3
    
    
    // ----- Leds [LED3, LED4, LED5, LED6] -----
   
    // Salidas
    P2DIR_bit.P4 = ON;               // P2.4 -> Salida (LED3)
    P2DIR_bit.P5 = ON;               // P2.5 -> Salida (LED4)
    P2DIR_bit.P6 = ON;               // P2.6 -> Salida (LED5)
    P2DIR_bit.P7 = ON;               // P2.7 -> Salida (LED6)
    
    // Estado inicial
    P2DIR_bit.P4 = LED_OFF;          // P2.4 -> Apaga (LED3)
    P2DIR_bit.P5 = LED_OFF;          // P2.5 -> Apaga (LED4)
    P2DIR_bit.P6 = LED_OFF;          // P2.6 -> Apaga (LED5)
    P2DIR_bit.P7 = LED_OFF;          // P2.7 -> Apaga (LED6)
    
   
} // Fin Config_Perifericos



/* ===== RUTINAS TRATAMIENTO INTERRUPCION ===== */

// Rutina de tratamiento del puerto 1 (P1)
#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1 {
    
    if 
    
} // Fin RTI_P1()





