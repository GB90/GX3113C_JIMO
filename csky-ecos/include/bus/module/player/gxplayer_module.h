#ifndef __GX_PLAYER__MODULE_H__
#define __GX_PLAYER__MODULE_H__

#include "gxcore.h"
#include "av/avapi.h"
#include "gxplayer_filter.h"
#include "gxplayer_common.h"
#include "gxplayer_subtitle.h"

__BEGIN_DECLS

#define PLAYER_NAME_LONG                        (31)
#define PLAYER_URL_LONG                         (2047)

#define PLAYER_MAX_TRACK_AUDIO                  (16)
#define PLAYER_MAX_TRACK_SUB                    (16)
#define PLAYER_MAX_TRACK_ADD                    (32)

#define PLAYER_TARCK_LANG_LONG                  (8)
#define PLAYER_TARCK_NAME_LONG                  (32)
#define PLAYER_TARCK_CODEC_LONG                 (32)

#define PLAYER_MAX_SUB_LOAD                     (1)

#define PLAYER_CONFIG_PLAY_RESERVED_MEMSIZE     "Play>packet_cache_size"
#define PLAYER_CONFIG_RECORD_CACHED_MEMSIZE     "Play>record_cache_size"
#define PLAYER_CONFIG_NETWORK_CACHED_MEMSIZE    "Play>network_cache_size"
#define PLAYER_CONFIG_FLAG_ERROR_STOP           "Play>play_error_stop"
#define PLAYER_CONFIG_SUPPORT_NDS               "Play>play_support_nds"

#define PLAYER_CONFIG_AUTOPLAY_VIDEO_NAME       "AutoPlay>video_name"
#define PLAYER_CONFIG_AUTOPLAY_VIDEO_URL        "AutoPlay>video_url"
#define PLAYER_CONFIG_AUTOPLAY_RADIO_NAME       "AutoPlay>radio_name"
#define PLAYER_CONFIG_AUTOPLAY_RADIO_URL        "AutoPlay>radio_url"
#define PLAYER_CONFIG_AUTOPLAY_RADIO_FLAG       "AutoPlay>radio_flag"
#define PLAYER_CONFIG_AUTOPLAY_PLAY_FLAG        "AutoPlay>play_flag"

#define PLAYER_CONFIG_VIDEO_ESV_SIZE            "Video>esv_size"
#define PLAYER_CONFIG_VIDEO_INTERFACE           "Video>interface"
#define PLAYER_CONFIG_VIDEO_RESOLUTION          "Video>resolution"
#define PLAYER_CONFIG_VIDEO_RESOLUTION1         "Video>resolution1"
#define PLAYER_CONFIG_VIDEO_DISPLAY_SCREEN      "Video>screen"
#define PLAYER_CONFIG_VIDEO_SCREEN_XRES         "Video>xres"
#define PLAYER_CONFIG_VIDEO_SCREEN_YRES         "Video>yres"
#define PLAYER_CONFIG_VIDEO_ASPECT              "Video>aspect"
#define PLAYER_CONFIG_VIDEO_SCART_SOURCE        "Video>scart_source"
#define PLAYER_CONFIG_VIDEO_SCART_SOURCE1       "Video>scart_source1"
#define PLAYER_CONFIG_VIDEO_SCART_SCREEN        "Video>scart_screen"
#define PLAYER_CONFIG_VIDEO_SCART_SCREEN1       "Video>scart_screen1"
#define PLAYER_CONFIG_VIDEO_BRIGHTNESS          "Video>brightness"
#define PLAYER_CONFIG_VIDEO_SATURATION          "Video>Stauration"
#define PLAYER_CONFIG_VIDEO_CONTRAST            "Video>contrast"
#define PLAYER_CONFIG_VIDEO_FREEZE_SWITCH       "Video>freeze_switch"
#define PLAYER_CONFIG_VIDEO_WAIT_SWITCH         "Video>wait_switch"
#define PLAYER_CONFIG_VIDEO_DEC_TIMEOUT         "Video>dec_timeout"
#define PLAYER_CONFIG_VIDEO_DEC_CC_ENABLE       "Video>cc_enable"
#define PLAYER_CONFIG_VIDEO_DEC_CC_DATA_LENGTH  "Video>cc_data_length"
#define PLAYER_CONFIG_VIDEO_DEC_MOSAIC_DROP     "Video>mosaic_drop"
#define PLAYER_CONFIG_VIDEO_DEC_MOSAIC_GATE     "Video>mosaic_gate"
#define PLAYER_CONFIG_VIDEO_DEC_SYNC_FLAG       "Video>sync_flag"
#define PLAYER_CONFIG_VIDEO_SUPPORT_PPZOOM      "Video>support_ppzoom"
#define PLAYER_CONFIG_VIDEO_AUTO_ADAPT          "Video>auto_adapt"
#define PLAYER_CONFIG_VIDEO_AUTO_PAL            "Video>auto_pal"
#define PLAYER_CONFIG_VIDEO_AUTO_NTSC           "Video>auto_ntsc"
#define PLAYER_CONFIG_VIDEO_PP_MAX_WIDTH        "Video>pp_max_width"
#define PLAYER_CONFIG_VIDEO_PP_MAX_HEIGHT       "Video>pp_max_height"
#define PLAYER_CONFIG_VIDEO_FB_MAX_SIZE         "Video>fb_max_size"

