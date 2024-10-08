/* sim_tmxr.h: terminal multiplexor definitions

   Copyright (c) 2001-2024, Robert M Supnik

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   ROBERT M SUPNIK BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   Except as contained in this notice, the name of Robert M Supnik shall not be
   used in advertising or otherwise to promote the sale, use or other dealings
   in this Software without prior written authorization from Robert M Supnik.

   Based on the original DZ11 simulator by Thord Nilson, as updated by
   Arthur Krewat.

   19-Apr-24    RMS     Merged changes for CH11 (Lars Brinkhoff)
   04-Apr-24    JDB     Added "sim_con_tmxr" and "sim_con_ldsc" global declarations
   12-Aug-23    JDB     Added extension pointer to TMXR structure
   23-Oct-20    JDB     Added tmxr_post_logs global routine
   19-Dec-19    JDB     Added tmxr_is_extended global hook
   19-Mar-19    JDB     Added extension pointer to TMLN structure;
                        added tmxr_read, tmxr_write, tmxr_show, tmxr_close global hooks;
                        added tmxr_find_ldsc, tmxr_send_buffered_data, tmxr_init_line,
                        tmxr_report_connection, and tmxr_disconnect_line globals
   06-Aug-15    JDB     [4.0] Added modem control bits and functions
   14-Dec-14    JDB     [4.0] Added include of "sim_sock.h" for SOCKET, etc.
   20-Nov-08    RMS     Added three new standardized SHOW routines
   27-May-08    JDB     Added lnorder to TMXR structure,
                        added tmxr_set_lnorder and tmxr_set_lnorder
   14-May-08    JDB     Added dptr to TMXR structure
   04-Jan-04    RMS     Changed TMXR ldsc to be pointer to linedesc array
                        Added tmxr_linemsg, logging (from Mark Pizzolato)
   29-Dec-03    RMS     Added output stall support, increased buffer size
   22-Dec-02    RMS     Added break support (from Mark Pizzolato)
   20-Aug-02    RMS     Added tmxr_open_master, tmxr_close_master, tmxr.port
   30-Dec-01    RMS     Renamed tmxr_fstatus, added tmxr_fstats
   20-Oct-01    RMS     Removed tmxr_getchar, formalized buffer guard,
                        added tmxr_rqln, tmxr_tqln
*/

#ifndef SIM_TMXR_H_
#define SIM_TMXR_H_     0

#include "sim_sock.h"

#define TMXR_V_VALID    15
#define TMXR_VALID      (1 << TMXR_V_VALID)
#define TMXR_MAXBUF     512                             /* buffer size */
#define TMXR_GUARD      12                              /* buffer guard */

/* Modem Control Bits */

#define TMXR_MDM_DTR        0x01    /* Data Terminal Ready */
#define TMXR_MDM_RTS        0x02    /* Request To Send     */
#define TMXR_MDM_DCD        0x04    /* Data Carrier Detect */
#define TMXR_MDM_RNG        0x08    /* Ring Indicator      */
#define TMXR_MDM_CTS        0x10    /* Clear To Send       */
#define TMXR_MDM_DSR        0x20    /* Data Set Ready      */
#define TMXR_MDM_INCOMING   (TMXR_MDM_DCD|TMXR_MDM_RNG|TMXR_MDM_CTS|TMXR_MDM_DSR)  /* Settable Modem Bits */
#define TMXR_MDM_OUTGOING   (TMXR_MDM_DTR|TMXR_MDM_RTS)  /* Settable Modem Bits */

struct tmln {
    SOCKET              conn;                           /* line conn */
    char                *ipad;                          /* IP address */
    uint32              cnms;                           /* conn time */
    int32               tsta;                           /* Telnet state */
    int32               rcve;                           /* rcv enable */
    int32               xmte;                           /* xmt enable */
    int32               dstb;                           /* disable Tlnt bin */
    int32               rxbpr;                          /* rcv buf remove */
    int32               rxbpi;                          /* rcv buf insert */
    int32               rxcnt;                          /* rcv count */
    int32               txbpr;                          /* xmt buf remove */
    int32               txbpi;                          /* xmt buf insert */
    int32               txcnt;                          /* xmt count */
    FILE                *txlog;                         /* xmt log file */
    char                *txlogname;                     /* xmt log file name */
    char                rxb[TMXR_MAXBUF];               /* rcv buffer */
    char                rbr[TMXR_MAXBUF];               /* rcv break */
    char                txb[TMXR_MAXBUF];               /* xmt buffer */
    void                *exptr;                         /* extension pointer */
    };

