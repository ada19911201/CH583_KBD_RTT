/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Custom User Module
 * Version: V1.01
 * Date:        2013/11
 *------------------------------------------------------------------------------*/

#include "CH58x_common.h"
#include "usbdesc.h"
#include "hid.h"
#include "report.h"
#include <stdint.h>

#define NKRO_REPORT_KEYS      (NKRO_EPSIZE - 1)
// 设备描述符
/*****************************************************************************
* Description   : USB_DeviceDescriptor[]
*****************************************************************************/
const uint8_t USB_DeviceDescriptor[] =
{
    USB_DEVICE_DESC_SIZE,                   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,             /* bDescriptorType */
    USB_WBVAL(0x0200), /* 2.00 */           /* bcdUSB */
    0x00,                                   /* bDeviceClass */
    0x00,                                   /* bDeviceSubClass */
    0x00,                                   /* bDeviceProtocol */
    USB_EP0_PACKET_SIZE,                    /* bMaxPacketSize0 */
    USB_WBVAL(USB_VID),                     /* idVendor */
    USB_WBVAL(USB_PID),                     /* idProduct */
    USB_WBVAL(USB_REV),                     /* bcdDevice */
    USB_DEVICE_STRING_MANUFACTURER,         /* iManufacturer */
    USB_DEVICE_STRING_PRODUCT,              /* iProduct */
    USB_DEVICE_STRING_RESERVED,             /* iSerialNumber */
    0x01,                                   /* bNumConfigurations: one possible configuration*/
};


/*****************************************************************************
* Description   : HID_ReportDescriptor[]
*****************************************************************************/
const uint8_t HID_ReportDescriptor[] =
{
        0x05, 0x01,                    /* USAGE_PAGE (Generic Desktop)                   */
        0x09, 0x06,                    /* USAGE (Keyboard)                               */
        0xa1, 0x01,                    /* COLLECTION (Application)                       */
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */
        0x19, 0xe0,                    /*   USAGE_MINIMUM (Keyboard LeftControl)         */
        0x29, 0xe7,                    /*   USAGE_MAXIMUM (Keyboard Right GUI)           */
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */
        0x25, 0x01,                    /*   LOGICAL_MAXIMUM (1)                          */
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */
        0x95, 0x08,                    /*   REPORT_COUNT (8)                             */
        0x81, 0x02,                    /*   INPUT (Data,Var,Abs)                         */
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */
        0x81, 0x03,                    /*   INPUT (Cnst,Var,Abs)                         */
        0x95, 0x05,                    /*   REPORT_COUNT (5)                             */
        0x75, 0x01,                    /*   REPORT_SIZE (1)                              */
        0x05, 0x08,                    /*   USAGE_PAGE (LEDs)                            */
        0x19, 0x01,                    /*   USAGE_MINIMUM (Num Lock)                     */
        0x29, 0x05,                    /*   USAGE_MAXIMUM (Kana)                         */
        0x91, 0x02,                    /*   OUTPUT (Data,Var,Abs)                        */
        0x95, 0x01,                    /*   REPORT_COUNT (1)                             */
        0x75, 0x03,                    /*   REPORT_SIZE (3)                              */
        0x91, 0x03,                    /*   OUTPUT (Cnst,Var,Abs)                        */
        0x95, 0x06,                    /*   REPORT_COUNT (6)                             */
        0x75, 0x08,                    /*   REPORT_SIZE (8)                              */
        0x15, 0x00,                    /*   LOGICAL_MINIMUM (0)                          */
        0x25, 0x66,                    /*   LOGICAL_MAXIMUM (101)                        */
        0x05, 0x07,                    /*   USAGE_PAGE (Keyboard)                        */
        0x19, 0x00,                    /*   USAGE_MINIMUM (Reserved (no event indicated))*/
        0x29, 0x66,                    /*   USAGE_MAXIMUM (Keyboard Application)         */
        0x81, 0x00,                    /*   INPUT (Data,Ary,Abs)                         */
        0xc0                           /* END_COLLECTION                                 */
};
const uint16_t HID_ReportDescSize = sizeof(HID_ReportDescriptor);


