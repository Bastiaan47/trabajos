#include<stdio.h>
#include<signal.h>
#include<pigpio.h>

const int filas[8]={40,38,36,32,26,24,22,18};
const int columnas[8]={37,35,33,31,29,23,21,19};
volatile sig_atomic_t signal_received=0;

void sigint_handler(int signal){
    signal_received=signal;
}

void inicializar_gpio(){
    if(gpioInitialise()==PI_INIT_FAILED){
        printf("ERROR");
        return;
    }

    for(int j=0;j<8;j++){
        gpioSetMode(columnas[j],PI_OUTPUT);
        gpioWrite(columnas[j],PI_HIGH);
    }
}

void encender_fila(int fila){
    for(int i=0;i<8;i++){
        gpioWrite(filas[i],(i==fila)?PI_HIGH:PI_LOW);
    }
}

int main(){
    if(gpioInitialise()==PI_INIT_FAILED){
        printf("ERROR");
        return 1;
    }

    signal(SIGINT,sigint_handler);
    inicializar_gpio();
    printf("Presione CTRL-C para salir.\n");

    while(!signal_received){
        for(int fila=0;fila<8;fila++){
            encender_fila(fila);
            gpioSleep(PI_TIME_RELATIVE,0,1000000);//1 seg de espera

        }
    }
    for(int i=0;i<8;i++){
        gpioSetMode(filas[i],PI_INPUT);
    }

    gpioTerminate();
    printf("\n");
    return 0;


}