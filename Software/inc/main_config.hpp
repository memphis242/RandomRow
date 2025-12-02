/**
 * @file main_config.hpp
 * @brief Configuration of the top-level software parameters of the RandomRow.
 * 
 * @author Abdulla Almosalami (@memphis242)
 * @date June 03, 2025
 * @copyright MIT License
 */
#pragma once

#include "types.hpp"

/* Public constexpr Definitions */

namespace RandomRow::Cfg
{
   constexpr ms_t RERUN_RNG_DEBOUNCE_TIME = 5000;
   constexpr ms_t GO_TO_SLEEP_WAIT_TIME   = 1000;
   constexpr ms_t SLEEP_CYCLE_TIME        = 1000;
};

// #define KNOB_PORT TODO
