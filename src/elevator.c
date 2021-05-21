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
osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

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

uint8_t elevator1_targetFloor = 0;
uint8_t elevator2_targetFloor = 0;
uint8_t elevator3_targetFloor = 0;

char elevator1_way = 'p';  /// p parado, d descendo, s subindo
char elevator2_way = 'p';
char elevator3_way = 'p';

bool elevator1_moving = false;
bool elevator2_moving = false;
bool elevator3_moving = false;

bool elevator1ReceiveCommand = false;
bool elevator2ReceiveCommand = false;
bool elevator3ReceiveCommand = false;


typedef struct elevatorParameters
{
  char elevatorCharacter;
  uint32_t led_time;
}elevatorParameters;



void sendComand(uint8_t floor_called, char elevator, uint8_t floor_now)
{
  char message = 'f';
    if (floor_called!=floor_now)                                //Verifica se o elevador chamado não esta no mesmo andar
    {
    UARTprintf("%c%c\r",elevator,message);  //Fecha a porta do elevador
    UARTFlushTx(false);
    if (floor_called>floor_now)
      message = 's';
    else
      message = 'd';
    switch (elevator)
    {
      case 'e':
       elevator1_way = message;
      break;
      case 'c':
        elevator2_way = message;
      break;
      case 'd':
        elevator3_way = message;
      break ; 
    }
    UARTprintf("%c%c\r",elevator,message);
    UARTFlushTx(false);
  }
}


void thread2(void *arg)                                           //Controla o elevador 1
{
  uint8_t listSize= 0;

  while(1)
  {
    if (elevator2ReceiveCommand)
    {
      listSize = tamanho(listElevator2);
      if (elevator2_moving == false) // se o elevador nao esta andando eu tenho que mandar comando para andar
      {
        if (listSize > 0)
        {
          elevator2_targetFloor = removePos(&listElevator2, 0);
          sendComand(elevator2_targetFloor , 'c', elevator2_floor);
          elevator2_moving = true;
        }
      }
      else
      {
        if ((elevator2_floor >= elevator2_targetFloor && elevator2_way == 's')|| (elevator2_floor <= elevator2_targetFloor && elevator2_way == 'd'))
        {
          UARTprintf("cp\r");
          UARTFlushTx(false);
          UARTprintf("ca\r");
          UARTFlushTx(false);
          osDelayUntil(2000000);
          elevator2_moving = false;
        }
        else
        {
          if (listSize > 0)
          {
            if (searchValueAndRemove(&listElevator2, elevator2_floor, elevator2_way, listSize)==1) ///Tentar fazer a comparação somente quando o andar mudar
            {
              UARTprintf("cp\r");
              UARTFlushTx(false);
              UARTprintf("ca\r");
              UARTFlushTx(false);
              osDelayUntil(2000000);
              UARTprintf("cf\r");
              UARTFlushTx(false);
              UARTprintf("c%s\r",elevator2_way);
            }   
          }
        }
      }
      elevator2ReceiveCommand = false;
    }
  }     
} // thread2

void thread3(void *arg)                                           //Controla o elevador 3
{
  uint8_t listSize= 0;

  while(1)
  {
    if (elevator3ReceiveCommand)
    {
      listSize = tamanho(listElevator3);
      if (elevator3_moving == false) // se o elevador nao esta andando eu tenho que mandar comando para andar
      {
        if (listSize > 0)
        {
          elevator3_targetFloor = removePos(&listElevator3, 0);
          sendComand(elevator3_targetFloor , 'd', elevator3_floor);
          elevator3_moving = true;
        }
      }
      else
      {
        if ((elevator3_floor >= elevator3_targetFloor && elevator3_way == 's')|| (elevator3_floor <= elevator3_targetFloor && elevator3_way == 'd'))
        {
          UARTprintf("dp\r");
          UARTFlushTx(false);
          UARTprintf("da\r");
          UARTFlushTx(false);
          osDelayUntil(2000000);
          elevator3_moving = false;
        }
        else
        {
          if (listSize > 0)
          {
            if (searchValueAndRemove(&listElevator3, elevator3_floor, elevator3_way, listSize)==1) ///Tentar fazer a comparação somente quando o andar mudar
            {
              UARTprintf("dp\r");
              UARTFlushTx(false);
              UARTprintf("da\r");
              UARTFlushTx(false);
              osDelayUntil(2000000);
              UARTprintf("df\r");
              UARTFlushTx(false);
              UARTprintf("d%s\r",elevator3_way);
            }   
          }
        }
      }
      elevator3ReceiveCommand = false;
    }
  }     
} // thread1

void thread1(void *arg)                                           //Controla o elevador 1
{
  uint8_t listSize= 0;

  while(1)
  {
    if (elevator1ReceiveCommand)
    {
      listSize = tamanho(listElevator1);
      if (elevator1_moving == false) // se o elevador nao esta andando eu tenho que mandar comando para andar
      {
        if (listSize > 0)
        {
          elevator1_targetFloor = removePos(&listElevator1, 0);
          sendComand(elevator1_targetFloor , 'e', elevator1_floor);
          elevator1_moving = true;
        }
      }
      else
      {
        if ((elevator1_floor >= elevator1_targetFloor && elevator1_way == 's')|| (elevator1_floor <= elevator1_targetFloor && elevator1_way == 'd'))
        {
          UARTprintf("ep\r");
          UARTFlushTx(false);
          UARTprintf("ea\r");
          UARTFlushTx(false);
          osDelayUntil(20000);
          elevator1_moving = false;
        }
        else
        {
          if (listSize > 0)
          {
            if (searchValueAndRemove(&listElevator1, elevator1_floor, elevator1_way, listSize)==1) ///Tentar fazer a comparação somente quando o andar mudar
            {
              UARTprintf("ep\r");
              UARTFlushTx(false);
              UARTprintf("ea\r");
              UARTFlushTx(false);
              osDelayUntil(20000);
              UARTprintf("ef\r");
              UARTFlushTx(false);
              UARTprintf("e%s\r",elevator1_way);
            }   
          }
        }
      }
      elevator1ReceiveCommand = false;
    }
  }     
} // thread1





void getCommand()
{
  int floor = 0;
  bool isCommand = false;
  char receiveCode[5];
  UARTgets(receiveCode,sizeof(receiveCode));
  UARTFlushRx();
  switch (receiveCode[0])
  {
            case 'e':
            elevator1ReceiveCommand = true;
            floor = elevator1_floor ;
            break;
            case 'c':
            elevator2ReceiveCommand = true;
            floor = elevator2_floor;
            break;
            case 'd':
            elevator3ReceiveCommand = true;
            floor = elevator3_floor ;
            break;
  }   

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
    isCommand = true;
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
          break;
        } 
        break;
        //fim - Recebimento do botao externo
        //Recebimento do botao interno
 
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
          break;
        }
        break;
    }
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

void thread4(void *arg)                        //Controla a leutura da uart que foi passada para uma thread
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
  thread4_id = osThreadNew(thread4, NULL, NULL);
  thread1_id = osThreadNew(thread1, NULL, NULL);
  thread2_id = osThreadNew(thread2, NULL, NULL);
  thread3_id = osThreadNew(thread3, NULL, NULL);
  if(osKernelGetState() == osKernelReady)
    osKernelStart();
  while(1);
} // main
