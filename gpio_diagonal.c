#include<stdio.h>
#include<pigpio.h>
#include<signal.h>

#define fil 8
#define col 8

int filas[fil]={40,38,36,32,26,24,22,18};
int columnas[col]={37,35,33,31,29,23,21,19};

void handle_signal(int signum){
    gpioTerminate();
    printf("Programa terminado.\n");
    exit(signum);
}

void inicializar_gpio(){
    if(gpioInitialise()<0){
        fprintf(stderr, "error");
        exit(1);
    }
    for(int j=0;j<col;j++){
        gpioSetMode(columnas[j],PI_OUTPUT);
        gpioWrite(columnas[j],0);
    }
}

void encender_led(int fila, int columna){
    gpioWrite(columnas[columna],1);
    gpioWrite(filas[fila],0);
    gpioSleep(PI_TIME_RELATIVE,0,500000); //500 ms
}

void apagar_led(){
    for(int i=0;i<fil;i++){
        gpioWrite(filas[i],1);
    }
    for(int j=0;j<col;j++){
        gpioWrite(columnas[j],0);
    }
}

int main(){
    if(gpioInitialise()<0){
        fprintf(stderr,"ERROR");
        return 1;
    }

    inicializar_gpio();
    signal(SIGINT,handle_signal);
    for(int i=0;i<fil;i++){
        encender_led(i,i);
        gpioSleep(PI_TIME_RELATIVE,1,0);
        apagar_led();
    }
    gpioTerminate();
    return 0;
}