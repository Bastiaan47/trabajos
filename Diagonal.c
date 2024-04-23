//[Encender todos los LED 1 por 1]//
#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

#define led 8
//--numeros de los pines--//
const int led_pos[led] ={40,38,36,32,26,24,22,18};
const int led_neg[led] ={37,35,33,31,29,23,21,19};

void Prender_Apagar();
int inicio();
const int RedLed=21;
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal){
 signal_received = signal;
}

int main(){
  inicio();
  Prender_Apagar();
  pioTerminate();
  printf("\n");
  return 0;
}

int inicio(){
  //--Verifica si se inicio con exito
  if(gpioInitialise()== PI_INIT_FAILED){ 
    printf("ERROR: No funciona-Fallo en la interfaz\n");
    return 1;
  }
  //--Establecer controlador de señal--//
  signal(SIGINT, sigint_handler);
  printf("Pesiona CTRL-C para salir.\n");

}

void Prender_Apagar(){
 //--Configurar pines positivos y negativos como salida--//
   for(int i=0; i<led; i++){
      gpioSetMode(led_pos[i], PI_OUTPUT);
      gpioSetMode(led_neg[i], PI_OUTPUT);
   }

   while (!signal_received){
      
      for(int i=0; i<led; i++){
       //--Encender LED positivo y Apagar negativo--//
         gpioWrite(led_pos[i], PI_HIGH);
         gpioWrite(led_neg[i], PI_LOW);
         time_sleep(1);//tiempo de pausa 
       //--Encender LED negativo y Apagar Positivo--//
         gpioWrite(led_pos[i], PI_LOW);
         gpioWrite(led_neg[i], PI_HIGH);
       //time_sleep(1);
      }
      
   }
 //--Restaurar Pines como entradas--//
   for(int i=0; i<led; i++){
      gpioSetMode(led_pos[i], PI_INPUT);
      gpioSetMode(led_neg[i], PI_INPUT);
   }
}