typedef struct tmln TMLN;

struct tmxr {
    int32               lines;                          /* # lines */
    int32               port;                           /* listening port */
    SOCKET              master;                         /* master socket */
    TMLN                *ldsc;                          /* line descriptors */
    int32               *lnorder;                       /* line connection order */
    DEVICE              *dptr;                          /* multiplexer device */
    void                *exptr;                         /* extension pointer */
    };

typedef struct tmxr TMXR;

int32 tmxr_poll_conn (TMXR *mp);
void tmxr_reset_ln (TMLN *lp);
int32 tmxr_getc_ln (TMLN *lp);
t_stat tmxr_get_packet_ln (TMLN *lp, const uint8 **pbuf, size_t *psize);
void tmxr_poll_rx (TMXR *mp);
t_stat tmxr_putc_ln (TMLN *lp, int32 chr);
t_stat tmxr_put_packet_ln (TMLN *lp, const uint8 *buf, size_t size);
void tmxr_poll_tx (TMXR *mp);
t_stat tmxr_open_master (TMXR *mp, char *cptr);
t_stat tmxr_open_master_xtnd (TMXR *mp, char *cptr);
t_stat tmxr_close_master (TMXR *mp);
t_stat tmxr_attach (TMXR *mp, UNIT *uptr, char *cptr);
t_stat tmxr_detach (TMXR *mp, UNIT *uptr);
t_stat tmxr_set_modem_control_passthru (TMXR *mp);
t_stat tmxr_set_get_modem_bits (TMLN *lp, int32 bits_to_set, int32 bits_to_clear, int32 *incoming_bits);
t_stat tmxr_ex (t_value *vptr, t_addr addr, UNIT *uptr, int32 sw);
t_stat tmxr_dep (t_value val, t_addr addr, UNIT *uptr, int32 sw);
void tmxr_msg (SOCKET sock, const char *msg);
void tmxr_linemsg (TMLN *lp, const char *msg);
void tmxr_fconns (FILE *st, TMLN *lp, int32 ln);
void tmxr_fstats (FILE *st, TMLN *lp, int32 ln);
t_stat tmxr_set_log (UNIT *uptr, int32 val, char *cptr, void *desc);
t_stat tmxr_set_nolog (UNIT *uptr, int32 val, char *cptr, void *desc);
void tmxr_post_logs (t_bool close_logs);
t_stat tmxr_show_log (FILE *st, UNIT *uptr, int32 val, void *desc);
t_stat tmxr_dscln (UNIT *uptr, int32 val, char *cptr, void *desc);
int32 tmxr_rqln (TMLN *lp);
int32 tmxr_tqln (TMLN *lp);
t_stat tmxr_set_lnorder (UNIT *uptr, int32 val, char *cptr, void *desc);
t_stat tmxr_show_lnorder (FILE *st, UNIT *uptr, int32 val, void *desc);
t_stat tmxr_show_summ (FILE *st, UNIT *uptr, int32 val, void *desc);
t_stat tmxr_show_cstat (FILE *st, UNIT *uptr, int32 val, void *desc);
t_stat tmxr_show_lines (FILE *st, UNIT *uptr, int32 val, void *desc);
TMLN *tmxr_find_ldsc (UNIT *uptr, int32 val, TMXR *mp);
int32 tmxr_send_buffered_data (TMLN *lp);
void tmxr_init_line (TMLN *lp);
void tmxr_report_connection (TMXR *mp, TMLN *lp, int32 line);
void tmxr_disconnect_line (TMLN *lp);

/* Extension interface */

extern int32  (*tmxr_read)        (TMLN *lp, int32 length);
extern int32  (*tmxr_write)       (TMLN *lp, int32 length);
extern void   (*tmxr_show)        (TMLN *lp, FILE *stream);
extern void   (*tmxr_close)       (TMLN *lp);
extern t_bool (*tmxr_is_extended) (TMLN *lp);

/* Console Telnet support structures */

extern TMXR sim_con_tmxr;                       /* console multiplexer descriptor */
extern TMLN sim_con_ldsc;                       /* console line descriptor */

/* V4.X shims */

#define tmxr_set_console_units(rxuptr, txuptr)

#endif
