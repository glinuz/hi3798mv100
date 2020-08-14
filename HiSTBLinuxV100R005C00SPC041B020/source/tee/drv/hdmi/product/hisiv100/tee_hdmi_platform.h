#ifndef __TEE_DRV_HDMI_PLATFORM_H__
#define __TEE_DRV_HDMI_PLATFORM_H__

// TEE HDCP AND MONITOR REGISTER BASEADDR
#define TEE_HDMI_HDCP_BASE_ADDR                     0xf8a80000
#define HDMI_CTRL_BASE_ADDR                         0xf8ce0000
// HDMI HDCP require and video mute register
#define REG_HDMI_HDCP_CTRL_ADDR                     0x0038

// Set 0 when hdcp2.2 auth  success -- bit[0]
#define BIT_MSK__HDCP_CTRL__HDCP2X_VIDEO_MUTE      (0x01 << 0)
// Set 01 when hdcp2.2 auth failed and audio data are fixed 0, set 10 when audio data are lost -- bit[2:1]
#define BIT_MSK__HDCP_CTRL__HDCP2X_AUDIO_MUTE      (0x03 << 1)
// Set 0 when hdcp1.4 auth  success -- bit[3]
#define BIT_MSK__HDCP_CTRL__HDCP1X_VIDEO_MUTE      (0x01 << 3)
// Set 01 when hdcp1.4 auth failed and audio data are fixed 0, set 10 when audio data are lost -- bit[5:4]
#define BIT_MSK__HDCP_CTRL__HDCP1X_AUDIO_MUTE      (0x03 << 4)
// Set 0 when don't permit hdmi bypass
#define BIT_MSK__HDCP_CTRL__HDMITX_DEBUG_DISABLE   (0x01 << 6)
// Set 1 when the security level is 1 (only hdcp2.2)
#define BIT_MSK__HDCP_CTRL__HDCP2_REQ              (0x01 << 8)
// Set 1 when the security level is 2 (hdcp2.2 or hdcp1.4)
#define BIT_MSK__HDCP_CTRL__HDCPX_REQ              (0x01 << 9)
// Set 0 when the vdp v0 downscale check enable
#define BIT_MSK__HDCP_CTRL__DOWNSCALE_CHECK_V0     (0x01 << 10)
// Set 0 when the vdp v1 downscale check enable
#define BIT_MSK__HDCP_CTRL__DOWNSCALE_CHECK_V1     (0x01 << 11)

// TEE HDCP 8051 BASEADDR
#define HDMI_HDCP2_CCHK_DONE                       (0x1 << 25)
#define HDMI_HDCP2_CUPD_START                      (0x1 << 15)
#define HDMI_HDCP2_CUPD_DONE                       (0x1 << 13)
#define C8051_PRAM_BASE_ADDR                        0xf8cf0000
#define PERI_HDMITX_CTRL_ADDR                       0xf8a208b0

/* 2016.02.23 EC change */
#define HDMI_HDCP2_CUPD_HW                          (0x1 << 7)
#define HDMI_HDCP2_PRAM_LOCK_EN                     (0x1 << 12) // pram write enable
#define HDMI_HDCP2_PRAM_RD_LOCK                     (0x1 << 13) // pram read enable

#define REG_ADDR__HDCP2X_TX_SRST                    0x22d4
// (ReadWrite, Bits 0) 
// HDCP Soft Reset for crystal clock domain
#define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST                                       0x01
// (ReadWrite, Bits 1) 
// HDCP Soft Reset for pixel clock domain
#define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_PRST                                       0x02
// (ReadWrite, Bits 5) 
// Software Reset for hdcp2x logic only 1 - Reset 0 - Normal operation (default).
#define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_SW_RST                                     0x20

// HDCP Interrupt0 Status Register
#define REG_ADDR__HDCP2X_INTR0                                                        (0x200c)
  // (ReadWrite, Bits 0) 
  // ro_hdcp2_auth_stat[0] : auth_done
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT0                                           0x01
  // (ReadWrite, Bits 1) 
  // ro_hdcp2_auth_stat[1] : auth_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT1                                           0x02
  // (ReadWrite, Bits 2) 
  // ro_hdcp2_auth_stat[2] : rpt_ready
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT2                                           0x04
  // (ReadWrite, Bits 3) 
  // ro_hdcp2_auth_stat[3] : hash_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT3                                           0x08
  // (ReadWrite, Bits 4) 
  // ro_hdcp2_auth_stat[4] : cchk_done
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT4                                           0x10
  // (ReadWrite, Bits 5) 
  // ro_hdcp2_auth_stat[5] : cchk_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT5                                           0x20
  // (ReadWrite, Bits 6) 
  // ro_hdcp2_auth_stat[6] : reauth_req
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT6                                           0x40
  // (ReadWrite, Bits 7) 
  // ro_hdcp2_auth_stat[7]: polling_interval
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT7                                           0x80

  //Page 8:0x0C - REG_ADDR__HDCP2X_AUTH_STAT
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_DONE			0x01
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_FAIL			0x02
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE			0x10
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL			0x20

