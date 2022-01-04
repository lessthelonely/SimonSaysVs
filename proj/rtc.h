#ifndef RTC_H
#define RTC_H

#include "leaderboard.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions for handling rtc
 */

#define RTC_ADDR_REG 0x70 /**< @brief RTC address register. */
#define RTC_DATA_REG 0x71 /**< @brief RTC data register. */
#define RTC_IRQ 8 /**< @brief RTC IRQ line 8. */

#define SECONDS_REG 0 /**< @brief Seconds register. */
#define SECONDS_ALARM 1 /**< @brief  Seconds alarm register.*/
#define MINUTES_ALARM 3 /**< @brief  Minutes alarm register.*/
#define HOURS_ALARM 5 /**< @brief  Hours alarm register.*/
#define DAY_REG 7 /**< @brief  Day register.*/
#define MONTH_REG 8 /**< @brief  Month register.*/
#define YEAR_REG 9 /**< @brief  Year register.*/
#define REGISTER_A 10 /**< @brief  Control/Status Register A.*/
#define REGISTER_B 11 /**< @brief  Control/Status Register B.*/
#define REGISTER_C 12 /**< @brief  Control/Status Register C.*/

//Register A
#define UIP 7 /**< @brief Update In Progress Flag. */

//Register B
#define UIE 4 /**< @brief  Enable Update Interruptions.*/
#define AIE 5 /**< @brief  Enable Alarm Interruptions.*/
#define SET 7 /**< @brief  Set to 1 to inhibit updates of time/date registers.*/

//Register C
#define PF 6   /**< @brief  Periodic Interrupt Pending.*/
#define AF 5   /**< @brief  Alarm Interrupt Pending.*/
#define UF 4   /**< @brief  Update Interrupt Pending.*/

/**
 *@ brief Types of RTC interruptions.
*/
typedef enum
{
    PERIODIC,
    ALARM,
    UPDATE,
    RTC_NO_INT
} rtc_int_t;

/**
 * @brief A struct to save a date.
 */
struct date
{
    /**
	 * @brief The year
	*/
    int year;

    /**
	 * @brief The month
	*/
    int month;

    /**
	 * @brief The day
	*/
    int day;
};

/**
 * @brief Subscribes rtc interruptions for the rtc alarm
 * @param bit_no Return argument with the mask bit
 * @param notification Return argument with the notification address
 */
void rtc_alarm_subscribe(uint8_t *bit_no, int *notification);

/**
 * @brief Unsubscribes rtc interruptions for the rtc alarm
 * @param notification The correspondant rtc notification address
 */
void rtc_alarm_unsubscribe(int *notification);

/**
 * @brief Subscribes rtc interruptions
 * @param bit_no Return argument with the mask bit
 * @param notification Return argument with the notification address
 */
int rtc_subscribe(uint8_t *bit_no, int *notification);

/** 
 * @brief Unsubscribes rtc interruptions
 * @param notification The correspondant rtc notification address
 */
void rtc_unsubscribe(int *notification);

/**
 * @brief Enable register updates
 */
void enable_updates();

/**
 * @brief Disable register updates
 */
void disable_updates();

/**
 * @brief Handles the rtc interruptions
 * @details Clears the register_c
 * @return The type of interrupt
 */
rtc_int_t rtc_handler();

/**
 * @brief Returns a date through the date struct.
 * @details Check leaderboard.h or the data structures page.
 * @return The date struct we defined.
*/
struct date get_date();

/**
 * @brief Reads a date from the rtc
 * @param new_date The date to be read
 */
bool read_date(struct date *new_date);

/**
 * @brief Sets an alarm that is activated every second
 */
void set_seconds_alarm();

/**
 * @brief Sets an alarm in seconds seconds from now
 * @param seconds The time left until the alarm
 */
void set_timed_alarm(uint8_t seconds);

/**
 * @brief Clears the interruption flags
 * @return The reg_c at the time of the interrupt
 */
uint8_t clear_interruptions();

/**
 * @brief Conversion from bcd to integer
 * @param number The number to be converted
 */
int bcd_to_int(uint8_t number);

/**
 * @brief Conversion from integer to bcd
 * @param number The number to be converted
 */
uint8_t int_to_bcd(int number);

/**
 * @brief Disables periodic interruptions.
*/
void disable_periodic_int();

/**
 *@ brief Enables alarm interruptions.
*/
void enable_alarm_int();

/**
 *@ brief Disables alarm interruptions.
*/
void disable_alarm_int();

#endif //RTC_H
