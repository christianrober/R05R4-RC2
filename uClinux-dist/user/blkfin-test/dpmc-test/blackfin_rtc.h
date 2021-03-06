/* RTC registers */
#define RTC_STAT    0xFFC00300
#define RTC_ICTL    0xFFC00304
#define RTC_ISTAT   0xFFC00308
#define RTC_SWCNT   0xFFC0030c
#define RTC_ALARM   0xFFC00310
#define RTC_PREN    0xFFC00314


/* bit define */
#define DAY_BITS_OFF     24
#define HOUR_BITS_OFF    16
#define MIN_BITS_OFF     8
#define SEC_BITS_OFF     0

/*RTC Interrupt Control Register Bit Define*/

#define STPW_INT_EN     0x0001
#define ALM_INT_EN      0x0002
#define SEC_INT_EN      0x0004
#define MIN_INT_EN      0x0008
#define H_INT_EN        0x0010
#define H24_INT_EN      0x0020
#define DAY_INT_EN      0x0040
#define WC_INT_EN       0x8000

/*RTC Interrupt Status Register  bit define */
#define STPW_EVT_FG     0x0001
#define ALM_EVT_FG      0x0002
#define SEC_EVT_FG      0x0004
#define MIN_EVT_FG      0x0008
#define H_EVT_FG     	0x0010
#define H24_EVT_FG      0x0020
#define DAY_EVT_FG      0x0040
#define WP_EVT_FG       0x4000
#define WC_EVT_FG       0x8000

/* bit define */
#define PRESCALE_EN     0x0001

//Stopwatch IOCTL
#define RTC_SWCNT_OFF   _IO('p', 0xF0)
#define RTC_SWCNT_ON    _IO('p', 0xF1)
#define RTC_SWCNT_SET   _IOW('p', 0xF2, unsigned long) 
#define RTC_SWCNT_RD    _IOR('p', 0xF3, unsigned long) 
#define RTC_CLEAR_INTS  _IO('p', 0xF4)



