/**
 * @file main.c
 * @brief Top-level superloop file that manages the RandomRow software behavior.
 *
 * @author Abdulla Almosalami (memphis242)
 * @date June 3, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include "main_config.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "ascii7seg.h"
#include "ascii7seg_config.h"
#include "mcu_hal.h"
#include "set.h"

/* Local Macro Definitions */

// Constant-like macros

// Function-like macros

/* Local Datatypes */

/* Local Data */

/* Private Function Prototypes */

/* Main Logic */
int main(void)
{
   /* Initialization */
   // Hardware
   MCU_Init();
   RNG_Init();
   Display_Init();
   InputDriver_Init();
   UXButton_Init();
   OutputDriver_Init();
   NVM_Init();
   // Software
   Diagniostics_Init();
   RandomNumberTracker_Init();
   uint16_t min = RandomNumberTracker_RangeGetMin();
   uint16_t max = RandomNumberTracker_RangeGetMax();
   RNG_SetRange(min, max);

   /* Local Persistent Data */
   static Timer_T rng_debounce_timer;
   static Timer_T go_to_sleep_timer;
   Timer_Init(&rng_debounce_timer, Timer_ms, UINT16_MAX);
   Timer_Init(&go_to_sleep_timer,  Timer_ms, UINT16_MAX);

   static uint16_t random_num = 0;
   static bool new_num_generated = false;
   static bool state_entry = true;

   static enum MainFSM_E
   {
      Init,
      GenerateRandomNumber,
      DisplayRandomNumber,
      StoreRandomNumber,
      ReconfigureRandomNumberRange,
      Idle,
      Sleep
   } MainFSM = Init;

   /* Core Logic */
   while(1)
   {
      Diagnostics_Check();
      if ( Diagnostics_SevereFaultPresent() )
      {
         MainFSM = Idle;
      }

      switch(MainFSM)
      {
         /*********************************************************************/
         case Init:
         
            random_num = RandomNumberTracker_GetMostRecentNum();
            new_num_generated = true;
            MainFSM = DisplayRandomNumber;

            break;

         /*********************************************************************/
         case GenerateRandomNumber:
            rng_debounce_timer = 0;
            do
            {
               random_num = RNG_GetRandomNum();
            } while ( RandomNumberTracker_AlreadyUsed(random_num) );
            new_num_generated = true;
            MainFSM = DisplayRandomNumber;

            break;

         /*********************************************************************/
         case DisplayRandomNumber:
            // TODO
            if ( state_entry )
            {
               // On-entry...
            }

            // when done...
            if ( false /* TODO */ )
            {
               MainFSM = StoreRandomNumber;
            }

            break;

         /*********************************************************************/
         case StoreRandomNumber:
            assert( new_num_generated );

            if ( state_entry )
            {
               state_entry = false; // FIXME: State entry var pattern
               RandomNumberTracker_Store(num);
            }
            // TODO

            // when done...
            if ( false /* TODO */ )
            {
               new_num_generated = false;
               MainFSM = Idle;
            }

            break;

         /*********************************************************************/
         case ReconfigureRandomNumberRange:
            // TODO

            break;

         /*********************************************************************/
         case Sleep:
            // TODO
            if ( WakeUp() )
            {
               MainFSM = Idle;
            }

            break;

         /*********************************************************************/
         case Idle:
            // Fallthrough
         default:
            if ( Diagnostics_SevereFaultPresent() )
            {
               // TODO
               // Display "ERR" if possible
            }
            else if ( !UXButton_Held() && UXButton_ButtonHasBeenPressed() &&
                      (Timer_GetTime(&rng_deboucne_timer) >= RERUN_RNG_DEBOUNCE_TIME) )
            {
               MainFSM = GenerateRandomNumber;
            }
            else if ( UXButton_Held() )
            {
               MainFSM = ReconfigureRandomNumberRange;
            }
            else
            {
               Timer_Increment(&go_to_sleep_timer);
               if ( Timer_GetTime(&go_to_sleep_timer) >= GO_TO_SLEEP_WAIT_TIME )
               {
                  MainFSM = Sleep;
               }
            }

            break;
      }

      Timer_Increment(&rng_debounce_timer);
   }

	return 0;
}