#define PLAYER_CONFIG_VOUT_HDMI_HDCP_ON         "Vout>hdmi_hdcp_on"
#define PLAYER_CONFIG_VOUT_HDMI_HDCP_MODE       "Vout>hdmi_hdcp_mode"

#define PLAYER_CONFIG_AUDIO_INTERFACE           "Audio>interface"
#define PLAYER_CONFIG_AUDIO_SPDIF_SOURCE        "Audio>spd_source"
#define PLAYER_CONFIG_AUDIO_HDMI_SOURCE         "Audio>hdmi_source"
#define PLAYER_CONFIG_AUDIO_VOLUME_PRIV         "Audio>volume_priv"
#define PLAYER_CONFIG_AUDIO_VOLUME              "Audio>volume"
#define PLAYER_CONFIG_AUDIO_TRACK               "Audio>track"
#define PLAYER_CONFIG_AUDIO_AC3_MODE            "Audio>ac3mode"
#define PLAYER_CONFIG_AUDIO_AC3_ENABLE          "Audio>ac3enable"
#define PLAYER_CONFIG_AUDIO_AAC_ENABLE          "Audio>aacenable"
#define PLAYER_CONFIG_AUDIO_DOWNMIX             "Audio>downmix"

#define PLAYER_CONFIG_PVR_TIME_SHIFT_FLAG       "PVR>shift_flag"
#define PLAYER_CONFIG_PVR_TIME_SHIFT_FILE       "PVR>shift_file"
#define PLAYER_CONFIG_PVR_TIME_SHIFT_DMXID      "PVR>shift_dmxid"

#define PLAYER_CONFIG_DEALY_MAX_USED_MEMORY     "PVR>delay_memory"
#define PLAYER_CONFIG_PVR_RECORD_VOLUME_SIZE    "PVR>record_volume_sizemb"
#define PLAYER_CONFIG_PVR_RECORD_VOLUME_MAX     "PVR>record_volume_maxnum"

extern void GxPlayer_ModuleRegisterALL(void);
extern void GxPlayer_ModuleRegisterDVB(void);
extern void GxPlayer_ModuleRegisterFILE(void);
extern void GxPlayer_ModuleRegisterHTTP(void);
extern void GxPlayer_ModuleRegisterMP3(void);
extern void GxPlayer_ModuleRegisterMP4(void);
extern void GxPlayer_ModuleRegisterAVI(void);
extern void GxPlayer_ModuleRegisterMKV(void);
extern void GxPlayer_ModuleRegisterFLV(void);
extern void GxPlayer_ModuleRegisterAAC(void);
extern void GxPlayer_ModuleRegisterTS (void);
extern void GxPlayer_ModuleRegisterPS (void);
extern void GxPlayer_ModuleRegisterLOGO(void);
extern void GxPlayer_ModuleRegisterRMVB(void);
extern void GxPlayer_ModuleRegisterJPEG(void);
extern void GxPlayer_ModuleRegisterRTSP(void);

