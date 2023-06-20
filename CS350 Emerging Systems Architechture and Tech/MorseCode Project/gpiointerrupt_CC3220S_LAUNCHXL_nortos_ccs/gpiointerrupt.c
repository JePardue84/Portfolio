/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
*/

#include <ti/drivers/Timer.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"

static Timer_Handle timer;
static bool isSOS = true; // Flag to indicate the current message
static bool isButtonPressed = false; // Flag to indicate if the side button is pressed

void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    static int messageIndexSOS = 0;
    static const char *messageSOS = "...---...";
    static int messageLengthSOS = 11;
    static int dotCountSOS = 0;
    static bool isBlinkingSOS = false;

    static int messageIndexOK = 0;
    static const char *messageOK = "---.-";
    static int messageLengthOK = 5;
    static int dotCountOK = 0;
    static bool isBlinkingOK = false;

    if (isButtonPressed) {
        if (isSOS) {
            if (messageIndexSOS >= messageLengthSOS) {
                messageIndexSOS = 0;
                dotCountSOS = 0;
                isBlinkingSOS = false;
                return;
            }

            char currentChar = messageSOS[messageIndexSOS];

            if (isBlinkingSOS) {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
                isBlinkingSOS = false;
            } else {
                if (currentChar == '.') {
                    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Red LED (dots)
                    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
                } else if (currentChar == '-') {
                    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON); // Green LED (dashes)
                }

                dotCountSOS++;

                if (dotCountSOS == 3) {
                    messageIndexSOS++;
                    dotCountSOS = 0;
                    isBlinkingSOS = true;

                    if (messageIndexSOS == messageLengthSOS) {
                        // Inter-character delay for SOS
                        usleep(1000000); // 1 second delay
                        isSOS = false; // Switch to "OK" message
                    }
                }
            }
        } else {
            if (messageIndexOK >= messageLengthOK) {
                messageIndexOK = 0;
                dotCountOK = 0;
                isBlinkingOK = false;
            }

            char currentChar = messageOK[messageIndexOK];

            if (isBlinkingOK) {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
                isBlinkingOK = false;
            } else {
                if (currentChar == '.') {
                    if (dotCountOK < 4) {
                        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                        GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON); // Green LED (dashes)
                        usleep(10000); // 100 ms delay (faster green blink)
                    } else if (dotCountOK == 4) {
                        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Red LED (dots)
                        GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
                        usleep(200000); // 200 ms delay (red blink)
                    } else if (dotCountOK == 5) {
                        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Turn off red LED
                        GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON); // Green LED (dashes)
                        usleep(10000); // 100 ms delay (faster green blink)
                    }
                } else if (currentChar == '-') {
                    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON); // Green LED (dashes)
                    usleep(1000);// 600 ms delay (same as SOS)
                }

                dotCountOK++;

                if (dotCountOK == 6) {
                    messageIndexOK++;
                    dotCountOK = 0;
                    isBlinkingOK = true;
                }
            }
        }
    } else {
        // Side button not pressed, output SOS
        if (messageIndexSOS >= messageLengthSOS) {
            messageIndexSOS = 0;
            dotCountSOS = 0;
            isBlinkingSOS = false;
            return;
        }

        char currentChar = messageSOS[messageIndexSOS];

        if (isBlinkingSOS) {
            GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
            GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
            isBlinkingSOS = false;
        } else {
            if (currentChar == '.') {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON); // Red LED (dots)
                GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_OFF); // Green LED (dashes)
            } else if (currentChar == '-') {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF); // Red LED (dots)
                GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON); // Green LED (dashes)
            }

            dotCountSOS++;

            if (dotCountSOS == 3) {
                messageIndexSOS++;
                dotCountSOS = 0;
                isBlinkingSOS = true;

                if (messageIndexSOS == messageLengthSOS) {
                    // Inter-character delay for SOS
                    usleep(1000000); // 1 second delay
                }
            }
        }
    }
}

void buttonInterruptHandler(uint_least8_t index)
{
    isButtonPressed = !isButtonPressed;
    if (!isButtonPressed) {
        isSOS = true; // Reset to SOS message when button is released
    }
}

void initTimer(void)
{
    Timer_Params params;

    Timer_init();
    Timer_Params_init(&params);

    params.period = 200000; // 200 ms (blink faster)
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer = Timer_open(CONFIG_TIMER_0, &params);

    if (timer == NULL) {
        while (1) {}
    }

    if (Timer_start(timer) == Timer_STATUS_ERROR) {
        while (1) {}
    }
}

void *mainThread(void *arg0)
{
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); // Red LED (dots)
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); // Green LED (dashes)

    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, buttonInterruptHandler);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    initTimer();

    while (1) {}

    return NULL;
}



