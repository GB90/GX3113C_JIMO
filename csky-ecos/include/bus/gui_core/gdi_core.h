#ifndef __GDI_CORE_H__
    #define __GDI_CORE_H__

#include <gxcore.h>

typedef struct GAL_Rect {
	short x, y;
	int w, h;
} GAL_Rect;

#define KSURFACE 0x1
#define VBUFFER  0x2

#define SURFACE_FORMAT_ANY		0x0
#define SURFACE_FORMAT_Y8_444		0x1
#define SURFACE_FORMAT_Y8_400		0x2
#define SURFACE_FORMAT_Y8_422V		0x3
#define SURFACE_FORMAT_Y8_422H		0x4
#define SURFACE_FORMAT_Y8_420		0x5

typedef struct _GuiSurface GuiSurface;
struct _GuiSurface {
	GxSurface      sf;
	GxHwMallocObj *vq;
	uint8_t       *data;
	size_t         buffer_size;
	void          *hw_surface;
	int            bpp;
	int	       format;
	int            hw;
	int	       alpha;
	GuiSurface    *source;
	int            ref;
};

typedef enum _STRING_TYPE
{
	GDI_STRING_PARAGRAPH,
	GDI_STRING_SINGLE
}STRING_TYPE;

typedef enum
{
	GDI_BLIT_STRETCH,
	GDI_BLIT_COPY
}BLIT_FORMAT;

typedef struct GDI_Rect {
	unsigned int x, y;
	unsigned int w, h;
} GDI_Rect;

typedef struct GDI_Interval {
    unsigned int width;
    unsigned int height;
} GDI_Interval;

GuiSurface *hd_get_surface(GAL_Rect *rect, int bpp, void *buffer, int force);
GuiSurface *hd_surface_clone(GxLayerID layer, GAL_Rect *rect, int bpp);
GuiSurface *gal_image_surface(const char *filepath, int *x, int *y, int width, int height);
int hd_blit_surface(GuiSurface *src, GAL_Rect *src_rect, GuiSurface *dst, GAL_Rect *dst_rect);
void hd_putpixel(GuiSurface *surface, int x, int y, unsigned int pixel);
int hd_rotate_surface(GuiSurface *src, GAL_Rect *src_rect, GuiSurface *dst, GAL_Rect *dst_rect, GxScrewMode mode);
void hd_free_surface(GuiSurface *surface);
int  hd_check_surface(GuiSurface *surface);
void hd_clear(GxLayerID layer);
status_t gal_capture_jpeg(const unsigned char *temp_dir, const unsigned char *output_file);

/*OSD surface*/
void* 		GDI_GetMainOSDSurface(void);
void*	 	GDI_GetOSDSurface(GDI_Rect *rect, int bpp);
status_t 	GDI_SPP_OnOff(int flag);
status_t 	GDI_OSD_OnOff(int flag);
/*Color*/
int             GDI_GetColorKey(void);

/*SPP surface*/
void* 		GDI_GetMainSPPSurface(void);
void*	 	GDI_GetSPPSurface(GDI_Rect *rect);

/*Image*/
status_t 	GDI_DrawImage(void *surface, const void *pdesc, int x, int y);
status_t	GDI_DrawSPP(void *surface, const void *pdesc);
int			GDI_GetImageWidth(const void *pdesc);
int			GDI_GetImageHeight(const void *pdesc);
int			GDI_GetImageBpp(const void *pdesc);
status_t 	GDI_ClearSPP(void);

/*Rectangle*/
status_t 	GDI_FillRect(void *surface, GDI_Rect *rect, int color);

/*String*/
void*		GDI_GetFont(void);
void*		GDI_SetFont(const unsigned char *name);
int		GDI_GetFontHeight(void* pfont);
void*		GDI_I18N_String(const char *string);
status_t 	GDI_DrawString(void *surface, void *string, GDI_Rect *rect, int alignment, int color, STRING_TYPE flag);
int 		GDI_GetStringPixels(void *string);