typedef enum
{
	VIDEO_OUTPUT_RCA       = (1 << 0) ,
	VIDEO_OUTPUT_VGA       = (1 << 1) ,
	VIDEO_OUTPUT_YUV       = (1 << 2) ,
	VIDEO_OUTPUT_DVI       = (1 << 3) ,
	VIDEO_OUTPUT_HDMI      = (1 << 4) ,
	VIDEO_OUTPUT_SCART     = (1 << 5) ,
	VIDEO_OUTPUT_SVIDEO    = (1 << 6) ,
	VIDEO_OUTPUT_LCD       = (1 << 7) ,

	VIDEO_OUTPUT_RCA1      = (1 << 8) ,
	VIDEO_OUTPUT_VGA1      = (1 << 9) ,
	VIDEO_OUTPUT_YUV1      = (1 << 10) ,
	VIDEO_OUTPUT_DVI1      = (1 << 11) ,
	VIDEO_OUTPUT_HDMI1     = (1 << 12) ,
	VIDEO_OUTPUT_SCART1    = (1 << 13) ,
	VIDEO_OUTPUT_SVIDEO1   = (1 << 14) ,
	VIDEO_OUTPUT_LCD1      = (1 << 15) ,

}VideoOutputInterface;

typedef enum
{
	// RCA/SCART/SVIDEO
	VIDEO_OUTPUT_PAL                 = 1 ,  //-PAL
	VIDEO_OUTPUT_PAL_M               = 2 ,
	VIDEO_OUTPUT_PAL_N               = 3 ,
	VIDEO_OUTPUT_PAL_NC              = 4 ,
	VIDEO_OUTPUT_NTSC_M              = 5 ,  //-NTSC
	VIDEO_OUTPUT_NTSC_443            = 6 ,

	//YUV
	VIDEO_OUTPUT_YCBCR_480I          = 7 ,
	VIDEO_OUTPUT_YCBCR_576I          = 8 ,

	VIDEO_OUTPUT_YCBCR_1080I_50HZ    = 9 ,
	VIDEO_OUTPUT_YCBCR_1080I_60HZ    = 10 ,

	VIDEO_OUTPUT_YPBPR_480P          = 11 ,
	VIDEO_OUTPUT_YPBPR_576P          = 12 ,

	VIDEO_OUTPUT_YPBPR_720P_50HZ     = 13 ,
	VIDEO_OUTPUT_YPBPR_720P_60HZ     = 14 ,
	VIDEO_OUTPUT_YPBPR_1080P_50HZ    = 15 ,
	VIDEO_OUTPUT_YPBPR_1080P_60HZ    = 16 ,

	//VGA
	VIDEO_OUTPUT_VGA_480P            = 17 ,
	VIDEO_OUTPUT_VGA_576P            = 18 ,

	//LCD
	VIDEO_OUTPUT_DIGITAL_RGB_720x480_0_255 = 19 ,
	VIDEO_OUTPUT_DIGITAL_RGB_320x240_0_255 = 20 ,
	VIDEO_OUTPUT_DIGITAL_RGB_16_235  = 21 ,

	//HDMI->656
	VIDEO_OUTPUT_BT656_YC_8BITS      = 22 ,
	VIDEO_OUTPUT_BT656_YC_10BITS     = 23 ,

	//HDMI
	VIDEO_OUTPUT_HDMI_480I           = 24,
	VIDEO_OUTPUT_HDMI_576I           = 25,
	VIDEO_OUTPUT_HDMI_480P           = 26,
	VIDEO_OUTPUT_HDMI_576P           = 27,
	VIDEO_OUTPUT_HDMI_720P_50HZ      = 28,
	VIDEO_OUTPUT_HDMI_720P_60HZ      = 29,
	VIDEO_OUTPUT_HDMI_1080I_50HZ     = 30,
	VIDEO_OUTPUT_HDMI_1080I_60HZ     = 31,
	VIDEO_OUTPUT_HDMI_1080P_50HZ     = 32,
	VIDEO_OUTPUT_HDMI_1080P_60HZ     = 33,

	VIDEO_OUTPUT_MODE_MAX
}VideoOutputMode;

typedef enum {
	VIDEO_OUTPUT_SOURCE_CVBS,
	VIDEO_OUTPUT_SOURCE_RGB,
}VideoOutputSource;