/*****************************************************************************
* Description   : HID_ReportDescriptor2[]
*****************************************************************************/
const uint8_t HID_ReportDescriptor2[] =
{
            0x05, 0x01,                      /* USAGE_PAGE (Generic Desktop)*/
            0x09, 0x80,                      /* USAGE (System Control)*/
            0xa1, 0x01,                      /* COLLECTION (Application)*/
            0x85, REPORT_ID_SYSTEM,          /*   REPORT_ID (2)*/
            0x15, 0x01,                      /*   LOGICAL_MINIMUM (0x1)*/
            0x26, 0xb7, 0x00,                /*   LOGICAL_MAXIMUM (0xb7)*/
            0x19, 0x01,                      /*   USAGE_MINIMUM (0x1)*/    
            0x29, 0xb7,                      /*   USAGE_MAXIMUM (0xb7)*/    
            0x75, 0x10,                      /*   REPORT_SIZE (16)*/    
            0x95, 0x01,                      /*   REPORT_COUNT (1)*/    
            0x81, 0x00,                      /*   INPUT (Data,Array,Abs)*/    
            0xc0,                            /* END_COLLECTION*/    
            /* consumer */    
            0x05, 0x0c,                      /* USAGE_PAGE (Consumer Devices)*/    
            0x09, 0x01,                      /* USAGE (Consumer Control)*/    
            0xa1, 0x01,                      /* COLLECTION (Application)*/    
            0x85, REPORT_ID_CONSUMER,        /*   REPORT_ID (3)*/    
            0x15, 0x01,                      /*   LOGICAL_MINIMUM (0x1)*/    
            0x26, 0x9c, 0x02,                /*   LOGICAL_MAXIMUM (0x29c)*/    
            0x19, 0x01,                      /*   USAGE_MINIMUM (0x1)*/    
            0x2a, 0x9c, 0x02,                /*   USAGE_MAXIMUM (0x29c)*/    
            0x75, 0x10,                      /*   REPORT_SIZE (16)*/    
            0x95, 0x01,                      /*   REPORT_COUNT (1)*/    
            0x81, 0x00,                      /*   INPUT (Data,Array,Abs)*/    
            0xc0,                            /* END_COLLECTION*/    
            /* nkro */    
            0x05, 0x01,                      /* Usage Page (Generic Desktop),*/  
            0x09, 0x06,                      /* Usage (Keyboard),*/    
            0xA1, 0x01,                      /* Collection (Application),*/ 
            0x85, 0x80,                      /*   REPORT_ID (128)*/    
            /* bitmap of modifiers   */                                   
            0x75, 0x01,                      /*   Report Size (1),*/    
            0x95, 0x08,                      /*   Report Count (8),*/    
            0x05, 0x07,                      /*   Usage Page (Key Codes),*/    
            0x19, 0xE0,                      /*   Usage Minimum (224),*/    
            0x29, 0xE7,                      /*   Usage Maximum (231),*/    
            0x15, 0x00,                      /*   Logical Minimum (0),*/    
            0x25, 0x01,                      /*   Logical Maximum (1),*/    
            0x81, 0x02,                      /*   Input (Data, Variable, Absolute), ;Modifier byte*/    
            /* bitmap of keys*/    
            0x95, NKRO_REPORT_KEYS * 8,      /*   Report Count (),*/    
            0x75, 0x01,                      /*   Report Size (1),*/    
            0x15, 0x00,                      /*   Logical Minimum (0),*/    
            0x25, 0x01,                      /*   Logical Maximum(1),*/    
            0x05, 0x07,                      /*   Usage Page (Key Codes),*/    
            0x19, 0x00,                      /*   Usage Minimum (0),*/    
            0x29, (NKRO_REPORT_KEYS * 8 - 1) & 0xFF,  /*   Usage Maximum (),*/   
            0x81, 0x02,                      /*   Input (Data, Variable, Absolute),*/    
            0xc0,                            /* End Collection*/    
            /* mouse */    
            0x05, 0x01, /* USAGE_PAGE (Generic Desktop)*/    
            0x09, 0x02, /* USAGE (Mouse)*/    
            0xa1, 0x01, /* COLLECTION (Application)*/    
            0x85, 0x01, /*   REPORT_ID (1)*/    
            0x09, 0x01, /*   USAGE (Pointer)*/    
            0xa1, 0x00, /*   COLLECTION (Physical)*/    
            /* ----------------------------  Buttons*/    
            0x05, 0x09, /*     USAGE_PAGE (Button)*/    
            0x19, 0x01, /*     USAGE_MINIMUM (Button 1)*/    
            0x29, 0x05, /*     USAGE_MAXIMUM (Button 5)*/    
            0x15, 0x00, /*     LOGICAL_MINIMUM (0)*/    
            0x25, 0x01, /*     LOGICAL_MAXIMUM (1)*/    
            0x75, 0x01, /*     REPORT_SIZE (1)*/    
            0x95, 0x05, /*     REPORT_COUNT (5)*/    
            0x81, 0x02, /*     INPUT (Data,Var,Abs)*/    
            0x75, 0x03, /*     REPORT_SIZE (3)*/    
            0x95, 0x01, /*     REPORT_COUNT (1)*/    
            0x81, 0x03, /*     INPUT (Cnst,Var,Abs)*/    
            /* ----------------------------  X,Y position*/    
            0x05, 0x01, /*     USAGE_PAGE (Generic Desktop)*/    
            0x09, 0x30, /*     USAGE (X)*/    
            0x09, 0x31, /*     USAGE (Y)*/    
            0x15, 0x81, /*     LOGICAL_MINIMUM (-127)*/    
            0x25, 0x7f, /*     LOGICAL_MAXIMUM (127)*/    
            0x75, 0x08, /*     REPORT_SIZE (8)*/    
            0x95, 0x02, /*     REPORT_COUNT (2)*/    
            0x81, 0x06, /*     INPUT (Data,Var,Rel)*/    
            /* ----------------------------  Vertical wheel*/    
            0x09, 0x38, /*     USAGE (Wheel)*/    
            0x15, 0x81, /*     LOGICAL_MINIMUM (-127)*/    
            0x25, 0x7f, /*     LOGICAL_MAXIMUM (127)*/    
            0x35, 0x00, /*     PHYSICAL_MINIMUM (0)        - reset physical*/ 
            0x45, 0x00, /*     PHYSICAL_MAXIMUM (0)*/    
            0x75, 0x08, /*     REPORT_SIZE (8)*/    
            0x95, 0x01, /*     REPORT_COUNT (1)*/    
            0x81, 0x06, /*     INPUT (Data,Var,Rel)*/    
            /* ----------------------------  Horizontal wheel*/ 
            0x05, 0x0c, /*     USAGE_PAGE (Consumer Devices)*/    
            0x0a, 0x38, 0x02, /*     USAGE (AC Pan)*/    
            0x15, 0x81, /*     LOGICAL_MINIMUM (-127)*/    
            0x25, 0x7f, /*     LOGICAL_MAXIMUM (127)*/    
            0x75, 0x08, /*     REPORT_SIZE (8)*/    
            0x95, 0x01, /*     REPORT_COUNT (1)*/    
            0x81, 0x06, /*     INPUT (Data,Var,Rel)*/    
            0xc0, /*   END_COLLECTION*/
            0xc0, /* END_COLLECTION*/

};
const uint16_t HID_ReportDescSize2 = sizeof(HID_ReportDescriptor2);