#define HDCP2X_CCHK_FAIL								( BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE | BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL )

#define REG_ADDR__HDCP2X_AUTH_STAT                                                    (0x2030)
#define REG_ADDR__HDCP1X_AUTH_STAT                                                    (0x00fc)

/* hdcp21.4 enable status */
#define REG_ADDR__TPI_COPP_DATA2                                                      (0x18a8)
#define BIT_MSK__HDCP1X_ENABLE_STATUS                                                  0x80

// hdcp2 receiver ID from core For RX this 40-bit field is filled as soon as HDCP2x core starts running
// Receive ID [7:0]
#define HDCP2X_RX_ID_CORE_0                             (0x20a0)
// Receive ID [15:8]
#define HDCP2X_RX_ID_CORE_1                             (0x20a4)
// Receive ID [23:16]
#define HDCP2X_RX_ID_CORE_2                             (0x20a8)
// Receive ID [31:24]
#define HDCP2X_RX_ID_CORE_3                             (0x20ac)
// Receive ID [39:32]
#define HDCP2X_RX_ID_CORE_4                             (0x20b0)

// HDCP Devcnt Control Register
#define REG_ADDR_HDCP2X_RPT_DEVCNT                                      (0x20c0)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read DEVICE_COUNT  field of Receiver ID List message For RX use this bit to program DEVICE_COUNT field of Receiver ID List message
  #define BIT_MSK_HDCP2X_RPT_DEVCNT__RI_HDCP2RX_RPT_DEVCNT_B7_B0                           0xFF


#define REG_ADDR_HDCP2X_INTR2                           (0x2014)
  // ro_msg_intr[1]:ro_ekhkm_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT1            (0x02)
// REG_ADDR_HDCP2X_INTR2 mask
#define REG_ADDR_HDCP2X_INTR2_MASK                      (0x2024)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B1     (0x02)

  // HDCP Interrupt0 Status Register
#define REG_ADDR_HDCP2X_INTR0                                                       (0x200c)
    // (ReadWrite, Bits 0) 
    // ro_hdcp2_auth_stat[0] : auth_done
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT0                                           0x01
    // (ReadWrite, Bits 1) 
    // ro_hdcp2_auth_stat[1] : auth_fail
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT1                                           0x02
    // (ReadWrite, Bits 2) 
    // ro_hdcp2_auth_stat[2] : rpt_ready
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT2                                           0x04
    // (ReadWrite, Bits 3) 
    // ro_hdcp2_auth_stat[3] : hash_fail
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT3                                           0x08
    // (ReadWrite, Bits 4) 
    // ro_hdcp2_auth_stat[4] : cchk_done
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT4                                           0x10
    // (ReadWrite, Bits 5) 
    // ro_hdcp2_auth_stat[5] : cchk_fail
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT5                                           0x20
    // (ReadWrite, Bits 6) 
    // ro_hdcp2_auth_stat[6] : reauth_req
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT6                                           0x40
    // (ReadWrite, Bits 7) 
    // ro_hdcp2_auth_stat[7]: polling_interval
  #define BIT_MSK_HDCP2X_INTR0__INTR0_STAT7                                           0x80
  
  // HDCP Interrupt1 Status Register
#define REG_ADDR_HDCP2X_INTR1                                                       (0x2010)
    // (ReadWrite, Bits 0) 
    // ro_rpt_rcvid_changed
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT0                                           0x01
    // (ReadWrite, Bits 1) 
    // ro_rpt_smng_changed
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT1                                           0x02
    // (ReadWrite, Bits 2) 
    // ro_ake_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT2                                           0x04
    // (ReadWrite, Bits 3) 
    // ro_ske_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT3                                           0x08
    // (ReadWrite, Bits 4) 
    // ro_rpt_rcvid_xfer_done
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT4                                           0x10
    // (ReadWrite, Bits 5) 
    // ro_rpt_smng_xfer_done
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT5                                           0x20
    // (ReadWrite, Bits 6) 
    // ro_cert_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT6                                           0x40
    // (ReadWrite, Bits 7) 
    // ro_gp3[7]
  #define BIT_MSK_HDCP2X_INTR1__INTR1_STAT7                                           0x80
  
  // HDCP Interrupt2 Status Register