/*Blit*/
status_t 	GDI_Blit(void *src, GDI_Rect *src_rect, void *dst, GDI_Rect *dst_rect, BLIT_FORMAT flag);
status_t 	GDI_Begin(void);
status_t 	GDI_End(void);
status_t 	GDI_Flip(void);

/*Special Font*/
typedef enum _ALIGNMENT_TYPE
{
    /*Text alignment flags, horizontal*/
    ALIGNMENT_HORIZONTAL = (3<<0),
    ALIGNMENT_LEFT	 = (0<<0),
    ALIGNMENT_RIGHT      = (1<<0),
    ALIGNMENT_HCENTRE	 = (2<<0),

    /*Text alignment flags, vertical*/
    ALIGNMENT_VERTICAL	 = (3<<2),
    ALIGNMENT_TOP	 = (0<<2),
    ALIGNMENT_BOTTOM	 = (1<<2),
    ALIGNMENT_BASELINE	 = (2<<2),
    ALIGNMENT_VCENTRE	 = (3<<2)
}ALIGNMENT_TYPE;

typedef enum _TEXT_FORMAT
{
    PARAGRAPH_TEXT,
    SINGLELINE_TEXT
}TEXT_FORMAT;

typedef status_t GXGDI_GetPixelLen(void *string);
typedef status_t GXGDI_GetLines(void *string, GDI_Rect *rect, GDI_Interval *interval);
typedef status_t GXGDI_DrawText(void *screen, 
				void *string, 
				GDI_Rect *rect, 
				GDI_Interval *interval, 
				ALIGNMENT_TYPE alignment, 
				int color, 
				TEXT_FORMAT flag);
typedef int	 GXGDI_SkipLine(void *start, 
				void *string, 
				GDI_Interval *interval, 
				int line_width, 
				ALIGNMENT_TYPE alignment);
typedef status_t GXGDI_RollText(void *screen, 
				void *string, 
				GDI_Rect *rect, 
				int pos, 
				int color, 
				TEXT_FORMAT flag); 

typedef struct _GXGDI_FontOpt {
    char name[10];
    GXGDI_GetPixelLen *get_len;
    GXGDI_GetLines    *get_lines;
    GXGDI_DrawText    *draw_text;
    GXGDI_SkipLine    *skip_line;
    GXGDI_RollText    *roll_text;
}GXGDI_FontOpt;

status_t GDI_Font_RegisterOpt(GXGDI_FontOpt *opt);
status_t GDI_Font_UnRegisterOpt(GXGDI_FontOpt *opt);

/*Image Register*/
int GDI_RegisterJPEG(void);
int GDI_UnRegisterJPEG(void);
int GDI_RegisterJPEG_SOFT(void);
int GDI_RegisterGIF(void);
int GDI_RegisterPNG(void);
int GDI_RegisterAll(void);

/*DVB I18N Code Set*/

