/**
 * @file main.c
 * @brief Top-level superloop file that manages the RandomRow software behavior.
 *
 * @author Abdulla Almosalami (memphis242)
 * @date June 3, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include <cstdint>
#include <limits>
#include <assert>

#include "main_config.h"

#include "hal.hpp"
#include "rng.hpp"
#include "rntracker.hpp"
#include "display.hpp"
#include "inputdriver.hpp"
#include "outputdriver.hpp"
#include "nvm.hpp"
#include "diagnostics.hpp"
#include "generic_timer.hpp"

/* Local Macro Definitions */
constexpr std::uint8_t MAX_INIT_ATTEMPTS = std::numeric_limits<std::uint8_t>::max();

// Constant-like macros

// Function-like macros

/* Local Datatypes */

/* Local Data */

/* Private Function Prototypes */

/* Main Logic */
int main(void)
{

   /* Initialization */

   // Base Hardware
   HAL::PowerModes::Set( HAL::PowerModes::NormalRuntime /* TODO */ );
   HAL::Clock fcpu(1'000'000);
   HAL::SPI spi_nvm( // FYI: "nvm" == "non-volatile memory" (i.e., EEPROM, FRAM, FEE, ...)
               HAL::SPI::Ports::SPI1,
               100'000, // SCK rate
               HAL::GPIO(  // CS
                     HAL::GPIO::Direction::Out,
                     HAL::GPIO::Ports::D1
                     /* TODO */
                  ),
               /* TODO */
            );
   HAL::Timer tmrGoToSleep(
         HAL::Timer::Units::ms,
         std::numeric_limits<std::uint16_t>::max()
      );

   // Middleware
   Middleware::RNG rng(Middleware::RNG::TRNG);
   Middleware::OutputDriver::Init(/* TODO */);
   Middleware::NVM nvm(spi_nvm);

   // External Devices
   Devices::UXButton genButton(
               Middleware::InputDriver(
                     Middleware::InputDriver::Types::Digital,
                     HAL::GPIO(
                           HAL::GPIO::Direction::In,
                           HAL::GPIO::Ports::A5
                           /* TODO */
                        )
                     ),
               200 // ms, debounce time
         );
   Devices::Display display( /* TODO */ );
   Devices::Knob knobAdjRngs(
               Middleware::InputDriver(
                     Middleware::InputDriver::Types::Analog,
                     HAL::ADC(
                           HAL::ADC::Ports::ADC1
                           /* TODO */
                        )
                     ),
         );

   // Internal Software Modules
   Diagnostics diagnostics();
   RNT rnt();
   rng.setRange(rnt.min(), rnt.max());

   /* Local Persistent Data */
   static bool NewNumGenerated = false;

   /* Local Auto-Data */
   /* (basically persistent though, because we'll be in the while(1) inf loop) */
   uint16_t random_num = 0;

   enum MainFSM_E
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

   /* Superloop */
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

            if ( !UXButton_Held() && button_pressed )
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

#ifdef DESKTOP_ENV
      // TODO: Exit while(1) infinite loop
#endif
   }

	return 0;
}
