#ifndef __GXAV_VOUT_PROPERTYTYPES_H__
#define __GXAV_VOUT_PROPERTYTYPES_H__

typedef enum {
	GXAV_VOUT_RCA         = 0x01,
	GXAV_VOUT_VGA         = 0x02,
	GXAV_VOUT_YUV         = 0x04,
	GXAV_VOUT_DVI         = 0x08,
	GXAV_VOUT_HDMI        = 0x10,
	GXAV_VOUT_SCART       = 0x20,
	GXAV_VOUT_SVIDEO      = 0x40,
	GXAV_VOUT_LCD         = 0x80
}GxVideoOutProperty_Interface;

typedef enum {
	GXAV_VOUT_PAL = 1                   ,
	GXAV_VOUT_PAL_M                     ,
	GXAV_VOUT_PAL_N                     ,
	GXAV_VOUT_PAL_NC                    ,
	GXAV_VOUT_NTSC_M                    ,
	GXAV_VOUT_NTSC_443                  ,

	GXAV_VOUT_YCBCR_480I                ,
	GXAV_VOUT_YCBCR_576I                ,

	GXAV_VOUT_YPBPR_1080I_50HZ          ,
	GXAV_VOUT_YPBPR_1080I_60HZ          ,

	GXAV_VOUT_YPBPR_480P                ,
	GXAV_VOUT_YPBPR_576P                ,

	GXAV_VOUT_YPBPR_720P_50HZ           ,
	GXAV_VOUT_YPBPR_720P_60HZ           ,
	GXAV_VOUT_YPBPR_1080P_50HZ          ,
	GXAV_VOUT_YPBPR_1080P_60HZ          ,

	GXAV_VOUT_VGA_480P                  ,
	GXAV_VOUT_VGA_576P                  ,

	GXAV_VOUT_DIGITAL_RGB_720x480_0_255 ,
	GXAV_VOUT_DIGITAL_RGB_320x240_0_255 ,
	GXAV_VOUT_DIGITAL_RGB_16_235        ,

	GXAV_VOUT_BT656_YC_8BITS            ,
	GXAV_VOUT_BT656_YC_10BITS           ,

	GXAV_VOUT_HDMI_480I                 ,
	GXAV_VOUT_HDMI_576I                 ,
	GXAV_VOUT_HDMI_480P                 ,
	GXAV_VOUT_HDMI_576P                 ,
	GXAV_VOUT_HDMI_720P_50HZ            ,
	GXAV_VOUT_HDMI_720P_60HZ            ,
	GXAV_VOUT_HDMI_1080I_50HZ           ,
	GXAV_VOUT_HDMI_1080I_60HZ           ,
	GXAV_VOUT_HDMI_1080P_50HZ           ,
	GXAV_VOUT_HDMI_1080P_60HZ           ,

	GXAV_VOUT_NULL_MAX                  ,
}GxVideoOutProperty_Mode;

typedef enum {
	ASPECT_RATIO_NOMAL = 0,
	ASPECT_RATIO_PAN_SCAN,
	ASPECT_RATIO_LETTER_BOX,
	ASPECT_RATIO_RAW_SIZE,
	ASPECT_RATIO_RAW_RATIO,
	ASPECT_RATIO_4X3_PULL,
	ASPECT_RATIO_4X3_CUT,
	ASPECT_RATIO_16X9_PULL,
	ASPECT_RATIO_16X9_CUT,
	ASPECT_RATIO_4X3,
	ASPECT_RATIO_16X9,
	ASPECT_RATIO_AUTO,
}GxVideoOutProperty_Spec;

typedef enum {
	TV_SCREEN_4X3 = 0,
	TV_SCREEN_16X9
}GxVideoOutProperty_Screen;

typedef enum {
	SCART_CVBS = 0,
	SCART_RGB
}GxVideoOutProperty_Scart;

typedef struct {
#define VOUT_BUF_SIZE (720*576*3)
	unsigned int  enable;
	unsigned char *buf[3];//每个buf：720*576*3,8byte对齐
} GxVideoOutProperty_OutputConfig;

typedef struct {
	GxVideoOutProperty_Interface selection;
} GxVideoOutProperty_OutputSelect;

typedef struct {
	GxVideoOutProperty_Interface interface;
	GxVideoOutProperty_Mode      mode;
	int                          hdmi_hdcp_on;
} GxVideoOutProperty_Resolution;

typedef struct {
	unsigned int auto_en;
	GxVideoOutProperty_Mode auto_pal;
	GxVideoOutProperty_Mode auto_ntsc;
}GxVideoOutProperty_Auto;

typedef struct {
	unsigned int value;
} GxVideoOutProperty_Brightness;

typedef struct {
	unsigned int value;
} GxVideoOutProperty_Saturation;

typedef struct {
	unsigned int value;
} GxVideoOutProperty_Contrast;

typedef struct {
	unsigned int enable;
} GxVideoOutProperty_Visible;

typedef struct {
	GxVideoOutProperty_Spec spec;
}GxVideoOutProperty_AspectRatio;

typedef struct {
	GxVideoOutProperty_Screen screen;
}GxVideoOutProperty_TvScreen;

typedef struct {
	GxVideoOutProperty_Scart	source;
	GxVideoOutProperty_Screen	screen;
}GxVideoOutProperty_ScartConfig;

typedef struct {
	unsigned int enable;
} GxVideoOutProperty_OutDefault;

typedef struct {
	GxVideoOutProperty_Interface selection;
} GxVideoOutProperty_PowerOff;

typedef struct {
	GxVideoOutProperty_Interface selection;
} GxVideoOutProperty_PowerOn;

typedef struct {
	int status;
#define HDMI_PLUG_OUT	(0)
#define HDMI_PLUG_IN	(1)
} GxVideoOutProperty_OutHdmiStatus;


typedef struct {
	unsigned audio_codes;
#define AUDIO_LINE_PCM		(1<<0)
#define AUDIO_AC3			(1<<1)
#define AUDIO_DOLBY_PLUS	(1<<2)
} GxVideoOutProperty_EdidInfo;

typedef struct {
	GxVideoOutProperty_Mode vout_mode;
} GxVideoOutProperty_HdmiHdcpAuth;

#endif

