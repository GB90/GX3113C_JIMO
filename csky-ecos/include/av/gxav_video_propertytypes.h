#ifndef __GXAV_VIDEO_PROPERTYTYPES_H__
#define __GXAV_VIDEO_PROPERTYTYPES_H__

typedef enum {
	CODEC_MPEG2V = 1,
	CODEC_MPEG4V,
	CODEC_H264,
	CODEC_H263,
	CODEC_AVSV,
	CODEC_RV,
	CODEC_RV_BITSTREAM,
	CODEC_HEVC,
	CODEC_YUV,
	CODEC_VC1,
	CODEC_MJPEG,
	CODEC_MAX,
}GxVideoDecoderProperty_Type;

struct mpeg2v_property {
	unsigned int  pts_insert;
};


#define DECODER_WORKBUF_SIZE (380*1024)
typedef struct {
	unsigned int    addr;
	unsigned int    size;
}DecoderWorkbufConfig;

typedef struct {
	GxVideoDecoderProperty_Type type;
	unsigned int                pts_sync;
	unsigned int                pts_reorder;
	int                         fps;
	union {
		struct mpeg2v_property  mpeg2;
	} u;
	struct
	{
		unsigned int max_width;
		unsigned int max_height;
		unsigned int denoise_en;
	}deinterlace;
	struct {
		unsigned int enable;
		unsigned int data_length;
	}cc;

	DecoderWorkbufConfig        workbuf;
	unsigned int                mosaic_freez;
	unsigned int                err_gate;
} GxVideoDecoderProperty_Config;


typedef struct {
	unsigned int    addr;
	unsigned int    size;
} DecoderColbufConfig;

typedef struct {
	#define MAX_FB_NUM      (32)
	#define FREEZE_FB_ID    (0)
	unsigned int    fb_num;
	struct {
		unsigned int addr;
		unsigned int size;
	}fb[MAX_FB_NUM];
} DecoderFramebufConfig;

typedef struct {
	DecoderColbufConfig     colbuf;
	DecoderFramebufConfig   framebuf;
} GxVideoDecoderProperty_Run;

typedef struct {
	unsigned int	freeze;
} GxVideoDecoderProperty_Stop;

typedef struct {
	unsigned int frame_num;
} GxVideoDecoderProperty_Skip;

typedef enum {
	FAST_SPEED,            /* 快进 */
	SLOW_SPEED,            /* 慢放 */
	STEP_SPEED,            /* 快退 */
	NORMAL_SPEED,          /* 正常 */
}GxVideoDecoderProperty_SpeedMode;

typedef struct  {
	GxVideoDecoderProperty_SpeedMode mode;
} GxVideoDecoderProperty_Speed;

typedef enum {
	FRAMERATE_NONE     = 0,
	FRAMERATE_23976    = (1 << 0),
	FRAMERATE_24       = (1 << 1),
	FRAMERATE_25       = (1 << 2),
	FRAMERATE_29397    = (1 << 3),
	FRAMERATE_299      = (1 << 4),
	FRAMERATE_301      = (1 << 5),
	FRAMERATE_30       = (1 << 6),
	FRAMERATE_50       = (1 << 7),
	FRAMERATE_5994     = (1 << 8),
	FRAMERATE_60       = (1 << 9),
	FRAMERATE_RESERVE  = 0xFF
}GxVideoDecoderProperty_FrameRate;

typedef enum {
	ASPECTRATIO_NONE,
	ASPECTRATIO_1BY1,
	ASPECTRATIO_4BY3,
	ASPECTRATIO_14BY9,
	ASPECTRATIO_16BY9,
	ASPECTRATIO_221BY1,
	ASPECTRATIO_RESERVE
}GxVideoDecoderProperty_AspectRatio;

typedef enum {
	PROGRESSIVE,
	INTERLACE
}GxVideoDecoderProperty_ScanType;

typedef struct {
	unsigned int    width;
	unsigned int    height;

	unsigned int    fb_num;//means just play
	#define ZOOM_EXTRAL_FB_NUM          (3)//for zoom into smaller than 1/4 in I resolution or 1/8 in P resolution
	#define DEINTERLACE_EXTRAL_FB_NUM   (5)//for deinterlace, certainly can support zoom too
	unsigned int    fb_size;
	unsigned int    colbuf_size;

	GxAvFrameStat   stat;
	GxVideoDecoderProperty_ScanType    type;
	GxVideoDecoderProperty_FrameRate   rate;
	GxVideoDecoderProperty_AspectRatio ratio;
} GxVideoDecoderProperty_FrameInfo;

typedef struct {
	unsigned int pts;
} GxVideoDecoderProperty_PTS;

typedef struct {
	unsigned int state;

#define VIDEODEC_RUNNING	0
#define VIDEODEC_STOPED		1
#define VIDEODEC_PAUSED		2
#define VIDEODEC_SKIPED		3
#define VIDEODEC_SPEEDED	4
#define VIDEODEC_OVER		5
#define VIDEODEC_ERROR		6
#define VIDEODEC_READY		7
#define VIDEODEC_UNSUPPORT	8
#define VIDEODEC_BUSY		9
} GxVideoDecoderProperty_State;

typedef struct {
	unsigned int err_no;

#define STREAM_UNSUPPORT   (1 << 0)
#define FIRST_FRAME_ERROR  (1 << 1)

} GxVideoDecoderProperty_Errno;

typedef struct {
	GxVideoDecoderProperty_Type type;
	unsigned int min_width;
	unsigned int max_width;
	unsigned int min_height;
	unsigned int max_height;
	GxVideoDecoderProperty_FrameRate frame_rate;
} GxVideoDecoderProperty_Capability;

typedef enum {
	VIDEODEC_MODE_NORMAL = 1,
	VIDEODEC_MODE_IONLY  = 2,
	VIDEODEC_MODE_IPONLY = 3,
}DecodeMode;

typedef struct {
	DecodeMode dec_mode;
} GxVideoDecoderProperty_DecMode;

typedef struct {
	GxVideoDecoderProperty_Type type;
	unsigned int                frame;
} GxVideoDecoderProperty_CoverFrame;

typedef struct {
    int                value;
} GxVideoDecoderProperty_Tolerance;

typedef struct {
    int                over;
} GxVideoDecoderProperty_OneFrameOver;

#endif

