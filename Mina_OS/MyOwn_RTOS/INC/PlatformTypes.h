/*
 * platform_types.h
 *
 *      Author: Mina Saad
 */

/***********************************************************************/
/*                        HEADER PROTECTION                            */
/***********************************************************************/
#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_

/***********************************************************************/
/*                            MAKING BOOL                              */
/***********************************************************************/
typedef enum
{
false, true

}bool;

/***********************************************************************/
/*                  NON-VOLATILE SIGN DATA TYPES                       */
/***********************************************************************/
typedef char                               sint8_t;
typedef short int                          sint16_t;
typedef int                                sint32_t;
typedef long long                          sint64_t;

/***********************************************************************/
/*                NON-VOLATILE UNSIGNED DATA TYPES                     */
/***********************************************************************/
typedef unsigned short                     uint16_t;
typedef unsigned int                       unint32_t;
typedef unsigned long long                 uint64_t;

/***********************************************************************/
/*                NON-VOLATILE SIGN  REAL DATA TYPES                   */
/***********************************************************************/
typedef float                              float32_t;
typedef double                             float64_t;

/***********************************************************************/
/*                  VOLATILE SIGN DATA TYPES                           */
/***********************************************************************/
typedef  volatile char                     vsint8_t;
typedef  volatile short int                vsint16_t;
typedef  volatile int                      vsint32_t;
typedef  volatile long long                vsint64_t;

/***********************************************************************/
/*                VOLATILE UNSIGNED DATA TYPES                         */
/***********************************************************************/
typedef volatile unsigned short            vuint16_t;
typedef volatile unsigned int              vunint32_t;
typedef volatile unsigned long long        vuint64_t;

/***********************************************************************/
/*                VOLATILE SIGN  REAL DATA TYPES                       */
/***********************************************************************/
typedef volatile float                     vfloat32_t;
typedef volatile double                    vfloat64_t;

#endif
