#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <memory>


// CHANGE TO DESIRE BELOW ---
constexpr int MAX_DIGITS = 5;   // Number of places in the form: xx,xxx.
constexpr int RANGE = 99999;     // 9999 to adhere to 4 place digits, 99999 to adhere to 5 place digits, and so on.
// CHANGE TO DESIRE ABOVE ---
const int MAX_PLACE = static_cast<int>(pow(10, (MAX_DIGITS - 1)));  // Smallest power of 10 that has the number of digits specified by MAX_DIGITS. For 5 place digits: 10^(4).
