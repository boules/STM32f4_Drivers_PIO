#ifndef HCAL_DEF_H
#define HAL_DEF_H

typedef enum STATUS
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HALStatus_t;

#endif