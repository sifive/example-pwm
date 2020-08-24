/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/pwm.h>
#include <metal/io.h>
#include <metal/uart.h>
#include <stdio.h>
#include <time.h>

/* Return values */
#define RET_OK 0
#define RET_NOK 1

/* 1s delay macro */
#define WAIT_1S(timeout)                                                       \
  timeout = time(NULL) + 1;                                                    \
  while (timeout > time(NULL))                                                 \
    ;

 /* To enable PWM run in interrupt mode.*/
//#define ENABLE_INTERRUPTS

#ifdef metal_pwm_1
#define pwm metal_pwm_1
#else
#ifdef metal_pwm_0
#define pwm metal_pwm_0
#endif
#endif

#ifdef ENABLE_INTERRUPTS

void metal_sifive_pwm0_source0_handler(void)
{
    static unsigned int i = 0;

    /* Print interrupt ID and ISR run count
     * Note: Not recommended, but we have nothing else to do here.*/
    printf("id:%u i:%u\n", 0, i++);

    metal_pwm_clear_interrupt(pwm, 0);
}

void pwm_isr1(int pwm_id, void *data) {
    static unsigned int i = 0;

    /* Print interrupt ID and ISR run count
     * Note: Not recommended, but we have nothing else to do here.*/
    printf("id:%u i:%u\n", 1, i++);

    metal_pwm_clear_interrupt(pwm, 1);
}
#endif

int main(void) {
    printf("%s %s \n", __DATE__, __TIME__);
    printf("PWM demo test..\n");

#ifdef pwm
    time_t timeout;

    /* Enable PWM1 instance */
    metal_pwm_enable(pwm);

    /* Set PWM1 to 1Hz */
    metal_pwm_set_freq(pwm, 0, 1);

    /* Set Duty cycle and phase correct mode */
    metal_pwm_set_duty(pwm, 1, 90, METAL_PWM_PHASE_CORRECT_DISABLE);

    /* Start in continuous mode */
    metal_pwm_trigger(pwm, 0, METAL_PWM_CONTINUOUS);

#ifdef ENABLE_INTERRUPTS
    metal_pwm_cfg_interrupt(pwm, METAL_PWM_INTERRUPT_ENABLE);
#else
    metal_pwm_set_duty(pwm, 2, 90, METAL_PWM_PHASE_CORRECT_DISABLE);
    metal_pwm_set_duty(pwm, 3, 20, METAL_PWM_PHASE_CORRECT_DISABLE);
#endif

    /* Loop forever */
    while (1) {
        /* Wait 1s */
        WAIT_1S(timeout);
    }

    return RET_OK;
#else
    printf("PWM not present\n");
    return RET_NOK;
#endif
}