/*****************************************************************************
* Description   : HID_ReportDescriptor3[]
*****************************************************************************/
const uint8_t HID_ReportDescriptor3[] =
{
            0x06, 0x00, 0xff,    /* Usage Page (Vendor Defined)*/
            0x09, 0x00,    /* Usage Page (Vendor Defined)*/   
            0xa1, 0x01,    /* COLLECTION (Application)*/   
            0x09, 0x20,    /* Usage Page (Vendor Defined)*/   
            0x85, 0x3f,    /* Report ID (Vendor Defined)*/   
            0x26, 0xff, 0x00,    /* Usage Maximum*/   
            0x15, 0x00,    /* Usage Minimum*/   
            0x75, 0x08,    /* Report Size*/   
            0x95, 0x3F,    /* Report Count*/   
            0x81, 0x02,    /* Input (Data,Var,Abs)*/   
            0x09, 0x20,    /* Usage Page*/   
            0x85, 0x3f,    /* Report ID (Vendor Defined)*/   
            0x26, 0xff, 0x00,    /* Usage Maximum*/   
            0x15, 0x00,    /* Usage Minimum*/   
            0x75, 0x08,    /* Report Size*/   
            0x95, 0x3F,    /* Report Count*/   
            0x91 ,0x02,    /* Ouput (Data,Var,Abs)*/   
            0xc0    /* end Application Collection*/
};
const uint16_t HID_ReportDescSize3 = sizeof(HID_ReportDescriptor3);



