/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/01/25
 * Description        : ???USB?????��???????????????
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

#include "CH58x_common.h"
#include "usbdesc.h"
#include "hid_configuration.h"
#include "xprintf.h"
#include "report.h"
#ifdef RGB_TOP_ENABLE
#include "i2c_master.h"
#endif
#define DevEP0SIZE    0x40

/**********************************************************/
uint8_t        DevConfig;
uint8_t        SetupReqCode;
uint16_t       SetupReqLen;
const uint8_t *pDescr;
uint8_t        Report_Value = 0x00;
uint8_t        Idle_Value = 0x00;
uint8_t        USB_SleepStatus = 0x00; /* USB????? */

uint8_t debug_data[64] = {0};
uint8_t nrko_report[NKRO_EPSIZE + 1] = {0};
uint8_t extra_report[3] = {0};
uint8_t mouse_report[6] = {0x1,0,0,0,0,0};
/******** 用户自定义分配端点RAM ****************************************/
__attribute__((aligned(4))) uint8_t EP0_Databuf[64 + 64 + 64]; //ep0(64)+ep4_out(64)+ep4_in(64)
__attribute__((aligned(4))) uint8_t EP1_Databuf[64 + 64];      //ep1_out(64)+ep1_in(64)
__attribute__((aligned(4))) uint8_t EP2_Databuf[64 + 64];      //ep2_out(64)+ep2_in(64)
__attribute__((aligned(4))) uint8_t EP3_Databuf[64 + 64];      //ep3_out(64)+ep3_in(64)

/*********************************************************************
 * @fn      USB_DevTransProcess
 *
 * @brief   USB ??????????
 *
 * @return  none
 */