#define REG_ADDR__HDCP2X_INTR2                                                      (0x2014)
    // (ReadWrite, Bits 0) 
    // ro_msg_intr[0]:ro_km_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT0                                           0x01
    // (ReadWrite, Bits 1) 
    // ro_msg_intr[1]:ro_ekhkm_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT1                                           0x02
    // (ReadWrite, Bits 2) 
    // ro_msg_intr[2]:ro_h_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT2                                           0x04
    // (ReadWrite, Bits 3) 
    // ro_msg_intr[3]:ro_pair_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT3                                           0x08
    // (ReadWrite, Bits 4) 
    // ro_msg_intr[4]:ro_lc_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT4                                           0x10
    // (ReadWrite, Bits 5) 
    // ro_msg_intr[5]:ro_l_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT5                                           0x20
    // (ReadWrite, Bits 6) 
    // ro_msg_intr[6]:ro_vack_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT6                                           0x40
    // (ReadWrite, Bits 7) 
    // ro_msg_intr[7]:ro_mack_sent_rcvd
  #define BIT_MSK_HDCP2X_INTR2__INTR2_STAT7                                           0x80
  
  // HDCP AES Debug Register
#define REG_ADDR_HDCP2X_INTR3                                                       (0x2018)
    // (ReadWrite, Bits 0) 
    // Underrun in AES FIFO
  #define BIT_MSK_HDCP2X_INTR3__INTR3_STAT0                                           0x01
    // (ReadWrite, Bits 1) 
    // Encryption enable status changed(RX only)
  #define BIT_MSK_HDCP2X_INTR3__INTR3_STAT1                                           0x02
  
  // HDCP Interrupt0 Mask Register
#define REG_ADDR_HDCP2X_INTR0_MASK                                                  (0x201c)
    // (ReadWrite, Bits 0) 
    // Mask for INTR0[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B0                                         0x01
    // (ReadWrite, Bits 1) 
    // Mask for INTR0[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B1                                         0x02
    // (ReadWrite, Bits 2) 
    // Mask for INTR0[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B2                                         0x04
    // (ReadWrite, Bits 3) 
    // Mask for INTR0[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B3                                         0x08
    // (ReadWrite, Bits 4) 
    // Mask for INTR0[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B4                                         0x10
    // (ReadWrite, Bits 5) 
    // Mask for INTR0[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B5                                         0x20
    // (ReadWrite, Bits 6) 
    // Mask for INTR0[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B6                                         0x40
    // (ReadWrite, Bits 7) 
    // Mask for INTR0[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR0_MASK__INTR0_MASK_B7                                         0x80
  
  // HDCP Interrupt1 MaskRegister
#define REG_ADDR_HDCP2X_INTR1_MASK                                                      (0x2020)
    // (ReadWrite, Bits 0) 
    // Mask for INTR1[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B0                                         0x01
    // (ReadWrite, Bits 1) 
    // Mask for INTR1[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B1                                         0x02
    // (ReadWrite, Bits 2) 
    // Mask for INTR1[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B2                                         0x04
    // (ReadWrite, Bits 3) 
    // Mask for INTR1[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B3                                         0x08
    // (ReadWrite, Bits 4) 
    // Mask for INTR1[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B4                                         0x10
    // (ReadWrite, Bits 5) 
    // Mask for INTR1[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B5                                         0x20
    // (ReadWrite, Bits 6) 
    // Mask for INTR1[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B6                                         0x40
    // (ReadWrite, Bits 7) 
    // Mask for INTR1[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR1_MASK__INTR1_MASK_B7                                         0x80
  
  // HDCP Interrupt2 Mask Register
#define REG_ADDR_HDCP2X_INTR2_MASK                                                        (0x2024)
    // (ReadWrite, Bits 0) 
    // Mask for INTR2[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B0                                         0x01
    // (ReadWrite, Bits 1) 
    // Mask for INTR2[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B1                                         0x02
    // (ReadWrite, Bits 2) 
    // Mask for INTR2[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B2                                         0x04
    // (ReadWrite, Bits 3) 
    // Mask for INTR2[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B3                                         0x08
    // (ReadWrite, Bits 4) 
    // Mask for INTR2[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B4                                         0x10
    // (ReadWrite, Bits 5) 
    // Mask for INTR2[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B5                                         0x20
    // (ReadWrite, Bits 6) 
    // Mask for INTR2[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B6                                         0x40
    // (ReadWrite, Bits 7) 
    // Mask for INTR2[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR2_MASK__INTR2_MASK_B7                                         0x80
  
  // HDCP Interrupt3 Mask Register
