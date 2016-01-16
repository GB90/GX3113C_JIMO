#ifndef __GXPLAYER_DECODER_H__
#define __GXPLAYER_DECODER_H__

__BEGIN_DECLS

typedef enum {
    AVCODEC_STOPPED        = 0x1000000,
    AVCODEC_RUNNING        ,
    AVCODEC_PAUSED         ,
    AVCODEC_FINISHED       ,
    AVCODEC_UNSUPPORT      ,
    AVCODEC_BUSY           ,
    AVCODEC_ERROR          ,
    AVCODEC_READY		   ,
}AVCodecStatus;

typedef enum {
    VIDEO_CODEC_MPEG12     ,
    VIDEO_CODEC_MPEG4      ,
    VIDEO_CODEC_H263       ,
    VIDEO_CODEC_H264       ,
    VIDEO_CODEC_REAL       ,
    VIDEO_CODEC_AVS        ,
    VIDEO_CODEC_H265       ,
    VIDEO_CODEC_UNKNOWN    ,
}VideoCodecType;

typedef enum {
    AUDIO_CODEC_MPEG1     ,
    AUDIO_CODEC_MPEG2     ,
    AUDIO_CODEC_AAC_LATM   ,
    AUDIO_CODEC_AAC_ADTS   ,
    AUDIO_CODEC_OGG        ,
    AUDIO_CODEC_REAL       ,
    AUDIO_CODEC_AC3        ,
    AUDIO_CODEC_AVS        ,
    AUDIO_CODEC_PCM        ,
    AUDIO_CODEC_EAC3       ,
    AUDIO_CODEC_DTS        ,
    AUDIO_CODEC_DRA1       ,
    AUDIO_CODEC_UNKNOWN    ,
}AudioCodecType;

__END_DECLS

#endif
