 /******************************************************************************
 *
 * Module: Common - Platform Abstraction
 *
 * File Name: Platform_Types.h
 *
 * Description: Platform types for C/C++
 *
 * Author: Boules Ruphael
 *
 *******************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include <stdint.h>

/*
 * Boolean Values
 */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

/* This is used to define the void pointer to zero definition */
#ifndef NULL_PTR
#define NULL_PTR          ((void *)0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned char         boolean;
typedef unsigned char         uint8;                /*                0 .. 255                  */
typedef signed char           sint8;                /*             -128 .. +127                 */
typedef unsigned short        uint16;               /*                0 .. 65,535               */
typedef signed short          sint16;               /*          -32,768 .. +32,767              */
typedef unsigned long         uint32;               /*                0 .. 4,294,967,295        */
typedef signed long           sint32;               /*   -2,147,483,648 .. +2,147,483,647       */
typedef unsigned long long    uint64;               /*                0 .. ~ 18(10^18)          */
typedef signed long long      sint64;               /*      ~ -9(10^18) .. ~ +9(10^18)          */
typedef float                 float32;
typedef double                float64;

/* Pointer to Function */
typedef void (*PtrFun_t) (void);


#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */



#endif /* PLATFORM_TYPES_H */