typedef enum {
	VIDEO_OUTPUT_SCREEN_4X3,
	VIDEO_OUTPUT_SCREEN_16X9,
}VideoOutputScreen;

typedef struct {
	VideoOutputSource source;
	VideoOutputScreen screen;
}VideoOutputScartConfig;

typedef struct {
	VideoOutputInterface    interface;
	VideoOutputMode         mode;
	union {
		VideoOutputScartConfig scart;
	}config;
}VideoOutputConfig;

typedef struct {
	int                           enable;
	int                           data_length;
}VideoCcConfig;

typedef enum {
	AUDIO_OUTPUT_DAC_IN     = (1 << 0) ,
	AUDIO_OUTPUT_DAC_OUT    = (1 << 1) ,
	AUDIO_OUTPUT_SPDIF      = (1 << 2) ,
	AUDIO_OUTPUT_HDMI       = (1 << 3) ,
}AudioOutputInterface;

typedef enum {
	AUDIO_OUTPUT_PCM,
	AUDIO_OUTPUT_AC3,
	AUDIO_OUTPUT_EAC3,
	AUDIO_OUTPUT_AUTO,
	AUDIO_OUTPUT_NOTANY,
}AudioOutputDataType;

typedef enum {
	SEEK_ORIGIN_SET         ,
	SEEK_ORIGIN_CUR         ,
	SEEK_ORIGIN_END         ,
	SEEK_ORIGIN_PERCENT         ,
}SeekFlag;

typedef struct {
	unsigned int            x;
	unsigned int            y;
	unsigned int            width;
	unsigned int            height;
} PlayerWindow;

typedef struct {
	int                     vout_id;//0:vout0 is the default,1:vout1
	int                     brightness;
	int                     saturation;
	int                     contrast;
}VideoColor;

typedef struct {
	AudioOutputDataType     output_data;
	AudioOutputInterface    output_port;
}AudioOutputConfig;

typedef enum
{
	AUDIO_TRACK_MONO        , // all mono
	AUDIO_TRACK_LEFT        , // all left
	AUDIO_TRACK_RIGHT       , // all right
	AUDIO_TRACK_STEREO      , // left->left, right->right
	AUDIO_TRACK_RIGHT_MUTE_LEFT, // left->mute, right->right
	AUDIO_TRACK_LEFT_MUTE_RIGHT, // left->left, right->mute
	AUDIO_TRACK_MONO_MUTE_LEFT,//right->mono, other->mute 
	AUDIO_TRACK_MONO_MUTE_RIGHT, //left ->mono, other->mute
	//DOLBY TEST
	AUDIO_TRACK_SPDIF_LR    ,
	AUDIO_TRACK_SPDIF_LsRs  ,
	AUDIO_TRACK_SPDIF_CS    ,
	AUDIO_TRACK_SPDIF_X1X2  ,
}AudioTrack;

typedef enum
{
	AUDIO_AC3_DECODE_MODE = (1<<0),
	AUDIO_AC3_BYPASS_MODE = (1<<1),
	AUDIO_AC3_FULL_MODE   = AUDIO_AC3_DECODE_MODE|AUDIO_AC3_BYPASS_MODE,
}AudioAc3Mode;

typedef enum {
	ASPECT_NORMAL       = 0 ,
	ASPECT_PAN_SCAN         ,
	ASPECT_LETTER_BOX       ,
	ASPECT_RAW_SIZE         ,
	ASPECT_RAW_RATIO        ,
	ASPECT_4X3_PULL         ,
	ASPECT_4X3_CUT          ,
	ASPECT_16X9_PULL        ,
	ASPECT_16X9_CUT         ,
	ASPECT_4X3              ,
	ASPECT_16X9             ,
}VideoAspect;

typedef struct {
	int aspect;
#define DISPLAY_SCREEN_4X3   0
#define DISPLAY_SCREEN_16X9  1
	int xres;
	int yres;
}DisplayScreen;

typedef struct {
	int vout_id;
	int enable;
	VideoOutputMode pal;
	VideoOutputMode ntsc;
}PlayerVideoAutoAdapt;

