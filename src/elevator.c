#include <stdbool.h>
#include <stdio.h>
#include "cmsis_os2.h" // CMSIS-RTOS

// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "system_TM4C1294.h"
#include "ListaInt.h"
// UART definitions
extern void UARTStdioIntHandler(void);

//threads
osThreadId_t thread1_id, thread2_id, thread3_id, thread5_id;

struct ListaInt *listElevator1 = NULL;
struct ListaInt *listElevator2 = NULL;
struct ListaInt *listElevator3 = NULL;
//queue elevator
int queue_elevator1_max = 10;

bool readUARTBuffer = false;

//elevator varibles

uint8_t elevator1_floor = 0;
uint8_t elevator2_floor = 0;
uint8_t elevator3_floor = 0;

bool elevator1_isGoingUp = false;
bool elevator2_isGoingUp = false;
bool elevator3_isGoingUp = false;

bool elevator1_moving = false;
bool elevator2_moving = false;
bool elevator3_moving = false;

//queue elevator
uint8_t queue_elevator1_size = 10;
uint8_t queue_elevator2_size = 10;
uint8_t queue_elevator3_size = 10;

uint8_t queue_elevator1_count = 0;
uint8_t queue_elevator2_count = 0;
uint8_t queue_elevator3_count = 0;


void sendComand(uint8_t floor_called, char elevator, uint8_t floor_now)
{
  char message = 'f';
    if (floor_called!=floor_now)                                //Verifica se o elevador chamado não esta no mesmo andar
    {
    UARTprintf("%s%s",elevator,message);                        //Fecha a porta do elevador
    if (floor_called>floor_now)
      message = 's';
    else
      message = 'd';
    UARTprintf("%s%s",elevator,message);
    UARTFlushTx(false);
  }
}


void thread1(void *arg)                                           //Controla o elevador 1
{
  uint8_t listSize= 0;
  while(1)
  {
      listSize = tamanho(listElevator1);
      if (elevator1_moving == false && listSize > 0) // se o elevador nao esta andando eu tenho que mandar comando para andar
      {
        int removePos(listElevator1, 0)

        sendComand(msg, 'e', elevator1_floor);
        elevator1_moving = true;
      }
  }     
} // thread1





void getCommand()
{
  int floor = 0;
  bool isCommand = false;
  char receiveCode[5];
  UARTgets(receiveCode,6);
  UARTFlushRx();
  switch (receiveCode[1])
  {
    //Recebimento de andar atual
    case '1':
    switch (receiveCode[2])
    {
        case NULL:
          floor=1;
        break;
        case '0':
          floor=10;
        break;
        case '1':
          floor=11;
        break;
        case '2':
          floor=12;
        break;
        case '3':
          floor=13;
        break;
        case '4':
          floor=14;
        break;
        case '5':
          floor=15;
    }
    break;
    case '2':
      floor=2;
    break;
    case '3':
      floor=3;
    break;
    case '4':
      floor=4;
    break;
    case '5':
      floor=5;
    break;
    case '6':
      floor=6;
    break;
    case '7':
      floor=7;
    break;
    case '8':
      floor=8;
    break;
    case '9':
      floor=9;
    break;
    //Recebimento de andar atual
    
    
    //recebimento de comando do botão extern0
    case 'E':
    switch (receiveCode[2])
    {
          case '0':
          switch (receiveCode[3])
          {
            case '1':
            floor = 1;
            break;
            case '2':
              floor = 2;
            break;
            case '3':
              floor = 3;
            break;
            case '4':
              floor = 4;
            break;
            case '5':
              floor = 5;
            break;
            case '6':
              floor = 6;
            break;
            case '7':
              floor = 7;
            break;
            case '8':
              floor = 8;
            break;
            case '9':
              floor = 9;
            break;
          }
          break;
          case '1':
          switch (receiveCode[3])
          {
            case '0':
            floor = 10;
            case '1':
            floor = 11;
            break;
            case '2':
            floor = 12;
            break;
            case '3':
            floor = 13;
            break;
            case '4':
            floor = 14;
            break;
            case '5':
            floor = 15;
            break;
          }
          
   }
   //fim - Recebimento do botao externo
   //Recebimento do botao interno
    break;
    case 'I':
    isCommand = true;
    switch (receiveCode[2])
    {
          case 'a':
            floor = 1;
          break;
          case 'b':
            floor = 2;
          break;
          case 'c':
            floor = 3;
          break;
          case 'd':
            floor = 4;
          break;
          case 'e':
            floor = 5;
          break;
          case 'f':
            floor = 6;
          break;
          case 'g':
            floor = 7;
          break;
          case 'h':
            floor = 8;
          break;
          case 'i':
            floor = 9;
          break;
          case 'j':
            floor = 10;
          break;
          case 'k':
            floor = 11;
          break;
          case 'l':
            floor = 12;
          break;
          case 'm':
            floor = 13;
          break;
          case 'o':
            floor = 14;
          break;
          case 'p':
            floor = 15;
    }
    break;
    if (isCommand==false)
      switch (receiveCode[0])
      {
            case 'e':
            elevator1_floor = floor;
            break;
            case 'c':
            elevator2_floor = floor;
            break;
            case 'd':
            elevator3_floor = floor;
            break;
      }   
    else
    {
      char sentido = 'I';
      if (receiveCode[1] == 'E')
        sentido = receiveCode[4];
      switch (receiveCode[0])
      {
            case 'e':
            insert(&listElevator1,floor, sentido);
            break;
            case 'c':
            insert(&listElevator2,floor, sentido);
            break;
            case 'd':
            insert(&listElevator3,floor, sentido);
            break;
      }    
    }
  }
}

void thread5(void *arg)                        //Controla a leutura da uart que foi passada para uma thread
{
  while(1)
  {
      if (readUARTBuffer)
      {
          getCommand();
          readUARTBuffer = false;
      }
  }     
} // thread1



void UARTInit(void){
  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 115200, SystemCoreClock);

  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
  
  UARTEchoSet(false);

  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
} // UARTInit

      
void UART0_Handler(void){
  UARTStdioIntHandler();
  readUARTBuffer = true;
} // UART0_Handler



void main(void){
  UARTInit();
  osKernelInitialize();

  //char receiveCode[5] = {'e','s'};
  //decodeCommand(receiveCode);
  
  thread1_id = osThreadNew(thread1, NULL, NULL);
  thread5_id = osThreadNew(thread5, NULL, NULL);
  if(osKernelGetState() == osKernelReady)
    osKernelStart();
  //Teste decodificador

  
  while(1);
} // main
