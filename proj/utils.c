#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb)
{
  if (lsb == NULL)
  {
    return 1;
  }
  uint8_t LSB = val & 0xFF;
  lsb[0] = LSB;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb)
{
  if (msb == NULL)
  {
    return 1;
  }
  uint8_t MSB = (val & 0xFF00) >> 8;
  msb[0] = MSB;

  return 0;
}

int(util_sys_inb)(int port, uint8_t *value)
{
  int call_back = 0;
  if (value == NULL)
  {
    return 1;
  }

  uint32_t word = 0;
  uint32_t *byte = &word;
  call_back = sys_inb(port, byte);

  value[0] = byte[0] & 0xff;
  return call_back;
}

int16_t cpl2_9_to_16(uint8_t value, bool sign_bit)
{
  if (sign_bit)
  {
    return -(1 << 8) + value;
  }
  return value;
}
