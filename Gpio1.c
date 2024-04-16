#include<stdio.h>
#include<pigpio.h>
#include<time.h>

int const positivos[8]={40,38,36,32,26,24,22,18};
int const negativos[8]={37,35,33,31,29,23,21,19};

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal){
    signal_received=signal;
}
//void inicializar_led();
//void apagar_led();
//void encender_led();

int main(){
    if(gpioInitialise()==PI_INIT_FAILED){
        printf("Ha ocurrido un error al iniacializar el gpio");
    }

    for(int i=0;i<8;i++){
        gpioSetMode(positivos[i],PI_OUTPUT);
        gpioSetMode(negativos[i],PI_OUTPUT); 
    }

    signal(SIGINT,sigint_handler);
    printf("Presiona Ctrl-C para salir");
    int display_led[8][8]={
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1},

    }
    while(!signal_received){
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(display_led[i][j]==1){
                    gpioWrite(positivos[j],PI_HIGH);
                    gpioWrite(negativos[i],PI_HIGH);
                }else{
                    gpioWrite(positivos[j],PI_LOW);
                    gpioWrite(negativos[i],PI_LOW);
                }
            }
            time_sleep(0.5);
        }
    }
    


    for(int i=0;i<8;i++){
        gpioWrite(positivos[i],PI_LOW);
        gpioWrite(negativos[i],PI_LOW)
    }

    for(int i=0;i<8;i++){
        gpioSetMode(positivos[i],PI_INPUT);
        gpioSetMode(negativos[i],PI_INPUT);
    }

    gpioTerminate()
    printf("\n");
    return 0;

}

//void inicializar_led(){}

//void apagar_led(){}

//void encender_led(){}
