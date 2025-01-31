
// Niekas neveike, kol i nRF_Drivers folderi neikeliau radio_config.c failo. Dabar viskas veikia kaip ir turetu.


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "nrf_drv_systick.h"
#include "nrf_delay.h"
#include "boards.h"

#include "nrf_drv_clock.h"

#include "radio_config.h"

static uint32_t                   packet;           //   < Packet to transmit. 

uint32_t read_packet()
{
    uint32_t result = 0;

    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_RXEN = 1U;     // Enable radio and wait for ready
    while (NRF_RADIO->EVENTS_READY == 0U) ;
    NRF_RADIO->EVENTS_END = 0U;
    NRF_RADIO->TASKS_START = 1U;  // Start listening and wait for address received event
    while (NRF_RADIO->EVENTS_END == 0U) ; // Wait for end of packet or buttons state changed

    if (NRF_RADIO->CRCSTATUS == 1U) { result = packet; }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
    while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 
    return result;
}


void LED_Control(uint8_t led_idx)
{
  switch(led_idx)
  {
    case 0: 

    bsp_board_led_on(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);

    break; 

    case 1: 

    bsp_board_led_off(0);
    bsp_board_led_on(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);


    break;


    case 2: 

    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_on(2);
    bsp_board_led_off(3);


    break;

    case 3: 

    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_on(3);

    break;

  }


}

int main(void)
{
  bsp_board_init(BSP_INIT_LEDS);

  ///start clock set mandatory for USB CDC
  nrf_drv_clock_init(); 
  nrf_drv_clock_hfclk_request(NULL); // for HF 32MHz external X-tal
  while(!nrf_drv_clock_hfclk_is_running()) ; // Just waiting
  nrf_drv_clock_lfclk_request(NULL); // for LF 32.768kHz external X-tal
  while(!nrf_drv_clock_lfclk_is_running()) ; 
  /// end clock set

  /// for radio receiver
 radio_configure();
  NRF_RADIO->PACKETPTR = (uint32_t)&packet;


  while(true)
  {
        uint32_t received = read_packet(); // received data from RADIO

        LED_Control(received-1); 

  }
}



/*
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "boards.h"
#include "radio_config.h" // for radio
#include "nrf_drv_clock.h"

#include "nrf_delay.h"

static uint32_t                   packet = 0; 

void send_packet()
{
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;
    while (NRF_RADIO->EVENTS_READY == 0U) ; 

    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;
    while (NRF_RADIO->EVENTS_END == 0U) ;

    NRF_RADIO->EVENTS_DISABLED = 0U;
    NRF_RADIO->TASKS_DISABLE = 1U;     // Disable radio
    while (NRF_RADIO->EVENTS_DISABLED == 0U) ; 

}

void LED_Control(uint8_t led_idx)
{
  switch(led_idx)
  {
    case 0: 

    bsp_board_led_on(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);

    break; 

    case 1: 

    bsp_board_led_off(0);
    bsp_board_led_on(1);
    bsp_board_led_off(2);
    bsp_board_led_off(3);


    break;


    case 2: 

    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_on(2);
    bsp_board_led_off(3);


    break;

    case 3: 

    bsp_board_led_off(0);
    bsp_board_led_off(1);
    bsp_board_led_off(2);
    bsp_board_led_on(3);


    break;

  }
}

int main(void)
{

  bsp_board_init(BSP_INIT_LEDS);

  nrf_drv_clock_init(); 
  nrf_drv_clock_hfclk_request(NULL); // for HF 32MHz external X-tal
  while(!nrf_drv_clock_hfclk_is_running()) ; // Just waiting
  nrf_drv_clock_lfclk_request(NULL); // for LF 32.768kHz external X-tal
  while(!nrf_drv_clock_lfclk_is_running()) ; // Just waiting

  radio_configure();
  NRF_RADIO->PACKETPTR = (uint32_t)&packet;   // Set payload pointer

  while(true)
  {

    LED_Control(packet-1);    
    send_packet();
    packet++; if(packet > 4) packet = 1;
    nrf_delay_ms(500);
  }
}
*/