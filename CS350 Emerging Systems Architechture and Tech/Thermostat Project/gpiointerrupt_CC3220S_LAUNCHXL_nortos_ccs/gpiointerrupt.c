/*
 *  ======== gpiointerrupt.c ========
 */

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <uart.h>
#include <i2c.h>
#include <timer.h>

//#include "wifi_library.h" // Include the Wi-Fi library header

/* Driver configuration */
#include <ti_drivers_config.h>

/* Init buttons before callback */
int leftButton = 0;                     // Left button for decrement
int rightButton = 0;                    // Right button for increment

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    /* Button function */
    leftButton = 1;
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    /* Button function */
    rightButton = 1;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();

    /* Configure the red LED */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Configure BUTTON0 pin */
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1) {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);

        /* Enable interrupts */
        GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
    }

    /*
     *  ======== Main code ========
     */

    /*
     * Add flags (similar to timer flag) to button handlers
     * Refer to Zybooks - "Converting different-period tasks to C"
     * Remember to configure timer period (currently 100ms, or 100000us)
     */

    /* Call driver init functions */
    initUART();
    initI2C();
    initTimer();

    /* Init variables*/
    unsigned long buttonCheckTime = 0;                              // elapsed time for button
    unsigned long tempCheckTime = 0;                                // elapsed time for temp
    unsigned long displayCheckTime = 0;                             // elapsed time for display
    const unsigned long buttonCheckPeriod = 200;                    // constant button period (200ms)
    const unsigned long tempCheckPeriod = 500;                      // constant temp period (500ms)
    const unsigned long displayCheckPeriod = 1000;                  // constant display period (1000ms)
    const unsigned long timerPeriod = 100;                          // constant timer period/GCD (100ms)
    int setpoint = 25;                                              // setpoint (default thermostat value)
    int heat = 0;                                                   // heat (0 is off, 1 is on)
    int seconds = 0;                                                // seconds
    int temperature = 0;                                            // temperature



    /*
     *  Task Scheduler
     */
    while (1) {
        readTemp();  // temp changes

        // Check button 200ms
        if (buttonCheckTime >= buttonCheckPeriod) {
            if (rightButton == 1) {
                setpoint += 1;
                rightButton = 0;
            }
            if (leftButton == 1) {
                setpoint -= 1;
                leftButton = 0;
            }
        }

        // Check temperature update LED/heat 500ms
        if (tempCheckTime >= tempCheckPeriod) {
            temperature = readTemp();
            if (temperature < setpoint) {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                heat = 1;
            } else {
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                heat = 0;
            }
        }

        // Output to UART to display temperature 1000ms
        if (displayCheckTime >= displayCheckPeriod) {
            DISPLAY(snprintf(output, 64, "<%02d,%02d,%d,%04d>\n\r", temperature, setpoint, heat, seconds));
            ++seconds;
        }

        // Wait for the timer period
        while (!TimerFlag) {}
        TimerFlag = 0;

        // Increment time counters
        buttonCheckTime += timerPeriod;
        tempCheckTime += timerPeriod;
        displayCheckTime += timerPeriod;
    }
}

/*
 *  ======== WiFi/Cloud Connectivity ========


void connectToWiFi() {
    // Initialize Wi-Fi module
    WiFi_Init();

    // Configure Wi-Fi parameters (SSID, password, etc.)
    WiFi_Setup("your_ssid", "your_password");

    // Connect to Wi-Fi network
    if (WiFi_Connect()) {
        // Wi-Fi connection successful
        DISPLAY(snprintf(output, 64, "Connected to Wi-Fi\n\r"));
    } else {
        // Wi-Fi connection failed
        DISPLAY(snprintf(output, 64, "Failed to connect to Wi-Fi\n\r"));
        while (1) {}
    }

    // Connect to the cloud or server
    if (Cloud_Connect()) {
        // Cloud connection successful
        DISPLAY(snprintf(output, 64, "Connected to cloud\n\r"));
    } else {
        // Cloud connection failed
        DISPLAY(snprintf(output, 64, "Failed to connect to cloud\n\r"));
        while (1) {}
    }
}

*/



























