

#ifndef _SMARTCFG_TYPE_H_
#define _SMARTCFG_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* macro */
//#define BUFFER_MAX 800
#if 0
#define IEEE80211_FC0_VERSION_MASK      0x03
#define IEEE80211_FC0_VERSION_SHIFT     0
#define IEEE80211_FC0_VERSION_0         0x00
#define IEEE80211_FC0_TYPE_MASK         0x0c
#define IEEE80211_FC0_TYPE_SHIFT        2
#define IEEE80211_FC0_TYPE_MGT          0x00
#define IEEE80211_FC0_TYPE_CTL          0x04
#define IEEE80211_FC0_TYPE_DATA         0x08
#define IEEE80211_FC0_SUBTYPE_MASK      0xf0
#define IEEE80211_FC0_SUBTYPE_SHIFT     4
/* for TYPE_MGT */
#define IEEE80211_FC0_SUBTYPE_ASSOC_REQ     0x00
#define IEEE80211_FC0_SUBTYPE_ASSOC_RESP    0x10
#define IEEE80211_FC0_SUBTYPE_REASSOC_REQ   0x20
#define IEEE80211_FC0_SUBTYPE_REASSOC_RESP  0x30
#define IEEE80211_FC0_SUBTYPE_PROBE_REQ     0x40
#define IEEE80211_FC0_SUBTYPE_PROBE_RESP    0x50
#define IEEE80211_FC0_SUBTYPE_BEACON        0x80
#define IEEE80211_FC0_SUBTYPE_ATIM          0x90
#define IEEE80211_FC0_SUBTYPE_DISASSOC      0xa0
#define IEEE80211_FC0_SUBTYPE_AUTH          0xb0
#define IEEE80211_FC0_SUBTYPE_DEAUTH        0xc0
#define IEEE80211_FCO_SUBTYPE_ACTION        0xd0
/* for TYPE_CTL */
#define IEEE80211_FC0_BLOCK_ACK_REQ         0x80
#define IEEE80211_FC0_BLOCK_ACK             0x90
#define IEEE80211_FC0_SUBTYPE_PS_POLL       0xa0
#define IEEE80211_FC0_SUBTYPE_RTS       0xb0
#define IEEE80211_FC0_SUBTYPE_CTS       0xc0
#define IEEE80211_FC0_SUBTYPE_ACK       0xd0
#define IEEE80211_FC0_SUBTYPE_CF_END        0xe0
#define IEEE80211_FC0_SUBTYPE_CF_END_ACK    0xf0
/* for TYPE_DATA (bit combination) */
#define IEEE80211_FC0_SUBTYPE_DATA      0x00
#define IEEE80211_FC0_SUBTYPE_CF_ACK        0x10
#define IEEE80211_FC0_SUBTYPE_CF_POLL       0x20
#define IEEE80211_FC0_SUBTYPE_CF_ACPL       0x30
#define IEEE80211_FC0_SUBTYPE_NODATA        0x40
#define IEEE80211_FC0_SUBTYPE_CFACK     0x50
#define IEEE80211_FC0_SUBTYPE_CFPOLL        0x60
#define IEEE80211_FC0_SUBTYPE_CF_ACK_CF_ACK 0x70
#define IEEE80211_FC0_SUBTYPE_QOS       0x80
#define IEEE80211_FC0_SUBTYPE_QOS_NULL      0xc0

#define IEEE80211_FC1_DIR_MASK          0x03
#define IEEE80211_FC1_DIR_NODS          0x00    /* STA->STA */
#define IEEE80211_FC1_DIR_TODS          0x01    /* STA->AP  */
#define IEEE80211_FC1_DIR_FROMDS        0x02    /* AP ->STA */
#define IEEE80211_FC1_DIR_DSTODS        0x03    /* AP ->AP  */

#define IEEE80211_FC1_MORE_FRAG         0x04
#define IEEE80211_FC1_RETRY             0x08
#define IEEE80211_FC1_PWR_MGT           0x10
#define IEEE80211_FC1_MORE_DATA         0x20
#define IEEE80211_FC1_WEP               0x40
#define IEEE80211_FC1_ORDER             0x80

#define IEEE80211_SEQ_FRAG_MASK         0x000f
#define IEEE80211_SEQ_FRAG_SHIFT        0
#define IEEE80211_SEQ_SEQ_MASK          0xfff0
#define IEEE80211_SEQ_SEQ_SHIFT         4
#endif

/* enum */
typedef enum smrtcfg_state {
    SMARTCGF_STATE_ERROR = -1,
    SMARTCGF_STATE_INVALID_DATA,
    SMARTCGF_STATE_START,
    SMARTCGF_STATE_INPROGRESS,
    SMARTCGF_STATE_FIND_BASE,
    SMARTCGF_STATE_FIND_MAGIC,
    SMARTCGF_STATE_FIND_PREFIX,
    SMARTCGF_STATE_FIND_SEQ_HEADER,
    SMARTCGF_STATE_DONE,
} smartcfg_state_t;

/* structure */
typedef struct smrtcfg_frm_hdr_lite {
    unsigned char fc0;
    unsigned char fc1;
    unsigned char dmac[6];
    int length;
}smrtcfg_frm_hdr_lite_t;

#define MAX_DATA_VAL 512

#ifdef __cplusplus
}
#endif

#endif