typedef struct {
	PlayerStatus            status;
	PlayerError             error;
	AVCodecStatus           acodec;
	AVCodecStatus           vcodec;
}PlayerStatusInfo;

typedef struct {
	const char* name;
	uint64_t current;
	uint64_t totle;
	uint64_t seek_min;
}PlayTimeInfo;

typedef enum {
	PLAYER_SUB_TYPE_CC      ,
	PLAYER_SUB_TYPE_DVB     ,
	PLAYER_SUB_TYPE_FILE    ,
	PLAYER_SUB_TYPE_INSIDE  ,
}PlayerSubType;

typedef enum {
	PLAYER_SUB_RENDER_SPP   ,
	PLAYER_SUB_RENDER_OSD   ,
}PlayerSubRender;

typedef struct {
	int16_t                 pid;
	int16_t                 major;
	int16_t                 minor;
}PlayerSubPID;

typedef struct {
	int                     id;
	char                    lang[PLAYER_TARCK_LANG_LONG];
	char                    track_name[PLAYER_TARCK_NAME_LONG];
	char                    codec_id[PLAYER_TARCK_CODEC_LONG];
	PlayerSubEncode         encode;
}PlayerSubTrack;

typedef struct{
	PlayerSubType           type;
	PlayerSubRender         render;
	const char*             file_name;
	PlayerSubPID            pid;
	int                     (*init)(PlayerWindow* rect,int type);
	void                    (*destory)(int handle);
	int                     (*show)(int handle);
	int                     (*hide)(int handle);
	int                     (*draw)(void* data, int num,int type);
	void                    (*clear)(int handle);
}PlayerSubPara;

typedef struct  {
	void*                   sub_hdr;
	int                     sub_num;
	PlayerSubTrack          track[PLAYER_MAX_TRACK_SUB];
}PlayerSubtitle;

#pragma pack(1)
typedef struct {
	char                    Header[4+1];   // 标签头必须是"TAG"否则认为没有标签,3字节
	char                    Title[30+1];   // 标题,30字节
	char                    Artist[30+1];  // 作者,30字节
	char                    Album[30+1];   // 专集,30字节
	char                    Year[4+1];     // 出品年代,4字节
	char                    Comment[28+1]; // 备注,28字节
	char                    Reserve[1];    // 保留,1字节
	char                    Track[1+1];    // 音轨,1字节
	char                    Genre[1+1];    // 类型,1字节
}PlayerID3V1;
#pragma pack()

#pragma pack(1)
typedef struct id3_v2
{
	char                    fName[4+1];
	char*                   pFBody;
	int                     size;
	struct                  id3_v2 *pNext;
}PlayerID3V2;
#pragma pack()

typedef struct {
	PlayerID3V1*            v1;
	PlayerID3V2*            v2;
}PlayerID3Info;

typedef enum {
	SNAP_LAYER_OSD          ,
	SNAP_LAYER_VPP          ,
	SNAP_LAYER_SPP          ,
	SNAP_LAYER_BKG          ,
	SNAP_LAYER_MIX          ,
}PlayerLayer;

typedef struct {
	PlayerLayer             layer;
	PlayerWindow            rect;
	void*                   surface;
}PlayerSnapshot;

typedef struct {
	char                    codec_id[PLAYER_TARCK_CODEC_LONG]  ; // 视频编码格式
	char                    track_name[PLAYER_TARCK_NAME_LONG] ;//轨道 名称
	uint32_t                width   ; // 视频帧宽
	uint32_t                height  ; // 视频帧高
	uint32_t                bitrate ; // 视频的比特率
	uint32_t                interlace;// 隔行编码
	float                   fps     ; // 视频帧率
	float                   aspect  ; // 视频的宽高比
	VideoCodecType          codec_type;
}PlayerVideoTrack;

typedef struct {
	uint32_t                id;
	char                    lang[PLAYER_TARCK_LANG_LONG];
	char                    codec_id[PLAYER_TARCK_CODEC_LONG]  ; // 音频的编码格式
	char                    track_name[PLAYER_TARCK_NAME_LONG] ; //轨道 名称
	uint32_t                bitrate    ;  // 音频的比特率
	uint32_t                samplerate ;  // 音频的采样率
	uint32_t                channels   ;  // 音频的声道数
	AudioCodecType          codec_type;
}PlayerAudioTrack;

