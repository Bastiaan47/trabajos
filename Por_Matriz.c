//[Encender LED por Matriz]//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pigpio.h>

#define filas 8
#define columnas 8

const int led_pos[filas] ={40,38,36,32,26,24,22,18};
const int led_neg[columnas] ={37,35,33,31,29,23,21,19};
int inicio();
void sigint_handler(int signal);
void abrirMatriz(char *orden, int matriz[filas][columnas]);
void Prender_Apagar(int matriz[filas][columnas]);
volatile sig_atomic_t signal_received = 0;

int main(){
    int matriz[filas][columnas];
    abrirMatriz("orden.txt", matriz);
    inicio();
    Prender_Apagar(matriz);
    gpioTerminate();
    printf("\n");
    return 0;
}

int inicio(){

  if(gpioInitialise()== PI_INIT_FAILED){ 
    printf("ERROR: No funciona-Fallo en la interfaz\n");
    return 1;
  }

  signal(SIGINT, sigint_handler);
  printf("Pesiona CTRL-C para salir.\n");
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

void Prender_Apagar(int matriz[filas][columnas]){
    for(int i=0; i<filas; i++){
        gpioSetMode(led_pos[i], PI_OUTPUT);
        for(int j=0; j<columnas; j++){
            gpioSetMode(led_neg[j], PI_OUTPUT);
            if (matriz[i][j] == 1){
                gpioWrite(led_pos[i], PI_HIGH);
                gpioWrite(led_neg[j], PI_LOW);
            }else{
                gpioWrite(led_pos[i], PI_LOW);
                gpioWrite(led_neg[j], PI_HIGH);
            }
            time_sleep(1);
        }
    }
}
