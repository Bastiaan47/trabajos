//---[ENCENDER POR MATRIZ]---//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>
#include <time.h>

#define filas 8
#define columnas 8

//--numeros de los pines--//
const int led_pos[filas] ={24,25,8,7,12,16,20,21};
const int led_neg[columnas] ={10,9,11,5,6,13,19,26};
int inicioGPIO();
void sigint_handler(int signal);
void abrirMatriz(char *orden, int matriz[filas][columnas]);
void Prender_Apagar(int matriz[filas][columnas], int tiempo);
volatile sig_atomic_t signal_received = 0;

int main(){
    int matriz[filas][columnas];
    int tiempo = 0;
    abrirMatriz("orden.txt", matriz);
    if (inicioGPIO() == 1) return 1;
    //--Establecer controlador de señal--//
    signal(SIGINT, sigint_handler);
    printf("Pesiona CTRL-C para salir.\n");
    
    printf("ingrese la cantidad de segundos: \n");
    scanf("%d", &tiempo);
    Prender_Apagar(matriz, tiempo);
    gpioTerminate();
    printf("\n");
    return 0;
}

int inicioGPIO(){

  if(gpioInitialise()== PI_INIT_FAILED){ 
    printf("ERROR: No funciona-Fallo en la interfaz\n");
    return 1;
  }

   //--Configurar pines positivos y negativos como salida--//
   for(int i=0; i<filas; i++){
      gpioSetMode(led_pos[i], PI_OUTPUT);
      gpioSetMode(led_neg[i], PI_OUTPUT);
      //--Apagar--//
      gpioWrite(led_pos[i], PI_LOW);
      gpioWrite(led_neg[i], PI_HIGH);
      
   }
  return 0;
}

void sigint_handler(int signal){
 signal_received = signal;
}

void abrirMatriz(char *orden, int matriz[filas][columnas]){
    FILE *file; 
    file= fopen(orden, "r");
    if(file == NULL){
        printf("Error al abrir archivo.\n");
        exit(1);
    }

    for (int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            fscanf(file, "%d", &matriz[i][j]);
        }
    }
    fclose(file);
}

void Prender_Apagar(int matriz[filas][columnas], int tiempo){
    time_t inicio;
    double seg;
    inicio= time(NULL); 

    while ((seg = difftime(time(NULL), inicio))< tiempo && !signal_received){
        for(int i=0; i<filas; i++){
            for(int j=0; j<columnas; j++){
                if (matriz[i][j] == 1){
                    gpioWrite(led_pos[i], PI_HIGH);
                    gpioWrite(led_neg[j], PI_LOW);
                    time_sleep(0.001);
    
                    gpioWrite(led_pos[i], PI_LOW);
                    gpioWrite(led_neg[j], PI_HIGH);
                }
            }
        }
    }
}
