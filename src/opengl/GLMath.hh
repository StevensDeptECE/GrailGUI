#pragma once

template<typename Precision>
 constexpr Precision PI = 3.14159265358979;

template<typename Precision>
constexpr Precision DEG2RAD = PI<Precision>/180;

template<typename Precision>
constexpr Precision RAD2DEG = 180 / PI<Precision>;

template<typename Precision>
constexpr Precision PI2 = 2*PI<Precision>;

template<typename Precision>
constexpr Precision PI_2 = 0.5*PI<Precision>;
