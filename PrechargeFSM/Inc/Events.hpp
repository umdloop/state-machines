/*
 * Events.hpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Minya
 */

#include "tinyfsm.hpp"

#ifndef EVENTS_HPP_
#define EVENTS_HPP_

struct PrechargeEvent : tinyfsm::Event {};

struct StartPrecharge : PrechargeEvent {};
struct TimerPulse : PrechargeEvent {};
struct Discharge : PrechargeEvent {};
struct Fault : PrechargeEvent {};


#endif /* EVENTS_HPP_ */