#define REG_ADDR_HDCP2X_INTR3_MASK                                                        (0x2028)
    // (ReadWrite, Bits 0) 
    // Mask for INTR3[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR3_MASK__INTR3_MASK_B0                                         0x01
    // (ReadWrite, Bits 1) 
    // Mask for INTR3[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK_HDCP2X_INTR3_MASK__INTR3_MASK_B1                                         0x02

  // HDCP Misc Control 0 Register
#define REG_ADDR__HDCP2X_TX_CTRL_0                                       (0x21c0)
    // (ReadWrite, Bits 0) 
    // Valid only for Tx. Don't care for Rx. Read from Receiver ID List message buffer.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD                               0x01
    // (ReadWrite, Bits 1) 
    // Valid only for Tx. Don't care for Rx. Write 1 to reset Receiver ID List message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START                         0x02
    // (ReadWrite, Bits 2) 
    // Valid only for Tx. Don't care for Rx. Write 1 to start transfer of Stream Manage message.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START                        0x04
    // (ReadWrite, Bits 3) 
    // Valid only for Tx. Don't care for Rx. Write to Stream Manage message buffer.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR                                0x08
    // (ReadWrite, Bits 4) 
    // Valid only for Tx. Don't care for Rx. Write 1 to reset Stream Manage message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START                          0x10

  // HDCP RPT RCVID Out Register
#define REG_ADDR_HDCP2X_TX_RPT_RCVID_OUT                                (0x21cc)
    // (ReadOnly, Bits 7:0) 
    // Valid only for Tx. Don't care for Rx. Data output port for Receiver ID List message buffer.
  #define BIT_MSK_HDCP2X_TX_RPT_RCVID_OUT__RO_HDCP2TX_RPT_RCVID_OUT_B7_B0                        0xFF


    // HDCP Polling Control and Status Register
#define REG_ADDR__HDCP2X_POLL_CS                                         (0x22d8)
  
  #define BIT_MSK__HDCP2X_DS_POLL_EN                                                     0x01

  // HDCP General Control 2 Register
#define REG_ADDR__HDCP2X_CTRL_2                                          (0x2008)
    // (ReadWrite, Bits 0) 
    // Active high long pulse Write 1 to this register to start patch code check. Write back to 0 after code check finishes.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_DONE                                    0x01
    // (ReadWrite, Bits 1) 
    // Active high long pulse Write 1 to this register to gain write access to PRAM for patch.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_START                                   0x02
    // (ReadWrite, Bits 2) 
    // 1(default): Use i_hw_cupd_start/i_hw_cupd_done. 0: Use ri_hdcp2_cupd_start/ri_hdcp2_cupd_done.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_HW                                      0x04
    // (ReadWrite, Bits 7:4) 
    // 4'hA for HDCP 2.2 for HDMI or MHL. Do not change this regiseter.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CPVER_B3_B0                                  0xF0

#define BIT_MSK__HDCP2X_ENABLE_STATUS                                                  0x01

  // HDCP General Control 0 Register
#define REG_ADDR__HDCP2X_CTL_0                                           (0x22f4)
    // (ReadWrite, Bits 0) 
    // HDCP 2.x Enable When enabled video encryption mux and ddc mux will select HDCP 2.x. However this signal doesn't go to the hdcp2xcore
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN                                         0x01
    // (ReadWrite, Bits 5) 
    // DDC polling interval override
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_POLINT_OVR                                 0x20
    // (ReadWrite, Bits 6) 
    // DDC polling interval select
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_POLINT_SEL                                 0x40
    // (ReadWrite, Bits 7) 
    // HDCP 2.x Encryption Enable This goes to the TX hdmi block to generate CTL3 signal 0: Disable 1: Enable
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN                                 0x80

// TEE monitor output control addr 
#define SEC_MONITOR_OUTPUT_CTRL_ADDR              0x0020
  
#define MASK_V0_TRUSTZONE_CTRL                    (0x01 << 18)
#define MASK_V1_TRUSTZONE_CTRL                    (0x01 << 17)
#define MASK_V3_TRUSTZONE_CTRL                    (0x01 << 16)
#define MASK_V4_TRUSTZONE_CTRL                    (0x01 << 15)



#endif
