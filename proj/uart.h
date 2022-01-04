#include <lcom/lcf.h>

/** @defgroup uart uart
 * @{
 *
 * Functions for using the uart
 */
 
#define COM1 0x3F8 /**< @brief Port base address.*/

#define THR 0 /**< @brief Transmitter Holding Register.*/
#define SER_DATA 0 /**< @brief Receiver Buffer Register.*/
#define IER 1 /**< @brief Interrupt Enable Register.*/
#define IIR 2 /**< @brief Interrupt Identification Register.*/
#define LCR 3 /**< @brief Line Control Register.*/
#define LSR 5 /**< @brief Line Status Register.*/


#define LCR_DLAB BIT(7) /**< @brief Line Control Register - Divisor Latch Access.*/

#define LSR_REC_DATA BIT(0) /**< @brief Line Status Register - Receiver Data.*/
#define LSR_THR BIT(5) /**< @brief Line Status Register - Transmitter Holding Register Empty.*/
#define LSR_FIFO_ERROR BIT(7) /**< @brief Line Status Register - FIFO Error.*/

#define IER_DATA_INT BIT(0) /**< @brief Interrupt Enable Register - Enable the received Data Available Interrupt.*/
#define IER_TRANS_EMPTY_INT BIT(1) /**< @brief Interrupt Enable Register - Enable the Transmitter Holding Register Empty Interrupt.*/

#define IIR_MOD_STAT 0 /**< @brief Interrupt Identification Register - Modem Status (if set, no interrupt is pending).*/
#define IIR_TRANS_EMPTY 1 /**< @brief Interrupt Identification Register - Transmitter Holding Register Empty (transmitter ready interrupt).*/
#define IIR_REC_DATA 2 /**< @brief Interrupt Identification Register - Received Data Ready (received char interrupt).*/
#define IIR_LINE_STAT 3 /**< @brief Interrupt Identification Register - Receiver Line Status(received error interrupts).*/
#define IIR_CHAR_TIMEOUT 4 /**< @brief Interrupt Identification Register - Character Timeout(FIFO).*/

#define UART_IRQ 4 /**< @brief Port COM1 IRQ line 4.*/

/**
 * @brief Subscribes uart interruptions
 * @param bit_no Return argument with the mask bit
 * @param notification Return argument with the notification address
 */

int uart_subscribe(uint8_t* bit_no, int *notification);

/**
 * @brief Unsubscribes uart interruptions
 * @param notification The correspondent uart notification address
 */
int uart_unsubscribe(int *notification);


/**
 * @brief  Handles the uart interruptions
 * @return The origin of the interrupt, as in IIR defines
 */
uint8_t uart_handler(uint8_t *data);

/**
 *  @brief Checks if the THR is emtpy and ready for transmission
 *  @return True if is empty, false otherwise
 */
bool check_thr_emtpy();

/**
 * @brief Check if the buffer is ready for reading
 * @return 0 if OK, 1 if error, 2 if empty
 */
int check_buffer();

/**
 * @brief Check if address 0 is set to data mode
 * @details Check if bit dlab of the lcr is 0
 * @return True if is in data mode, false otherwise
 */
bool check_data_dlab();

/**
 * @brief Sets the address 0 to data mode
 * @details Sets the bit dlab of the lcr to 0
 */
void set_data_dlab();

/**
 * @brief Sends message specified in bytes to the other machine, uses serial_write_byte as an auxiliary function.
 * @param bytes Message to send.
 * @param size Size of the message.
 * @return True if the message is sent successfully, false otherwise.
 */
bool serial_write_message(uint8_t *bytes, uint8_t size);

/**
 * @brief Sends information to the other machine, if THR is empty.
 * @param byte Information to send.
 * @return True if information is sent successfully, false otherwise.
 */
bool serial_write_byte(uint8_t byte);

/**
 * @brief Writes acknowlegment byte.
 * @return True if successful, false otherwise.
 */
bool seria_write_ack();

/**
 * @brief Reads information received from the other machine.
 * @param byte Variable where to store read information.
 * @return True if THR is empty, false otherwise.
 */
bool serial_read_byte(uint8_t *byte);

/** 
 * @brief Enables the Received Data Available Interrupt.
*/
void enable_received_data_int();

/** 
 * @brief Disables the Received Data Available Interrupt.
*/
void disable_received_data_int();

/** 
 * @brief Enables the Transmitter Holding Register Empty Interrupt.
*/
void enable_trans_empty_int();

/** 
 * @brief Disables the Transmitter Holding Register Empty Interrupt.
*/
void disable_trans_empty_int();

/** 
 * @brief Gets the source of interrupts using the Interrupt Identification Register.
 * @return The source of the interrupts.
*/
uint8_t get_int_origin();