typedef struct {
	char                    url[PLAYER_URL_LONG+1];
	uint64_t                file_size;
	uint64_t                duration ;
	PlayerVideoTrack        video;
	PlayerAudioTrack        audio[PLAYER_MAX_TRACK_AUDIO];
	PlayerSubTrack          sub[PLAYER_MAX_TRACK_SUB];
	uint32_t                bitrate;  // 流的比特率
	unsigned int            bandwidth[GXPLAYER_MAX_BAND_WIDTH];
	int                     audio_num;
	int                     sub_num;
	int                     bandwidth_num;
	int                     is_radio;
	int                     cur_sub_id;
	int                     cur_audio_id;
	int                     cur_bandwidth_id;
	uint64_t				rec_filesize;
	uint64_t				rec_duration;
	int                     net_speed;
	int                     buf_percent;
	int                     buffering;
	GxDemuxerRecordExtInfo  tsinfo;
	GxAvFrameStat           video_stat;
	GxAvFrameStat           audio_stat;
	unsigned int            switch_cost_timems;
}PlayerProgInfo;

typedef struct {
	int                     cur_id;
	int                     num;
	unsigned int            value[GXPLAYER_MAX_BAND_WIDTH];
}PlayerBandwidth;

typedef struct {
	int                     buffering;
	int                     net_speed;
	int                     buf_percent;
	int64_t                 restart_time;
	PlayerBandwidth         bandwidth;
}PlayerNetInfo;

typedef struct {
	GxDemuxerRecordExtInfo  ext_info;
	GxDemuxerRecordUserInfo ext_data;
}PlayerRecordConfig;

typedef struct {
	GxDemuxerPlayTsExtInfo ext_ts_info;
}PlayerPlayConfig;

typedef struct {
	int                     volume_sizemb;
	int                     volume_maxnum;
}PlayerPVRConfig;

typedef struct {
	unsigned char           url[PLAYER_URL_LONG+1];
	int                     enable;
	int                     shift_dmxid;
}PlayerShiftConfig;

typedef enum
{
	PLAYER_EVENT_SPEED_REPORT      ,
	PLAYER_EVENT_STATUS_REPORT     ,
	PLAYER_EVENT_AVCODEC_REPORT    ,
	PLAYER_EVENT_RESOLUTION_REPORT ,
	PLAYER_EVENT_RECORD_OVERFLOW   ,
}PlayerEventID;

typedef struct {
	char                          player[PLAYER_NAME_LONG+1];
	char                          url[PLAYER_URL_LONG+1];
	PlayerStatus                  status;
	int                           error;
}PlayerEventStatus;

typedef struct {
	char                          player[PLAYER_NAME_LONG+1];
	char                          url[PLAYER_URL_LONG+1];
	AVCodecStatus                 vcodec;
	AVCodecStatus                 acodec;
}PlayerEventAVCodec;

typedef struct {
	VideoOutputMode               resolution;
}PlayerEventResolution;

typedef struct {
	char                          player[PLAYER_NAME_LONG+1];
	float                         speed;
}PlayerEventSpeed;

typedef struct {
	char                          player[PLAYER_NAME_LONG+1];
}PlayerEventRecordOverflow;

typedef struct {
	int64_t start;
	int volume;
	int audio_pid;
	int video_pid;
	int sub_pid;
	int abort;
	PlayerWindow rect;
}PlayerPlayInfo;

typedef void (*PLAYER_EVENT_REPORT)(PlayerEventID event,void* args);

typedef int  (*PLAYER_FREEZE_FRAME)(void);

typedef int  (*PLAYER_UNFREEZE_FRAME)(void);

typedef int  (*PLAYER_SEEK_CBK)(int timems);

typedef int  (*PLAYER_INTERRUPT_CBK)(void);

extern void GxPlayer_SetSeekCallback(PLAYER_SEEK_CBK cbk);

extern void GxPlayer_SetEventCallback(PLAYER_EVENT_REPORT report);

extern void GxPlayer_ModuleInit(int modFlag);

extern void GxPlayer_ModuleDestroy(void);


