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

// UART definitions
extern void UARTStdioIntHandler(void);

//threads
osThreadId_t thread1_id, thread2_id, thread3_id, thread5_id;

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

uint8_t queue_elevator1_count =0;
uint8_t queue_elevator2_count =0;
uint8_t queue_elevator3_count =0;

osMessageQueueId_t elevator1_queue;
osMessageQueueId_t elevator2_queue;
osMessageQueueId_t elevator3_queue;




void sendComand(uint8_t floor_called, char elevator, uint8_t floor_now)
{
  char message = 'f';
  if (floor_called!=floor_now)                               // verifica se o elevador chamado não esta no mesmo andar
  {
    UARTprintf("%s%s",elevator,message);//fecha a porta do elevador
    if (floor_called>floor_now)
      message = 's';
    else
      message = 'd';
    UARTprintf("%s%s",elevator,message);
    UARTFlushTx(false);
  }
}


void thread1(void *arg)                        //Controla o elevador 1
{
  uint8_t count_queue = 0;
  while(1)
  {
      uint8_t msg;
      count_queue = osMessageQueueGetCount(elevator1_queue);
      if (elevator1_moving == false && count_queue != 0)
      {
        osMessageQueueGet(elevator1_queue, &msg, NULL, 0U);
        sendComand(msg, 'e', elevator1_floor);
        elevator1_moving = true;
      }
  }     
} // thread1

void sendCommandToElevatorQueueQueueExternal( void *command, uint8_t elevator_number)
{
  char *receiveCommand;
  receiveCommand = command;
  uint8_t floor;
  if (*receiveCommand == 'I')
  {
    
  }
}
void sendCommandToElevatorQueueInternal( void *command, uint8_t elevator_number)
{
  char *receiveCommand;
  receiveCommand = command;
  uint8_t floor;
  switch (*receiveCommand)
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
   } // end of switch
   //PushToList()
}

void secondCharacter(void *command, uint8_t elevator_number)
{
  char *receiveCommand;
  receiveCommand = command;
  if (*receiveCommand == 'I')
  {
      sendCommandToElevatorQueueInternal(receiveCommand+0x1, elevator_number);
  }
  else
  {
    if(*receiveCommand == 'E')
    {
      //sendCommandToElevatorQueueExternal(receiveCommand+0x1, elevator_number);
    }
    else
    {
      switch (*receiveCommand)
      {
        case '0':
          //verifica se tem que parar
        break;
        case '1':
          //verifica se tem que parar
        break;
        case '2':
        break;
        case '3':
        break;
        case '4':
        break;
        case '5':
        break;
        case '6':
        break;
        case '7':
        break;
        case '8':
        break;
        case '9':
        break;
      }
    }
  }
 // int floor = 5;
  //osMessageQueueGet(elevator1_queue, &floor, NULL, 0U);
}



void decodeCommand(void *command)
{
  char *receiveCommand;
  receiveCommand = command;
  switch (*receiveCommand)
  {
    case 'e':
    secondCharacter(receiveCommand +0x1, 1);
    break;
    case 'c':
    break;
    case 'd':
    break;
  }
}

void getCommand()
{
  char receiveCode[5];
  UARTgets(receiveCode,6);
  UARTFlushRx();
  decodeCommand(&receiveCode);
}

void thread5(void *arg)                        //Controla o elevador 1
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

  elevator1_queue = osMessageQueueNew(queue_elevator1_count, queue_elevator1_size, NULL);
  elevator2_queue = osMessageQueueNew(queue_elevator2_count, queue_elevator2_size, NULL);
  elevator3_queue = osMessageQueueNew(queue_elevator3_count, queue_elevator3_size, NULL);
  //char receiveCode[5] = {'e','s'};
  //decodeCommand(receiveCode);
  
  thread1_id = osThreadNew(thread1, NULL, NULL);
  thread5_id = osThreadNew(thread5, NULL, NULL);
  if(osKernelGetState() == osKernelReady)
    osKernelStart();
  //Teste decodificador

  
  while(1);
} // main