#define nUsb_NumInterfaces  3
const uint16_t nUsb_TotalLength = 98;

const uint8_t USB_ConfigDescriptor[] =
{
    /* Configuration 1 */
    USB_CONFIGUARTION_DESC_SIZE,            /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType */
    nUsb_TotalLength, 0x00,             /* wTotalLength */
    nUsb_NumInterfaces,                     /* bNumInterfaces */
    USB_CONFIG_VALUE,                       /* bConfigurationValue */
    0x00,                                   /* iConfiguration */
    0xA0,
    0xC8,                                   /* bMaxPower */

/*****************************************************************************
* Description   : Interface 0[]
*****************************************************************************/
    /* Interface 0, Alternate Setting 0, HID Class */
    USB_INTERFACE_DESC_SIZE,                /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,          /* bDescriptorType */
    USB_INTERFACE_0,                        /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x02,                                   /* bNumEndpoints */
    0x03,                                   /* bInterfaceClass */
    HID_SUBCLASS_BOOT,                      /* bInterfaceSubClass */
    HID_PROTOCOL_KEYBOARD,
    0x00,                                   /* iInterface */

    /* HID Class Descriptor */
    USB_HID_DESC_SIZE,                      /* bLength */
    HID_HID_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x01,0x01,                            // HID Revision number 1.01
    0x00,                                   /* bCountryCode */
    0x01,                                   /* bNumDescriptors */
    HID_REPORT_DESCRIPTOR_TYPE,             /* bDescriptorType */
    (HID_ReportDescSize & 0x0ff),     // Total length of report descriptor
    (HID_ReportDescSize > 0xff ? HID_ReportDescSize >> 8 : 0x00),

    /* Endpoint1, HID Interrupt In */
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x81,              /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */
    0x40, 0x00,                         // wMaxPacketSize, 64 bytes
    0x01,          /* 1ms */                /* bInterval */
    /* Endpoint1, HID Interrupt Out */
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x01,               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */
    0x40, 0x00,                         // wMaxPacketSize, 64 bytes
    0x01,          /* 1ms */                /* bInterval */

/*****************************************************************************
* Description   : Interface 1[]
*****************************************************************************/
    /* Interface 1, Alternate Setting 0, HID Class */
    USB_INTERFACE_DESC_SIZE,                /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,          /* bDescriptorType */
    USB_INTERFACE_1,                        /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x01,                                   /* bNumEndpoints */
    0x03,                                   /* bInterfaceClass */
    0,                                      /* bInterfaceSubClass */
    0,                                      /* bInterfaceProtocol */
    0x01,                                   /* iInterface */

    /* HID Class Descriptor */
    USB_HID_DESC_SIZE,                      /* bLength */
    HID_HID_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x01,0x01,                            // HID Revision number 1.01
    0x00,                                   /* bCountryCode */
    0x01,                                   /* bNumDescriptors */
    HID_REPORT_DESCRIPTOR_TYPE,             /* bDescriptorType */
    (HID_ReportDescSize2 & 0x0ff),     // Total length of report descriptor
    (HID_ReportDescSize2 > 0xff ? HID_ReportDescSize2 >> 8 : 0x00),

    /* Endpoint2, HID Interrupt In */
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x82,               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */
    0x40, 0x00,                         // wMaxPacketSize, 64 bytes
    0x01,          /* 1ms */                /* bInterval */

/*****************************************************************************
* Description   : Interface 2[]
*****************************************************************************/
    /* Interface 2, Alternate Setting 0, HID Class */
    USB_INTERFACE_DESC_SIZE,                /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,          /* bDescriptorType */
    USB_INTERFACE_2,                        /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x02,                                   /* bNumEndpoints */
    0x03,                                   /* bInterfaceClass */
    HID_SUBCLASS_NONE,                      /* bInterfaceSubClass */
    HID_PROTOCOL_NONE,                    /* bInterfaceProtocol */
    0x02,                                   /* iInterface */

    /* HID Class Descriptor */
    USB_HID_DESC_SIZE,                      /* bLength */
    HID_HID_DESCRIPTOR_TYPE,                /* bDescriptorType */
    0x01,0x01,                            // HID Revision number 1.01
    0x00,                                   /* bCountryCode */
    0x01,                                   /* bNumDescriptors */
    HID_REPORT_DESCRIPTOR_TYPE,             /* bDescriptorType */
    (HID_ReportDescSize3 & 0x0ff),     // Total length of report descriptor
    (HID_ReportDescSize3 > 0xff ? HID_ReportDescSize3 >> 8 : 0x00),

    /* Endpoint3, HID Interrupt In */
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x83,               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */
    0x40, 0x00,                         // wMaxPacketSize, 64 bytes
    0x01,          /* 1ms */                /* bInterval */
        /* Endpoint3, HID Interrupt out */
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x03,               /* bEndpointAddress */
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */
    0x40, 0x00,                             // wMaxPacketSize, 64 bytes
    0x01,          /* 1ms */                /* bInterval */
};

