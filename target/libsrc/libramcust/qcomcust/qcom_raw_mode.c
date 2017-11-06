#include <osapi.h>
#include <wlan_dev.h>
#include "ipport.h"
#include "netbuf.h"
#include "qcom/tx_alloc_api.h"
#include "qcom/qcom_cdr.h"
#include "cdr_buf_api.h"
#include "wlan/bufpool/buf_pool_internal_api.h"
#include "qcom/qcom_timer.h"
#include "qcom/qcom_raw_mode.h"
#include "qcom/qcom_legacy_api.h"
#include "qcom/qcom_wlan.h"

#define QCOM_BEACON_DEBUG_EN 0
#if (QCOM_BEACON_DEBUG_EN == 0)
#define QCOM_BEACON_DEBUG_PRINTF(...)
#else
extern int cmnos_printf(const char * format, ...);
#define QCOM_BEACON_DEBUG_PRINTF    cmnos_printf
#endif

#define BEACON_FRAME (0)
#define QOS_DATA_FRAME (1)
#define ADDRESS_DATA_FRAME (2)
#define PROBE_REQ_FRAME (3)


extern devh_t *gdevp;
extern BUF_POOL    *g_bp;

/* raw mode */
struct ath_buf *dbg_abf = NULL;

A_STATUS _qcom_raw_mode_send_pkt(QCOM_RAW_MODE_PARAM_t *ppara)
{
    devh_t *dev ;
    struct ath_buf *abf = NULL;
    struct HTC_bufinfo_s *bufinfo;
    struct txbuf * tbf;
    tx_cb_t * cb;
    WMI_DATA_HDR wmi_data_h;
    WMI_DATA_HDR *dhdr;
    WMI_TX_META_V3 v3_meta;
    A_UINT8 pktInfo = 0;
    A_UINT8 devId=0;
    A_UINT8 access_cat;
    A_UINT32 flags;
    A_STATUS ret = 0;
    A_UINT32 i = 0;
#define WIFI_HDR_SZ (24)
#define WIFI_4_HDR_SZ (32)
#define WIFI_QOS_HDR_SZ (28)
#define IEEE80211_ADDR_COPY(dst,src)    A_MEMCPY(dst,src,IEEE80211_ADDR_LEN)
	/*beacom frame and proobe header is the same */
    A_UINT8 header_sz[4]={WIFI_HDR_SZ, WIFI_QOS_HDR_SZ, WIFI_4_HDR_SZ,WIFI_HDR_SZ};

    if (ppara == NULL)
    {
    	return A_EINVAL ;
    }
	
	A_UINT8 rate_index = ppara->rate_index;
	A_UINT8 tries = ppara->tries;
	A_UINT32 size = ppara->size;
	A_UINT32 chan = ppara->chan;
	A_UINT8 header_type = ppara->header_type;
	A_UINT16 bcn_seq_num = ppara->seq;
	A_UINT8 *paddr1 = &ppara->addr1.addr[0];
	A_UINT8 *paddr2 = &ppara->addr2.addr[0];
	A_UINT8 *paddr3 = &ppara->addr3.addr[0];
	A_UINT8 *paddr4 = &ppara->addr4.addr[0];
	A_UINT8 *pdatabuf = ppara->pdatabuf;
	A_UINT32 buflen = ppara->buflen;


    typedef struct{
               A_UINT8 fc[2];
               A_UINT8 duration[2];
               A_UINT8 addr[3][6];
               A_UINT8 seq[2];
    }WIFI_HDR;

    typedef struct{
               A_UINT8 fc[2];
               A_UINT8 duration[2];
               A_UINT8 addr[3][6];
               A_UINT8 seq[2];
               A_UINT8 addr_3[6];
               A_UINT8 pad[2]; // pad required by firmware
    }WIFI_4_HDR;

    typedef struct{
               A_UINT8 fc[2];
               A_UINT8 duration[2];
               A_UINT8 addr[3][6];
               A_UINT8 seq[2];
               A_UINT8 qos[2];
               A_UINT8 pad[2]; // pad required by firmware
    }WIFI_QOS_HDR;

    WIFI_4_HDR* p4Hdr;
    WIFI_HDR* pHdr;
    WIFI_QOS_HDR *pQosHdr;

    if(rate_index > 19 ||
        tries > 14 ||
        size > 1400 ||
        header_type > 3){
        QCOM_BEACON_DEBUG_PRINTF("wrong parameter!\n");
        return A_EINVAL ;
    }

	if (pdatabuf != NULL)
	{
		if (buflen > size)
		{
		   QCOM_BEACON_DEBUG_PRINTF("wrong parameter, size is small!\n");
		   return A_EINVAL ;
		}
	}
    /* user can set channel outside this function to speed up the test time.      * if channel sets to 0, this function will not set channel
     */
    if( chan != 0 ) {        if (A_OK != qcom_set_channel(0, chan))		{            QCOM_BEACON_DEBUG_PRINTF("channel %d is invalid\n", chan);           return A_EBADCHANNEL ;        }        /*need this delay for channel stable. if not, send less packets than tries*/             qcom_thread_msleep(10);     }    //qcom_set_power_mode(0,MAX_PERF_POWER);

    QCOM_BEACON_DEBUG_PRINTF("set rate_index = %d, tries = %d, size = %d, chan = %d, header_type = %d\n", \
			             rate_index, tries, size, chan, header_type);

    dbg_abf =  abf = htc_buf_alloc();
    /*when htc buffer run out for hw pending, try to reallocate new buffers*/
    if (!dbg_abf) {
        qcom_thread_msleep(10);
        buf_pool_reassign_free_bufs(g_bp);
        abf = htc_buf_alloc();
    }

    if(!abf) {
        QCOM_BEACON_DEBUG_PRINTF("htc buf alloc failed!\n");
        return A_NO_MEMORY ;
    } else {
        bufinfo = ATH2HTC(abf);
        bufinfo->next = NULL;
        bufinfo->actual_length = 0;
        tbf = abf->bf_b.wlanBuf.tx;
        cb = (tx_cb_t *)tbf->cb;
        cb->ni = NULL;
        cb->flags = 0;
        cb->hsFlags = 0;
        cb->sendCompleteFlags = 0;
    }

        //A_DUMP_BUFFER(buf, length);
    pHdr = (WIFI_HDR*)tbf->bfData.wh;

    memset(pHdr, 0, WIFI_HDR_SZ);

    pHdr->fc[0] = 0x80; //beacon
    pHdr->seq[0] = ((bcn_seq_num & 0xf) << 4);
    pHdr->seq[1] = ((bcn_seq_num & 0xff0) >> 4);

    pHdr->addr[0][0] = paddr1[0];
    pHdr->addr[0][1] = paddr1[1];
    pHdr->addr[0][2] = paddr1[2];
    pHdr->addr[0][3] = paddr1[3];
    pHdr->addr[0][4] = paddr1[4];
    pHdr->addr[0][5] = paddr1[5];
	pHdr->addr[1][0] = paddr2[0];
	pHdr->addr[1][1] = paddr2[1];
	pHdr->addr[1][2] = paddr2[2];
	pHdr->addr[1][3] = paddr2[3];
	pHdr->addr[1][4] = paddr2[4];
	pHdr->addr[1][5] = paddr2[5];

	pHdr->addr[2][0] = paddr3[0];
	pHdr->addr[2][1] = paddr3[1];
	pHdr->addr[2][2] = paddr3[2];
	pHdr->addr[2][3] = paddr3[3];
	pHdr->addr[2][4] = paddr3[4];
	pHdr->addr[2][5] = paddr3[5];
    access_cat = 0;

    if(header_type == QOS_DATA_FRAME){
    	//change frame control to qos data
    	pHdr->fc[0] = 0x88;
    	pHdr->fc[1] = 0x01; //to-ds
    	pQosHdr = (WIFI_QOS_HDR*)pHdr;
    	pQosHdr->qos[0] = 0x03;
    	pQosHdr->qos[1] = 0x00;
    	//bssid == addr[0]
    	//memcpy(&(pHdr->addr[0][0]), &(pHdr->addr[1][0]), 6);
    	//change destination address
    	//pHdr->addr[2][3] = 0xaa;
    	//pHdr->addr[2][4] = 0xaa;
    	//pHdr->addr[2][5] = 0xaa;
    	access_cat = 2;
    }else if(header_type == ADDRESS_DATA_FRAME){
    	pHdr->fc[0] = 0x08;
    	pHdr->fc[1] = 0x03; //to-ds | from-ds
    	p4Hdr = (WIFI_4_HDR*)pHdr;
    	p4Hdr->addr_3[0] = paddr4[0];
    	p4Hdr->addr_3[1] = paddr4[1];
    	p4Hdr->addr_3[2] = paddr4[2];
    	p4Hdr->addr_3[3] = paddr4[3];
    	p4Hdr->addr_3[4] = paddr4[4];
    	p4Hdr->addr_3[5] = paddr4[5];
    }
	else if(header_type == PROBE_REQ_FRAME){
        //change beacon frame to probe req
	    pHdr->fc[0] = 0x40;
	 }

    size += header_sz[header_type];

    /* now setup the frame for transmission */
	if (pdatabuf == NULL)
	{
        QCOM_BEACON_DEBUG_PRINTF("pdatabuf is null\n");
    	for(i=header_sz[header_type];i<size;i++)
    		((A_UINT8 *)pHdr)[i] = (i-header_sz[header_type])&0xff;
	}
	else
	{
	  i=header_sz[header_type];
	  memcpy(&((A_UINT8 *)pHdr)[i], pdatabuf, buflen);
	}

    /*creat meta*/
    v3_meta.pktID = 1;
    v3_meta.rateSched.accessCategory = access_cat;
    /* we set the ACK bit simply to force the desired number of retries */
    v3_meta.rateSched.flags = 0x01;
    v3_meta.rateSched.trySeries[0] = tries;
    v3_meta.rateSched.rateSeries[0] = rate_index;
    v3_meta.rateSched.rateSeries[1] = 0xff;
    memcpy(tbf->bfData.meta, &v3_meta, sizeof(WMI_TX_META_V3));
    
    /*creat wmi header*/
    memset(&wmi_data_h, 0, sizeof(WMI_DATA_HDR));
    WMI_DATA_HDR_SET_MSG_TYPE(&wmi_data_h, DATA_MSGTYPE);
    WMI_DATA_HDR_SET_DATA_TYPE(&wmi_data_h, WMI_DATA_HDR_DATA_TYPE_802_11);
    //WMI_DATA_HDR_SET_META(dhdr, WMI_META_VERSION_3);
    wmi_data_h.info2 = 0x6000; //fix CR763176
    memcpy(&tbf->bfData.hdr, &wmi_data_h, sizeof(WMI_DATA_HDR));
	//dhdr->info2 = A_CPU2LE16(dhdr->info2);

    WLAN_BUF_START(tbf) = (A_UINT8 *)(&tbf->bfData.hdr);
    WLAN_BUF_LENGTH(tbf) =  sizeof(WMI_DATA_HDR) + WMI_MAX_TX_META_SZ + size;
    bufinfo->buffer = WLAN_BUF_START(tbf);
    bufinfo->actual_length = WLAN_BUF_LENGTH(tbf);

    //A_DUMP_BUFFER(WLAN_BUF_START(tbf), WLAN_BUF_LENGTH(tbf));
    dev = devArray[devId];
    do {
        dhdr = (WMI_DATA_HDR *)bufinfo->buffer;
#if defined(WLAN_CONFIG_CCXV4_FULL) && defined(WLAN_CONFIG_CCX)
        if(dhdr != NULL){
            if(WMI_DATA_HDR_HAS_CCX_LINKTEST_BIT(dhdr)){
                CCX_LINK_TEST_FLAG_SET(dev, TRUE);
            }
        }
#endif
        /* Derive the device context from the device id in the WMI data hdr
         * in the message.
         */
        {
            /* strip off WMI header */
            bufinfo->buffer += WMI_DATA_HDR_LEN;
            bufinfo->actual_length -= WMI_DATA_HDR_LEN;
            pktInfo |= ((dhdr->info >> WMI_DATA_HDR_UP_SHIFT) & WMI_DATA_HDR_UP_MASK) << PKTINFO_UP_SHIFT;
            pktInfo |= (WMI_DATA_HDR_GET_DOT11(dhdr)) << PKTINFO_DOT11_SHIFT;
#ifndef DISABLE_META_SUPPORT
            pktInfo |= (WMI_DATA_HDR_GET_META(dhdr)) << PKTINFO_META_SHIFT;
#endif
            /* Is it ACL data? Let PAL perform frame conversion ACL->.11 (4-addr) frame
             * 4-add .11 qos frame : 32 bytes
             * LLC-SNAP hdr = 8 bytes
             * ACL payload hdr = 4 bytes
             * ACL frame will be carrying no headroom, as some host platform(QC)
             * have issues with larger block size(512). With that, it requires
             * 4-blocks of 512 for 1492 size BT frame; Target credit size is 1664
             * Target will now create headroom by moving data.
             */
            flags = 0;
            if (WMI_DATA_HDR_HAS_MORE_BIT(dhdr)) {
                flags |= WMI_DATA_HDR_FLAGS_MORE;
            }
            if (WMI_DATA_HDR_HAS_EOSP_BIT(dhdr)) {
                flags |= WMI_DATA_HDR_FLAGS_EOSP;
            }
            if (WMI_DATA_HDR_HAS_TRIGGERED_BIT(dhdr)) {
                flags |= WMI_DATA_HDR_FLAGS_TRIGGERED;
            }
            if (WMI_DATA_HDR_HAS_PSPOLLED_BIT(dhdr)) {
                flags |= WMI_DATA_HDR_FLAGS_PSPOLLED;
            }
            if (WMI_DATA_HDR_HAS_AMSDU_BIT(dhdr)) {
                flags |= WMI_DATA_HDR_FLAGS_AMSDU;
            }

/*START*/
            A_UINT8 mandatoryTx = 0;
            cb->flags |= (PKTINFO_GET_DOT11(pktInfo))? TXRX_RELAY_DOT11 : 0;
            cb->prio_8021d = ((pktInfo>>PKTINFO_UP_SHIFT) & PKTINFO_UP_MASK); /* 802.1d priority is lower 3 bits */
            cb->retry = 0;
            cb->secondaryFlags |= TXRX_GET_DEVID(dev->id);

                /* All meta data types are assumed to take up the same space WMI_MAX_TX_META_SZ */
            cb->flags |= WMI_META_VERSION_3 << TXRX_META_SHIFT;
                /* copy meta to the top of buffer space so that it doesn't get trampled by any
                * packet construction code. */
            A_MEMMOVE(&tbf->bfData.metaLoc[0], bufinfo->buffer, WMI_MAX_TX_META_SZ);

            WLAN_BUF_LENGTH(tbf) = bufinfo->actual_length - WMI_MAX_TX_META_SZ;
            WLAN_BUF_START(tbf) = bufinfo->buffer + WMI_MAX_TX_META_SZ;

            /* support for MAC_80211.  This conditional code identifies a
             * packet that should be queued straight to hardware. */
            if (gpDevCommon->metaTxFn (bufinfo->buffer,WMI_META_VERSION_3,7,&mandatoryTx) &&
                mandatoryTx){              
                /* queue this frame straight to hw dma */
                QueueFrameDirectToHW(dev, abf);
                ret = A_OK;
                break;
            }
/*END*/
        }
            /* if we get here, that packet is invalid  */
        QCOM_BEACON_DEBUG_PRINTF("incorrect wmi data hdr - dropping packet\n");
        ret = A_ERROR ;
    } while (FALSE);
    
    /* if the packet was not handed off to another module to recycle, recycle it here */
    if (ret != A_OK) {
         QCOM_BEACON_DEBUG_PRINTF ("send pkt to air failed. return %d\n",ret);
        TXRX_RecycleBuffers (dev, abf, TXRX_ERROR);
        return ret;
    }

    QCOM_BEACON_DEBUG_PRINTF("send raw pkt done.\n");
    return ret;
}