/*Single First Byte*/
#define DVB_FIRST_ISO8859_5		(0x01)	/*Cyrillic*/
#define DVB_FIRST_ISO8859_6		(0x02)	/*Arabic*/
#define DVB_FIRST_ISO8859_7		(0x03)	/*Greek*/
#define DVB_FIRST_ISO8859_8		(0x04)	/*Hebrew*/
#define DVB_FIRST_ISO8859_9		(0x05)	/*Latin alphabet No. 5*/
#define DVB_FIRST_ISO8859_10		(0x06)	/*Latin alphabet No. 6*/
#define DVB_FIRST_ISO8859_11		(0x07)  /*Thai*/
#define DVB_FIRST_CODE_0x08		(0x08)	/*Reserved for future use*/
#define DVB_FIRST_ISO8859_13		(0x09)	/*Latin alphabet No. 7*/
#define DVB_FIRST_ISO8859_14		(0x0A)	/*Latin alphabet No. 8(Celic)*/
#define DVB_FIRST_ISO8859_15		(0x0B)	/*Latin alphabet No. 9*/ 
#define DVB_FIRST_CODE_0x0C		(0x0C)	/*Reserved for future use*/
#define DVB_FIRST_CODE_0x0D		(0x0D)	/*Reserved for future use*/
#define DVB_FIRST_CODE_0x0E		(0x0E)	/*Reserved for future use*/
#define DVB_FIRST_CODE_0x0F		(0x0F)	/*Reserved for future use*/
#define DVB_FIRST_ISO8859		(0x10)	/*Set Double Byte*/
#define DVB_FIRST_ISOIEC10646		(0x11)	/*ISO/IEC 10646*/
#define DVB_FIRST_KSX101_2004		(0x12)	/*Korean*/
#define DVB_FIRST_GB2312		(0x13)	/*Simplified Chinese*/
#define DVB_FIRST_BIG5			(0x14)	/*Big5 Traditional Chinese in Taiwan*/
#define DVB_FIRST_UTF8			(0x15)	/*UTF8*/
#define DVB_FIRST_CODE_16		(0x16)	/*Reserved for future use*/
#define DVB_FIRST_CODE_17		(0x17)	/*Reserved for future use*/
#define DVB_FIRST_CODE_18		(0x18)	/*Reserved for future use*/
#define DVB_FIRST_CODE_19		(0x19)	/*Reserved for future use*/
#define DVB_FIRST_CODE_1A		(0x1A)	/*Reserved for future use*/
#define DVB_FIRST_CODE_1B		(0x1B)	/*Reserved for future use*/
#define DVB_FIRST_CODE_1C		(0x1C)	/*Reserved for future use*/
#define DVB_FIRST_CODE_1D		(0x1D)	/*Reserved for future use*/
#define DVB_FIRST_CODE_1E		(0x1F)	/*Reserved for future use*/
    
/*Second Byte*/
/*Reserved for future use*/

/*Third Byte*/
#define DVB_THIRD_ISO8859_1		(0x01)	/*West European*/
#define DVB_THIRD_ISO8859_2		(0x02)	/*East European*/
#define DVB_THIRD_ISO8859_3		(0x03)	/*South European*/
#define DVB_THIRD_ISO8859_4		(0x04)	/*North and North-East European*/
#define DVB_THIRD_ISO8859_5		(0x05)	/*Cyrillic*/
#define DVB_THIRD_ISO8859_6		(0x06)	/*Arabic*/
#define DVB_THIRD_ISO8859_7		(0x07)	/*Greek*/
#define DVB_THIRD_ISO8859_8		(0x08)	/*Hebrew*/
#define DVB_THIRD_ISO8859_9		(0x09)	/*West European & Turkish*/
#define DVB_THIRD_ISO8859_10		(0x0A)	/*North European*/
#define DVB_THIRD_ISO8859_11		(0x0B)	/*Thai*/
#define DVB_THIRD_0x0C			(0x0C)	/*Reserved for future use*/
#define DVB_THIRD_ISO8859_13		(0x0D)	/*Baltic*/
#define DVB_THIRD_ISO8859_14		(0x0E)	/*Celtic*/
#define DVB_THIRD_ISO8859_15		(0x0F)	/*West European*/

/*Discard*/
#define DVB_DISCARD			(0x80)	/*Discard the first code*/

typedef struct _DVB_CodeSet{
    unsigned char first_byte;
    unsigned char second_byte;
    unsigned char third_byte;
}DVB_CodeSet;

typedef struct _DVB_CodeTable{
    unsigned int flag;
    DVB_CodeSet src;
    DVB_CodeSet dst;
}DVB_CodeTable;

/*Set special DVB code*/
int gxgdi_set_dvbcode(DVB_CodeSet *src, DVB_CodeSet *dst);

/*i18n convert interface*/
int gxgdi_iconv(const unsigned char *from_str, 
		unsigned char **to_str,
		unsigned int from_size,
		unsigned int *to_size,
		unsigned char *iso_str);
status_t gal_add_image(const char *key, const char *path);
status_t gal_add_key_path(const char *key, const char *path);

#endif