extern status_t GxPlayer_MediaPlay(const char* name, const char* url, int64_t start, int volume, PlayerWindow* window);

extern status_t GxPlayer_MediaExitPlay(const char* name);

extern status_t GxPlayer_MediaWaitPlay(const char* name);

extern status_t GxPlayer_MediaPlay2(const char* name, const char* srcurl, PlayerPlayInfo *info);

extern status_t GxPlayer_MediaRecord(const char* name, const char* url, const char* file);

extern status_t GxPlayer_MediaStop(const char* name);

extern status_t GxPlayer_MediaPause(const char* name);

extern status_t GxPlayer_MediaResume(const char* name);

extern status_t GxPlayer_MediaSeek(const char* name, int64_t time, SeekFlag flag);

extern status_t GxPlayer_MediaSpeed(const char* name, float speed);

extern status_t GxPlayer_MediaWindow(const char* name, PlayerWindow* window);

extern status_t GxPlayer_MediaClip(const char* name, PlayerWindow* Clip);

extern status_t GxPlayer_MediaAudioSync(const char* name, int timems);

extern status_t GxPlayer_MediaAudioSwitch(const char* name,int pid,AudioCodecType type,char* url);

extern status_t GxPlayer_MediaVideoHide(const char* name);

extern status_t GxPlayer_MediaVideoShow(const char* name);

extern status_t GxPlayer_MediaRecordConfig(const char* name, PlayerRecordConfig* config);

extern status_t GxPlayer_MediaPlayConfig(const char* name, PlayerPlayConfig* config);

extern PlayerSubtitle*  GxPlayer_MediaSubLoad(const char* name, PlayerSubPara* subpara);

extern status_t GxPlayer_MediaSubUnLoad(const char* name, PlayerSubtitle* subdata);

extern status_t GxPlayer_MediaSubHide(const char* name, PlayerSubtitle* subdata);

extern status_t GxPlayer_MediaSubShow(const char* name, PlayerSubtitle* subdata);

extern status_t GxPlayer_MediaSubSync(const char* name, PlayerSubtitle* subdata,int timems);

extern status_t GxPlayer_MediaSubSwitch(const char* name, PlayerSubtitle* subdata,PlayerSubPID pid);

extern status_t GxPlayer_MediaSubResolution(const char* name, PlayerSubtitle* subdata,VideoOutputMode res);


extern status_t GxPlayer_MediaGetDataLength(const char* name, uint32_t* aDataLen, uint32_t* vDataLen);

extern status_t GxPlayer_MediaGetTime(const char* name, uint64_t* current, uint64_t* totle);
extern status_t GxPlayer_MediaGetTimeExt(PlayTimeInfo *info);
extern status_t GxPlayer_MediaGetPercent(const char* name, uint64_t* current);

extern status_t GxPlayer_MediaGetProgInfoByUrl(const char* url, PlayerProgInfo* info);

extern status_t GxPlayer_MediaGetProgInfoByName(const char* player, PlayerProgInfo* info);

extern status_t GxPlayer_MediaGetNetInfo(const char* name, PlayerNetInfo* info);

extern status_t GxPlayer_MediaGetStatus(const char* name, PlayerStatusInfo* info);

extern status_t GxPlayer_MediaFreeID3Info(PlayerID3Info* pID3);

extern PlayerID3Info* GxPlayer_MediaGetID3Info(const char* file);

extern status_t GxPlayer_SetVideoSyncMode(int flag);
// 0: free run;
// 1: play when synced;
// 2: play when decode over; before synced;

extern status_t GxPlayer_SetVideoColors(VideoColor color);

extern status_t GxPlayer_SetVideoAspect(VideoAspect aspect );

extern status_t GxPlayer_SetVideoOutputSelect(int interface);

extern status_t GxPlayer_SetVideoOutputConfig(VideoOutputConfig config);

extern status_t GxPlayer_SetVideoOutputDef(unsigned int enable);

extern status_t GxPlayer_SetDisplayScreen(DisplayScreen scr);

extern status_t GxPlayer_SetVideoMosaicDrop(int enable);

extern status_t GxPlayer_SetVideoCcConfig(VideoCcConfig *config);

extern status_t GxPlayer_SetAudioOutputSelect(int interface);