void USB2_DevTransProcess(void)
{
    uint8_t len, chtype;
    uint8_t intflag, errflag = 0;

    intflag = R8_USB2_INT_FG;
    if(intflag & RB_UIF_TRANSFER)
    {
        if((R8_USB2_INT_ST & MASK_UIS_TOKEN) != MASK_UIS_TOKEN) // �ǿ���
        {
            switch(R8_USB2_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
            // �����������ƺͶ˵��
            {
                case UIS_TOKEN_IN:
                {
                    switch(SetupReqCode)
                    {
                        case USB_GET_DESCRIPTOR:
                            len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen; // ���δ��䳤��
                            memcpy(pU2EP0_DataBuf, pDescr, len);                        /* �����ϴ����� */
                            SetupReqLen -= len;
                            pDescr += len;
                            R8_U2EP0_T_LEN = len;
                            R8_U2EP0_CTRL ^= RB_UEP_T_TOG; // ��ת
                            break;
                        case USB_SET_ADDRESS:
                            R8_USB2_DEV_AD = (R8_USB2_DEV_AD & RB_UDA_GP_BIT) | SetupReqLen;
                            R8_U2EP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            break;

                        case USB_SET_FEATURE:
                            break;

                        default:
                            R8_U2EP0_T_LEN = 0; // ״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ���
                            R8_U2EP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            break;
                    }
                }
                break;

                case UIS_TOKEN_OUT:
                {
                    len = R8_USB2_RX_LEN;
                    if(SetupReqCode == 0x09)
                    {
                    }
                }
                break;

                case UIS_TOKEN_OUT | 1:
                {
                    if(R8_USB2_INT_ST & RB_UIS_TOG_OK)
                    { // ��ͬ�������ݰ�������
                        R8_U2EP1_CTRL ^= RB_UEP_R_TOG;
                        len = R8_USB2_RX_LEN;
                        U2DevEP1_OUT_Deal(len);
                    }
                }
                break;

                case UIS_TOKEN_IN | 1:
                    R8_U2EP1_CTRL ^= RB_UEP_T_TOG;
                    R8_U2EP1_CTRL = (R8_U2EP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;

                case UIS_TOKEN_OUT | 2:
                {
                    if(R8_USB2_INT_ST & RB_UIS_TOG_OK)
                    { // ��ͬ�������ݰ�������
                        R8_U2EP2_CTRL ^= RB_UEP_R_TOG;
                        len = R8_USB2_RX_LEN;
                        U2DevEP2_OUT_Deal(len);
                    }
                }
                break;

                case UIS_TOKEN_IN | 2:
                    R8_U2EP2_CTRL ^= RB_UEP_T_TOG;
                    R8_U2EP2_CTRL = (R8_U2EP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;

                case UIS_TOKEN_OUT | 3:
                {
                    if(R8_USB2_INT_ST & RB_UIS_TOG_OK)
                    { // ��ͬ�������ݰ�������
                        R8_U2EP3_CTRL ^= RB_UEP_R_TOG;
                        len = R8_USB2_RX_LEN;
                        U2DevEP3_OUT_Deal(len);
                    }
                }
                break;

                case UIS_TOKEN_IN | 3:
                    R8_U2EP3_CTRL ^= RB_UEP_T_TOG;
                    R8_U2EP3_CTRL = (R8_U2EP3_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;

                case UIS_TOKEN_OUT | 4:
                {
                    if(R8_USB2_INT_ST & RB_UIS_TOG_OK)
                    {
                        R8_U2EP4_CTRL ^= RB_UEP_R_TOG;
                        len = R8_USB2_RX_LEN;
                        U2DevEP4_OUT_Deal(len);
                    }
                }
                break;

                case UIS_TOKEN_IN | 4:
                    R8_U2EP4_CTRL ^= RB_UEP_T_TOG;
                    R8_U2EP4_CTRL = (R8_U2EP4_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK;
                    break;

                default:
                    break;
            }
            R8_USB2_INT_FG = RB_UIF_TRANSFER;
        }
        if(R8_USB2_INT_ST & RB_UIS_SETUP_ACT) // Setup������
        {
            R8_U2EP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
            SetupReqLen = pU2SetupReqPak->wLength;
            SetupReqCode = pU2SetupReqPak->bRequest;
            chtype = pU2SetupReqPak->bRequestType;

            len = 0;
            errflag = 0;
            if((pU2SetupReqPak->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD)
            {
                /* �Ǳ�׼���� */
                /* ��������,�������󣬲�������� */
                if(pU2SetupReqPak->bRequestType & 0x40)
                {
                    /* �������� */
                }
                else if(pU2SetupReqPak->bRequestType & 0x20)
                {
                    switch(SetupReqCode)
                    {
                        case DEF_USB_SET_IDLE: /* 0x0A: SET_IDLE */
                            Idle_Value = EP0_Databuf[3];
                            break; //���һ��Ҫ��

                        case DEF_USB_SET_REPORT: /* 0x09: SET_REPORT */
                            break;

                        case DEF_USB_SET_PROTOCOL: /* 0x0B: SET_PROTOCOL */
                            Report_Value = EP0_Databuf[2];
                            break;

                        case DEF_USB_GET_IDLE: /* 0x02: GET_IDLE */
                            EP0_Databuf[0] = Idle_Value;
                            len = 1;
                            break;

                        case DEF_USB_GET_PROTOCOL: /* 0x03: GET_PROTOCOL */
                            EP0_Databuf[0] = Report_Value;
                            len = 1;
                            break;

                        default:
                            errflag = 0xFF;
                    }
                }
            }
            else /* ��׼���� */
            {
                switch(SetupReqCode)
                {
                    case USB_GET_DESCRIPTOR:
                    {
                        switch(((pU2SetupReqPak->wValue) >> 8))
                        {
                            case USB_DESCR_TYP_DEVICE:
                            {
                                pDescr = USB_DeviceDescriptor;
                                len = USB_DeviceDescriptor[0];
                            }
                            break;

                            case USB_DESCR_TYP_CONFIG:
                            {
                                pDescr = USB_ConfigDescriptor;
                                len = nUsb_TotalLength;
                            }
                            break;

                            case USB_DESCR_TYP_REPORT:
                            {
                                if(((pU2SetupReqPak->wIndex) & 0xff) == 0) //�ӿ�0����������
                                {
                                    pDescr = HID_ReportDescriptor; //??????????
                                    len = HID_ReportDescSize;
                                }
                                else if(((pU2SetupReqPak->wIndex) & 0xff) == 1) //�ӿ�1����������
                                {
                                    pDescr = HID_ReportDescriptor2; //??????????
                                    len = HID_ReportDescSize2;
                                }
                                else if(((pU2SetupReqPak->wIndex) & 0xff) == 2) //�ӿ�1����������
                                {
                                    pDescr = HID_ReportDescriptor3; //??????????
                                    len = HID_ReportDescSize3;
                                }
                                else
                                    len = 0xff; //������ֻ��2���ӿڣ���仰����������ִ��
                            }
                            break;

                            case USB_DESCR_TYP_STRING:
                            {
                                switch((pU2SetupReqPak->wValue) & 0xff)
                                {
                                    case 1:
                                    pDescr = USB_ManufacturerStringDescriptor;
                                    len = USB_ManufacturerStringDescriptor[0];
                                        break;
                                    case 2:
                                    pDescr = USB_ProductStringDescriptor;
                                    len = USB_ProductStringDescriptor[0];
                                        break;
                                    case 0:
                                    pDescr = USB_SerialNumberStringDescriptor;
                                    len = USB_SerialNumberStringDescriptor[0];
                                        break;
                                    default:
                                        errflag = 0xFF; // ��֧�ֵ��ַ���������
                                        break;
                                }
                            }
                            break;

                            default:
                                errflag = 0xff;
                                break;
                        }
                        if(SetupReqLen > len)
                            SetupReqLen = len; //ʵ�����ϴ��ܳ���
                        len = (SetupReqLen >= DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
                        memcpy(pU2EP0_DataBuf, pDescr, len);
                        pDescr += len;
                    }
                    break;

                    case USB_SET_ADDRESS:
                        SetupReqLen = (pU2SetupReqPak->wValue) & 0xff;
                        break;

                    case USB_GET_CONFIGURATION:
                        pU2EP0_DataBuf[0] = DevConfig;
                        if(SetupReqLen > 1)
                            SetupReqLen = 1;
                        break;

                    case USB_SET_CONFIGURATION:
                        DevConfig = (pU2SetupReqPak->wValue) & 0xff;
                        break;

                    case USB_CLEAR_FEATURE:
                    {
                        if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP) // �˵�
                        {
                            switch((pU2SetupReqPak->wIndex) & 0xff)
                            {
                                case 0x83:
                                    R8_U2EP3_CTRL = (R8_U2EP3_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x03:
                                    R8_U2EP3_CTRL = (R8_U2EP3_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x82:
                                    R8_U2EP2_CTRL = (R8_U2EP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x02:
                                    R8_U2EP2_CTRL = (R8_U2EP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x81:
                                    R8_U2EP1_CTRL = (R8_U2EP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x01:
                                    R8_U2EP1_CTRL = (R8_U2EP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                default:
                                    errflag = 0xFF; // ��֧�ֵĶ˵�
                                    break;
                            }
                        }
                        else if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE)
                        {
                            if(pU2SetupReqPak->wValue == 1)
                            {
                                USB_SleepStatus &= ~0x01;
                            }
                        }
                        else
                        {
                            errflag = 0xFF;
                        }
                    }
                    break;

                    case USB_SET_FEATURE:
                        if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)
                        {
                            /* �˵� */
                            switch(pU2SetupReqPak->wIndex)
                            {
                                case 0x83:
                                    R8_U2EP3_CTRL = (R8_U2EP3_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x03:
                                    R8_U2EP3_CTRL = (R8_U2EP3_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x82:
                                    R8_U2EP2_CTRL = (R8_U2EP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x02:
                                    R8_U2EP2_CTRL = (R8_U2EP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x81:
                                    R8_U2EP1_CTRL = (R8_U2EP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x01:
                                    R8_U2EP1_CTRL = (R8_U2EP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                default:
                                    /* ��֧�ֵĶ˵� */
                                    errflag = 0xFF; // ��֧�ֵĶ˵�
                                    break;
                            }
                        }
                        else if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE)
                        {
                            if(pU2SetupReqPak->wValue == 1)
                            {
                                /* ����˯�� */
                                USB_SleepStatus |= 0x01;
                            }
                        }
                        else
                        {
                            errflag = 0xFF;
                        }
                        break;

                    case USB_GET_INTERFACE:
                        pU2EP0_DataBuf[0] = 0x00;
                        if(SetupReqLen > 1)
                            SetupReqLen = 1;
                        break;

                    case USB_SET_INTERFACE:
                        break;

                    case USB_GET_STATUS:
                        if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP)
                        {
                            /* �˵� */
                            pU2EP0_DataBuf[0] = 0x00;
                            switch(pU2SetupReqPak->wIndex)
                            {
                                case 0x83:
                                    if((R8_U2EP3_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;

                                case 0x03:
                                    if((R8_U2EP3_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;

                                case 0x82:
                                    if((R8_U2EP2_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;

                                case 0x02:
                                    if((R8_U2EP2_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;

                                case 0x81:
                                    if((R8_U2EP1_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;

                                case 0x01:
                                    if((R8_U2EP1_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL)
                                    {
                                        pU2EP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                            }
                        }
                        else if((pU2SetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE)
                        {
                            pU2EP0_DataBuf[0] = 0x00;
                            if(USB_SleepStatus)
                            {
                                pU2EP0_DataBuf[0] = 0x02;
                            }
                            else
                            {
                                pU2EP0_DataBuf[0] = 0x00;
                            }
                        }
                        pU2EP0_DataBuf[1] = 0;
                        if(SetupReqLen >= 2)
                        {
                            SetupReqLen = 2;
                        }
                        break;

                    default:
                        errflag = 0xff;
                        break;
                }
            }
            if(errflag == 0xff) // �����֧��
            {
                //                  SetupReqCode = 0xFF;
                R8_U2EP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL; // STALL
            }
            else
            {
                if(chtype & 0x80) // �ϴ�
                {
                    len = (SetupReqLen > DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
                    SetupReqLen -= len;
                }
                else
                    len = 0; // �´�
                R8_U2EP0_T_LEN = len;
                R8_U2EP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK; // Ĭ�����ݰ���DATA1
            }

            R8_USB2_INT_FG = RB_UIF_TRANSFER;
        }
    }
    else if(intflag & RB_UIF_BUS_RST)
    {
        R8_USB2_DEV_AD = 0;
        R8_U2EP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_U2EP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_U2EP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_U2EP3_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_USB2_INT_FG = RB_UIF_BUS_RST;
    }
    else if(intflag & RB_UIF_SUSPEND)
    {
        if(R8_USB2_MIS_ST & RB_UMS_SUSPEND)
        {
            ;
        } // ����
        else
        {
            ;
        } // ����
        R8_USB2_INT_FG = RB_UIF_SUSPEND;
    }
    else
    {
        R8_USB2_INT_FG = intflag;
    }
}

void usb_send_key(uint8_t report_id, uint8_t len, uint8_t *report){
    switch (report_id)
    {
    case 0:
        memcpy(pU2EP1_IN_DataBuf, report, len);
        U2DevEP1_IN_Deal(len);
        break;
    case REPORT_ID_MOUSE:
        memcpy(&mouse_report[1], report, len);
        memcpy(pU2EP2_IN_DataBuf, mouse_report, len+1);
        U2DevEP2_IN_Deal(len+1);
        break;
    case REPORT_ID_SYSTEM ... REPORT_ID_CONSUMER:
        extra_report[0] = report_id;
        extra_report[1] = *report & 0xFF;
        extra_report[2] = *(report+1) & 0xFF;
        memcpy(pU2EP2_IN_DataBuf, extra_report, len+1);
        U2DevEP2_IN_Deal(len+1);
        break;
#ifdef NKRO_ENABLE
    case 0x80:
        nrko_report[0] = 0x80;
        memcpy(nrko_report + 1, report, len);
        memcpy(pU2EP2_IN_DataBuf, nrko_report, len+1);
        U2DevEP2_IN_Deal(len+1);
        break;
#endif
    default:
        break;
    }
}

/*********************************************************************
 * @fn      DevHIDKeyReport
 *
 * @brief   ???print
 *
 * @return  none
 */
void hid_send_data(uint8_t len ,uint8_t* data)
{
    uint8_t buff[64];
    buff[0] = 0x3f;
    buff[1] = len;
    memcpy(&buff[2], data, len);
    memcpy(pU2EP3_IN_DataBuf, buff, 64);
    U2DevEP3_IN_Deal(64);
}
/*********************************************************************
 * @fn      DevWakeup
 *
 * @brief   ?��??????????
 *
 * @return  none
 */
void DevWakeup(void)
{
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB2_DP_PU);
    R8_U2DEV_CTRL |= RB_UD_LOW_SPEED;
    mDelaymS(2);
    R8_U2DEV_CTRL &= ~RB_UD_LOW_SPEED;
    R16_PIN_ANALOG_IE |= RB_PIN_USB2_DP_PU;
}

/*********************************************************************
 * @fn      DevEP1_OUT_Deal
 *
 * @brief   ???1???????
 *
 * @return  none
 */
void U2DevEP1_OUT_Deal(uint8_t l)
{ /* ?????????? */
    uint8_t i;

    for(i = 0; i < l; i++)
    {
        pU2EP1_IN_DataBuf[i] = ~pU2EP1_OUT_DataBuf[i];
    }
    U2DevEP1_IN_Deal(l);
}

/*********************************************************************
 * @fn      DevEP2_OUT_Deal
 *
 * @brief   ???2???????
 *
 * @return  none
 */
void U2DevEP2_OUT_Deal(uint8_t l)
{ /* ?????????? */
    uint8_t i;

    for(i = 0; i < l; i++)
    {
        pU2EP2_IN_DataBuf[i] = ~pU2EP2_OUT_DataBuf[i];
    }
    U2DevEP2_IN_Deal(l);
}

/*********************************************************************
 * @fn      DevEP3_OUT_Deal
 *
 * @brief   ???3???????
 *
 * @return  none
 */
void U2DevEP3_OUT_Deal(uint8_t l)
{ 
    if (pU2EP3_OUT_DataBuf[0] == 0x3f)
    {
        hid_on_recv(pU2EP3_OUT_DataBuf[1], pU2EP3_OUT_DataBuf[2], &pU2EP3_OUT_DataBuf[3]);
    }
}

/*********************************************************************
 * @fn      DevEP4_OUT_Deal
 *
 * @brief   ???4???????
 *
 * @return  none
 */
void U2DevEP4_OUT_Deal(uint8_t l)
{ /* ?????????? */
    uint8_t i;

    for(i = 0; i < l; i++)
    {
        pU2EP4_IN_DataBuf[i] = ~pU2EP4_OUT_DataBuf[i];
    }
    U2DevEP4_IN_Deal(l);
}

void USB_Init(void){
    pU2EP0_RAM_Addr = EP0_Databuf;
    pU2EP1_RAM_Addr = EP1_Databuf;
    pU2EP2_RAM_Addr = EP2_Databuf;
    pU2EP3_RAM_Addr = EP3_Databuf;

    USB2_DeviceInit();

    PFIC_EnableIRQ(USB2_IRQn);
}
/*********************************************************************
 * @fn      USB_IRQHandler
 *
 * @brief   USB?��????
 *
 * @return  none
 */
__attribute__((section(".highcode")))
void USB2_IRQHandler(void) /* USB?��???????,??��??????1 */
{
    USB2_DevTransProcess();
}

