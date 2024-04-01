#ifndef MCAL_DEF_H
#define MCAL_DEF_H

typedef enum STATUS
{
  MCAL_OK       = 0x00U,
  MCAL_ERROR    = 0x01U,
  MCAL_BUSY     = 0x02U,
  MCAL_TIMEOUT  = 0x03U
} MCALStatus_t;

#endif