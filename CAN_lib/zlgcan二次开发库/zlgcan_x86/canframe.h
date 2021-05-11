#ifndef CANFRAME_H_
#define CANFRAME_H_


#include "typedef.h"

/* special address description flags for the MAKE_CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error message frame */
#define CAN_ID_FLAG  0x1FFFFFFFU /* id */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

/*
* Controller Area Network Identifier structure
*
* bit 0-28	: CAN identifier (11/29 bit)
* bit 29	: error message frame flag (0 = data frame, 1 = error message)
* bit 30	: remote transmission request flag (1 = rtr frame)
* bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
*/
typedef UINT canid_t;

#define CAN_SFF_ID_BITS		11
#define CAN_EFF_ID_BITS		29

/*
* Controller Area Network Error Message Frame Mask structure
*
* bit 0-28	: error class mask (see include/linux/can/error.h)
* bit 29-31	: set to zero
*/
typedef UINT can_err_mask_t;

/* CAN payload length and DLC definitions according to ISO 11898-1 */
#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN 8

/* CAN FD payload length and DLC definitions according to ISO 11898-7 */
#define CANFD_MAX_DLC 15
#define CANFD_MAX_DLEN 64

 // make id
#define MAKE_CAN_ID(id, eff, rtr, err) (id | (!!(eff) << 31) | (!!(rtr) << 30) | (!!(err) << 29))
#define IS_EFF(id) (!!(id & CAN_EFF_FLAG)) //1:extend frame 0:standard frame
#define IS_RTR(id) (!!(id & CAN_RTR_FLAG)) //1:remote frame 0:data frame
#define IS_ERR(id) (!!(id & CAN_ERR_FLAG)) //1:error frame 0:normal frame
#define GET_ID(id) (id & CAN_ID_FLAG)

/* TX_DELAY_SEND_FLAG apply to can_frame.__pad and canfd_frame.flags */
#define TX_DELAY_SEND_FLAG 0x80 /* indicat tx frame in delay send mode, 1:send in device queue; 0:send direct to bus */
#define IS_DELAY_SEND(flag) (!!(flag & TX_DELAY_SEND_FLAG)) //1:delay send frame with delay time in __res0/__res0 0:normal frame
#define TX_DELAY_SEND_TIME_UNIT_FLAG 0x40 /* indicat tx delay send time unit, 1:time unit is 100us; 0:time unit is 1ms */
#define IS_DELAY_SEND_TIME_UNIT_MS(flag) (!(flag & TX_DELAY_SEND_TIME_UNIT_FLAG))       //0: time unit 100us 1: time unit 1ms
#define IS_DELAY_SEND_TIME_UNIT_100US(flag) (!!(flag & TX_DELAY_SEND_TIME_UNIT_FLAG))   //1: time unit 100us 0: time unit 1ms

/**
* struct can_frame - basic CAN frame structure
* @can_id:  CAN ID of the frame and CAN_*_FLAG flags, see canid_t definition
* @can_dlc: frame payload length in byte (0 .. 8) aka data length code
*           N.B. the DLC field from ISO 11898-1 Chapter 8.4.2.3 has a 1:1
*           mapping of the 'data length code' to the real payload length
* @__pad:   padding
* @__res0:  reserved / padding
* @__res1:  reserved / padding
* @data:    CAN frame payload (up to 8 byte)
*/
typedef struct {
	canid_t can_id;  /* 32 bit MAKE_CAN_ID + EFF/RTR/ERR flags */
	BYTE    can_dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
	BYTE    __pad;   /* padding */
	BYTE    __res0;  /* reserved / padding */
	BYTE    __res1;  /* reserved / padding */
	BYTE    data[CAN_MAX_DLEN]/* __attribute__((aligned(8)))*/;
}can_frame;

/*
* defined bits for canfd_frame.flags
*
* The use of struct canfd_frame implies the Extended Data Length (EDL) bit to
* be set in the CAN frame bitstream on the wire. The EDL bit switch turns
* the CAN controllers bitstream processor into the CAN FD mode which creates
* two new options within the CAN FD frame specification:
*
* Bit Rate Switch - to indicate a second bitrate is/was used for the payload
* Error State Indicator - represents the error state of the transmitting node
*
* As the CANFD_ESI bit is internally generated by the transmitting CAN
* controller only the CANFD_BRS bit is relevant for real CAN controllers when
* building a CAN FD frame for transmission. Setting the CANFD_ESI bit can make
* sense for virtual CAN interfaces to test applications with echoed frames.
*/
#define CANFD_BRS 0x01 /* bit rate switch (second bitrate for payload data) */
#define CANFD_ESI 0x02 /* error state indicator of the transmitting node */

/**
* struct canfd_frame - CAN flexible data rate frame structure
* @can_id: CAN ID of the frame and CAN_*_FLAG flags, see canid_t definition
* @len:    frame payload length in byte (0 .. CANFD_MAX_DLEN)
* @flags:  additional flags for CAN FD
* @__res0: reserved / padding
* @__res1: reserved / padding
* @data:   CAN FD frame payload (up to CANFD_MAX_DLEN byte)
*/
typedef struct {
	canid_t can_id;  /* 32 bit MAKE_CAN_ID + EFF/RTR/ERR flags */
	BYTE    len;     /* frame payload length in byte */
	BYTE    flags;   /* additional flags for CAN FD,i.e error code */
	BYTE    __res0;  /* reserved / padding */
	BYTE    __res1;  /* reserved / padding */
	BYTE    data[CANFD_MAX_DLEN]/* __attribute__((aligned(8)))*/;
}canfd_frame;

#define CAN_MTU		(sizeof(struct can_frame))
#define CANFD_MTU	(sizeof(struct canfd_frame))

#endif //CANFRAME_H_