extern status_t GxPlayer_SetAudioOutputConfig(AudioOutputConfig config);

extern status_t GxPlayer_SetAudioVolume(unsigned int vol);

extern status_t GxPlayer_SetAudioTrack(AudioTrack track);

extern status_t GxPlayer_SetAudioMute(int enable);

extern status_t GxPlayer_SetAudioAC3Mode(AudioAc3Mode mode);

extern status_t GxPlayer_SetAudioDownMix(int enable);

extern status_t GxPlayer_SetFreezeFrameSwitch(int enable);

extern status_t GxPlayer_SetTimeShiftConfig(PlayerShiftConfig* config);

extern status_t GxPlayer_SetVideoAutoAdapt(PlayerVideoAutoAdapt* config);

extern status_t GxPlayer_MediaTrackAdd(const char* name, GxStreamTrackAdd* track);

extern status_t GxPlayer_MediaTrackDel(const char* name, GxStreamTrackDel* track);

extern status_t GxPlayer_MediaBandwidthSwitch(const char* name, unsigned int index);

extern status_t GxPlayer_SetPVRConfig(PlayerPVRConfig* config);

extern void GxPlayer_SetVideoPPZoom(int enable);

typedef struct{
	unsigned int frequency;
	unsigned int symborate;
	unsigned int qam_mode;
	union{
		unsigned int serviceid;
		unsigned int pmtpid;
		struct{
			unsigned int u32VideoPid;
			unsigned int u32AudioPid;
			unsigned int u32PcrPid;
			unsigned int u32EmmPid;
			unsigned int u32AudioEcmPid;
			unsigned int u32VideoEcmPid;
		}PidParam;
	}VodParam;
}PlayerVodPlayParam;

typedef struct{
	unsigned int u32VideoPid;
	unsigned int u32AudioPid;
	unsigned int u32PcrPid;
	unsigned int videofmt;
	unsigned int audiofmt;
}PlayerVodInfo;

typedef enum
{
	PLAYER_VOD_GET_ARECODE,  //info为int类型
	PLAYER_VOD_GET_USERID,
	PLAYER_VOD_GET_PRODUCTID,
}PlayerVodCommand;

typedef enum
{
	PLAYER_HDMI_HDCP_576I_MODE  = 0,
	PLAYER_HDMI_HDCP_OTHER_MODE,
}PlayerHdmiHdcpMode;

typedef struct {
	int  (*play_by_pid)  (PlayerVodPlayParam param, int flags_sync);
	int  (*play_by_serviceid)  (PlayerVodPlayParam param, int flags_sync);
	int  (*play_by_pmtid) (PlayerVodPlayParam param, int flags_sync);
	int  (*pause)  (int flags_sync);
	int  (*resume) (int flags_sync);
	int  (*stop)  (int clear, int flags_sync);
	int  (*seek) (int flags_sync);
	int  (*get_info)(PlayerVodInfo* info);
	int  (*get_info_by_pmtid)(PlayerVodPlayParam param, PlayerVodInfo* info);
	int  (*get_play_info)(int cmd, void* info);
}PlayerVodOption;

extern void GxPlayer_Register_VOD_Options(PlayerVodOption* opt);
extern void GxPlayer_unRegister_Vod_Options(void);
extern status_t GxPlayer_MediaVodPlay(const char* name, const char* srcurl, int64_t start, void* cb);
extern status_t GxPlayer_MediaVodStop(const char* name);
extern status_t GxPlayer_MediaVodSeek(const char* name, int64_t time, SeekFlag flag);
extern status_t GxPlayer_MediaVodPause(const char* name);
extern status_t GxPlayer_MediaVodResume(const char* name);
extern status_t GxPlayer_MediaVodSpeed(const char* name, float speed);
extern status_t GxPlayer_MediaVodGetTime(const char* name, int64_t* start, int64_t* position, int64_t* duration);
extern status_t GxPlayer_MediaVodDvbPause(const char* name);
extern status_t GxPlayer_MediaVodDvbResume(const char* name, int clear_data);
extern status_t GxPlayer_MediaVodDvbSeek(const char* name, int64_t seek_time, SeekFlag flag);

__END_DECLS

#endif

