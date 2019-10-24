/*
 * Precharge.hpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Minya
 */

#ifndef PRECHARGE_HPP_
#define PRECHARGE_HPP_

#include "Events.hpp"
#include "tim.h"
#include "tinyfsm.hpp"
#include "gpio.h"
#include "stdio.h"
#include "usart.h"
#include "print.h"


class Precharge : public tinyfsm::Fsm<Precharge> {
public:
	void react(tinyfsm::Event const &) {};

	virtual void react(TimerPulse const&) {};
	virtual void react(Fault const&) {};
	virtual void react(StartPrecharge const&) {};

	virtual void entry(void) {};
	void exit(void) {};

};



#endif /* PRECHARGE_HPP_ */
