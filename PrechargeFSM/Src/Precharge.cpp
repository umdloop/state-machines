/*
 * Precharge.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Minya
 */

#include "Precharge.hpp"



// We need to forward declare idle since it's our loopback point
class Idle;

class FaultState : public Precharge{
	void entry() {
		print("ESTOP\n");
	}
};

class OpenK42 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_RESET);
		print("Open K4\n");
		print("Discharged!\n");
		startTimer(50);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<Idle>();
	}
};

class CloseK42 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_SET);
		print("Close K4 \n");
		startTimer(15*1000);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<OpenK42>();
	}
};

class OpenAll2 : public Precharge {
	void entry() {
			HAL_GPIO_WritePin(K1_GPIO_Port, K1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(K2_GPIO_Port, K2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(K3_GPIO_Port, K3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_RESET);
			print("Discharging!\n");
			startTimer(50);
		}

		void react(TimerPulse const&) override {
			HAL_TIM_Base_Stop_IT(&htim2);
			transit<CloseK42>();
		}
};

class OpenK2 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K2_GPIO_Port, K2_Pin, GPIO_PIN_RESET);
		print("Open K2\n");
		print("Precharged!\n");
	}

	void react(Discharge const&) {
		transit<OpenAll2>();
	}
};

class CloseK1 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K1_GPIO_Port, K1_Pin, GPIO_PIN_SET);
		print("Close K1\n");
		startTimer(50);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<OpenK2>();
	}
};

class CloseK3K2 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K3_GPIO_Port, K3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(K2_GPIO_Port, K2_Pin, GPIO_PIN_SET);
		print("Close K3, K2\n");
		startTimer(1000);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<CloseK1>();
	}
};

class OpenK4 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_RESET);
		print("Open K4\n");
		startTimer(50);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<CloseK3K2>();
	}
};

class CloseK4 : public Precharge {
	void entry() {
		HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_SET);
		print("Close K4 \n");
		startTimer(15*1000);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<OpenK4>();
	}
};

class OpenAll : public Precharge {
	void entry() {
		print("Start Precharging!\n");
		HAL_GPIO_WritePin(K1_GPIO_Port, K1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(K2_GPIO_Port, K2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(K3_GPIO_Port, K3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(K4_GPIO_Port, K4_Pin, GPIO_PIN_RESET);
		print("Open K1, K2, K3, K4\n");
		startTimer(50);
	}

	void react(TimerPulse const&) override {
		HAL_TIM_Base_Stop_IT(&htim2);
		transit<CloseK4>();
	}
};

class Idle : public Precharge {
	void entry() override {
		print("Idle\n");
	}
	void react(StartPrecharge const&) override {
		transit<OpenAll>();
	}
	void react(Discharge const&) {
			transit<OpenAll2>();
	}
};

void Precharge::react(const Fault&) {
	transit<FaultState>();
}

FSM_INITIAL_STATE(Precharge, Idle);