/* USB String Descriptor (optional) */
/*****************************************************************************
* Description   : USB_LanguageStringDescriptor[]
*****************************************************************************/
const uint8_t USB_LanguageStringDescriptor[] =
{
/* Index 0x00: LANGID Codes */
    0x04,                               /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,         /* bDescriptorType */
    USB_WBVAL(0x0409),                  /* US English *//* wLANGID */
};

/*****************************************************************************
* Description   : USB_ManufacturerStringDescriptor[]
*****************************************************************************/
const uint8_t USB_ManufacturerStringDescriptor[] =
{
/* Index 0x01: Manufacturer */
    (6*2 + 2),                          /* bLength (5 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,         /* bDescriptorType */
    'Z',0,
    'O',0,
    'R',0,
    'A',0,
    'K',0,
    'B',0,
};

/*****************************************************************************
* Description   : USB_ProductStringDescriptor[]
*****************************************************************************/
const uint8_t USB_ProductStringDescriptor[] =
{
/* Index 0x02: Product */
    (7*2 + 2),                         /* bLength ( 10 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,         /* bDescriptorType */
    'Z',0,
    'O',0,
    'R',0,
    'O',0,
    'A',0,
    'D',0,
    'A',0,
};

/*****************************************************************************
* Description   : USB_SerialNumberStringDescriptor[]
*****************************************************************************/
const uint8_t USB_SerialNumberStringDescriptor[] =
{
/* Index 0x03: Serial Number */
    (4*2 + 2),                          /* bLength (4 Char + Type + lenght) */
    USB_STRING_DESCRIPTOR_TYPE,         /* bDescriptorType */
    'Z',0,
    'O',0,
    'R',0,
    'O',0,
};




