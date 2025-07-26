/**
 * @file main.c
 * @brief Top-level superloop file that manages the RandomRow software behavior.
 *
 * @author Abdulla Almosalami (memphis242)
 * @date June 3, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "main_config.h"
#include "mcu_hal.h"
#include "rng.h"
#include "rntracker.h"
#include "display.h"
#include "inputdriver.h"
#include "outputdriver.h"
#include "nvm.h"
#include "diagnostics.h"
#include "generic_timer.h"

/* Local Macro Definitions */
#define MAX_INIT_ATTEMPTS UINT8_MAX

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
   Display_HardwareInit();
   InputDriver_HardwareInit();
   UXButton_Init();
   OutputDriver_Init();
   NVM_Init();

   // Software
   Diagniostics_Init();

   RandomNumberTracker_Init();
   uint16_t min = RandomNumberTracker_RangeGetMin();
   uint16_t max = RandomNumberTracker_RangeGetMax();
   RNG_SetRange(min, max);

   static Timer_T Timer_RNGDebounce;
   static Timer_T Timer_GoToSleep;
   // Initialize local timers
   Timer_Init( &Timer_RNGDebounce, Timer_ms, UINT16_MAX );
   Timer_Init( &Timer_GoToSleep,   Timer_ms, UINT16_MAX );

   Input_T knob;
   InputDriver_Init( &knob,
                     InputDriver_Category_Analog,
                     InputDriver_Unit_Percent,
                     KNOB_PORT );

   /* Local Persistent Data */
   static bool NewNumGenerated = false;

   /* Local Auto-Data */
   /* (basically persistent though, because we'll be in the while(1) inf loop) */
   uint16_t random_num = 0;

   static enum MainFSM_E
   {
      Init,
      GenerateRandomNumber,
      DisplayRandomNumber,
      StoreRandomNumber,
      ReconfigureRandomNumberRange,
      Idle,
      Sleep,
      SevereFault
   } MainFSM = Init;

   /* Core Logic */
   while(1)
   {
      Diagnostics_Check();
      if ( Diagnostics_SevereFaultPresent() )
      {
         MainFSM = Idle;
      }

      // Calling UXButton_ButtonHasBeenPressed() will reset the associated flag
      // within the UXButton module. This is an intentional design pattern that
      // better suits the superloop design.
      // So, to properly use this, the return value is stored and used as needed.
      bool button_pressed = UXButton_ButtonHasBeenPressed();

      switch(MainFSM)
      {
         /*********************************************************************/
         case Init:
         
            random_num = RandomNumberTracker_GetMostRecentNum();
            MainFSM = DisplayRandomNumber;

            break;

         /*********************************************************************/
         case GenerateRandomNumber:

            Timer_RNGDebounce = 0;
            do { random_num = RNG_GetRandomNum(); }
            while ( RandomNumberTracker_AlreadyUsed(random_num) );
            NewNumGenerated = true;
            MainFSM = DisplayRandomNumber;

            break;

         /*********************************************************************/
         case DisplayRandomNumber:
         {
            // Local case-scope variables
            static bool StatePreviouslyExited_DisplayRandomNumber = true;

            if ( StatePreviouslyExited_DisplayRandomNumber )
            {
               // On-entry-only statements
               Display_Num(random_num);
               StatePreviouslyExited_StoreRandomNumber = false;
            }
            else
            {
               if ( Display_Complete() )
               {
                  MainFSM = NewNumGenerated ? StoreRandomNumber : Idle;
                  StatePreviouslyExited_DisplayRandomNumber = true;
               }
            }

            break;
         }

         /*********************************************************************/
         case StoreRandomNumber:
         {
            // Local case-scope variables
            static bool StatePreviouslyExited_StoreRandomNumber = true;

            assert( NewNumGenerated );

            if ( StatePreviouslyExited_StoreRandomNumber )
            {
               RandomNumberTracker_Store(random_num);
               StatePreviouslyExited_StoreRandomNumber = false;
            }
            else
            {
               if ( RandomNumberTracker_StoreComplete() )
               {
                  NewNumGenerated = false;
                  MainFSM = Idle;
                  StatePreviouslyExited_StoreRandomNumber = true;
               }
            }

            break;
         }

         /*********************************************************************/
         case ReconfigureRandomNumberRange:
         {
            static bool FirstStep = true;
            static uint8_t new_min = 0;
            static uint8_t new_max = DISPLAY_MAX_NUM;
            static bool max_valid = false;

            uint16_t num_selection =
               (uint16_t)(InputDriver_GetPercent(&knob) * 100.0f) *
               DISPLAY_MAX_NUM;
            Display_Num(num_selection);

            if ( FirstStep && button_pressed )
            {
               new_min = num_selection;
               RNG_SetMin(new_min);
               FirstStep = false;
            }
            else if ( !FirstStep && !max_valid && button_pressed )
            {
               if ( num_selection >= new_min )
               {
                  new_max = num_selection;
                  max_valid = true;
                  RNG_SetMax(new_max);
               }
               else
               {
                  max_valid = false;
                  Display_String("ERR");
               }
               FirstStep = true;
               MainFSM = Idle;
            }
            else
            {
               // Continue waiting in this state...
            }

            break;
         }

         /*********************************************************************/
         case Sleep:

            if ( WakeUpActive() )
            {
               MainFSM = Idle;
            }
            else
            {
               // The idea right now here is Sleep() puts the MCU to sleep for a
               // few seconds, let it wake itself up, execute an iteration of
               // the main loop, and end up back here again if no wakeup is
               // active.
               Sleep(SLEEP_CYCLE_TIME);
            }

            break;

         /*********************************************************************/
         case SevereFault:

            Display("ERR"); // Will also fail if the fault is in the display drive

            if ( !Diagnostics_SevereFaultPresent() )
            {
               MainFSM = Idle;
            }
            
            break;

         /*********************************************************************/
         case Idle:
            // Fallthrough
         default:

            if ( !UXButton_Held() && button_pressed &&
                 (Timer_GetTime(&Timer_RNGDebounce) >= RERUN_RNG_DEBOUNCE_TIME) )
            {
               MainFSM = GenerateRandomNumber;
            }
            else if ( UXButton_Held() )
            {
               MainFSM = ReconfigureRandomNumberRange;
            }
            else
            {
               Timer_Increment(&Timer_GoToSleep);
               if ( Timer_GetTime(&Timer_GoToSleep) >= GO_TO_SLEEP_WAIT_TIME )
               {
                  MainFSM = Sleep;
               }
            }

            break;
      }

      Timer_Increment(&Timer_RNGDebounce);

#ifdef DESKTOP_ENV
      // TODO: Exit while(1) infinite loop
#endif
   }

	return 0;
}
