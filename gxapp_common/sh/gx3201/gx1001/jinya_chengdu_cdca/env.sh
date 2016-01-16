unset CHIP
unset ARCH
unset OS
unset CROSS_PATH
unset GXLIB_PATH
unset GXSRC_PATH
unset RC_VERSION
unset GX_KERNEL_PATH

if [ "$1" = "csky-ecos" ] ; then
	echo ARCH=csky OS=ecos Configuration  !
	export ARCH=csky
	export OS=ecos
	export CROSS_PATH=csky-ecos
fi

if [ "$1" = "csky-linux" ] ; then
	echo ARCH=csky OS=linux Configuration  !
	export ARCH=csky
	export OS=linux
	export CROSS_PATH=csky-linux
fi

if [ -z "$CROSS_PATH" ] ; then
	echo Config Errror!!!
	echo "               "              
	echo eg: source env.sh csky-ecos
	echo "    "source env.sh csky-linux
fi

export RC_VERSION=GOXCEED_v1_9_1_1
export GXLIB_PATH=/opt/goxceed/$CROSS_PATH
export GXSRC_PATH=`pwd`
export GX_KERNEL_PATH=$GXSRC_PATH/../linux-2.6.27.55

unset DVB_MARKET
unset DVB_CA_MARKET
unset DVB_AD_MARKET
unset DVB_CA_FLAG
unset DVB_USB_FLAG
unset DVB_PVR_SPEED_SUPPORT
unset DVB_PANEL_TYPE
unset PANEL_CLK_GPIO
unset PANEL_DATA_GPIO
unset PANEL_STANDBY_GPIO
unset PANEL_LOCK_GPIO
unset DVB_KEY_TYPE
unset DVB_TUNER_TYPE
unset DVB_DEMOD_TYPE
unset DVB_DEMOD_MODE
unset DVB_TS_SRC
unset DVB_AUTO_TEST_FLAG

#CA枚举（与app_common_porting_stb_api.h中dvb_ca_type_t结构体中对应）
unset DVB_CA_TYPE
#对应市场（厂家）CA链接库名称（如libY1120-tonghui-gx3001-20121212D.a，设为Y1120-tonghui-gx3001-20121212D）
unset DVB_CA_1_LIB
unset DVB_CA_2_LIB
#对应CA代码中宏定义
unset DVB_CA_1_FLAG
unset DVB_CA_2_FLAG
#对应CA名称（目录）,如（cdcas3.0）
unset DVB_CA_1_NAME
unset DVB_CA_2_NAME

unset DVB_AD_NAME
unset DVB_AD_TYPE
unset DVB_AD_LIB
unset DVB_OTA_TYPE

unset DVB_JPG_LOGO
unset AUDIO_DOLBY
unset DVB_SUBTITLE_FLAG
unset DVB_PVR_FLAG
unset DVB_NETWORK_FLAG
unset LINUX_OTT_SUPPORT

unset DVB_HD_LIST
unset DVB_LOGO_JPG
unset DVB_WORK_PATH
unset DVB_LOGO_PATH
unset DVB_I_FRAME_PATH

unset DVB_RESOLUTION
unset DVB_SYS_MODE
unset DVB_VIDEO_X
unset DVB_VIDEO_Y
unset DVB_VIDEO_W
unset DVB_VIDEO_H
unset DVB_SAT_MAX
unset DVB_TP_MAX
unset DVB_SERVICE_MAX
unset DVB_PAT_TIMEOUT
unset DVB_SDT_TIMEOUT
unset DVB_NIT_TIMEOUT
unset DVB_PMT_TIMEOUT
unset DVB_LCN_DEFAULT
unset DVB_CENTER_FRE
unset DVB_CENTER_SYMRATE
unset DVB_CENTER_QAM

export CHIP=gx3201
#宏开关、配置项

#市场:XXX(市场)_XXX(厂家),如sichuan_boxsam,shanxi_jizhong等。
#针对同一市场可能存在多个厂家。不同厂家因硬件平台、CA库等差异，需兼容差异化编译
#对应CA目录gxapp_common/cas/$DVB_CA_1_NAME/$DVB_CA_MARKET/; gxapp_common/cas/$DVB_CA_2_NAME/$DVB_CA_MARKET/;
export DVB_MARKET=jinya_gongban
export DVB_CA_MARKET=gongban
export DVB_AD_MARKET=gongban
export DVB_AD_MARKET_1=gongban

#是否编译CA，如值为no则为清流版本
export DVB_CA_FLAG=yes

if [ $DVB_CA_FLAG = "yes" ]; then
	#CA枚举（与app_common_porting_stb_api.h中dvb_ca_type_t结构体中对应）
	export DVB_CA_TYPE=DVB_CA_TYPE_CDCAS30
	export DVB_CA_1_NAME=cd_cas30
	export DVB_CA_1_LIB=Y1125-jinya-GX3201-20140801D
	export DVB_CA_1_FLAG=DVB_CA_TYPE_CD_CAS30_FLAG
fi

#兼容双CA。多数情况下只有一个CA，第二个CA不用设置
#if [ $DVB_CA_FLAG = "yes" ]; then
#	export DVB_CA_2_NAME=byca
#	export DVB_CA_2_LIB=byca
#	export DVB_CA_2_FLAG=DVB_CA_TYPE_BY_FLAG
#fi

# set ad support type
export DVB_AD_NAME=cmisads
export DVB_AD_FLAG=DVB_AD_TYPE_CMIS_FLAG

export DVB_AD_NAME_1=jiuzhouads
export DVB_AD_FLAG_1=DVB_AD_TYPE_JIUZHOU_FLAG

#set ota type
export DVB_OTA_TYPE=DVB_NATIONALCHIP_OTA

#set HD prog list yes,no
export DVB_HD_LIST=no

# set JPG LOGO SUPPORT yes, no
export DVB_JPG_LOGO=yes

# set audio dolby decode SUPPORT yes, no
export AUDIO_DOLBY=yes

# set dvb pvr function support yes or no 
export DVB_PVR_FLAG=no

# set dvb subtitle function support yes or no 
export DVB_SUBTITLE_FLAG=no

# set network function support yes or no (only linux support network )
export DVB_NETWORK_FLAG=no

# set ott function support yes or no 
export LINUX_OTT_SUPPORT=no

#是否支持USB升级
export DVB_USB_FLAG=yes

#是否支持PVR快进、快退
export DVB_PVR_SPEED_SUPPORT=0

#选用前面板类型 panel_xxx(市场)_xxx(厂家)_xxx(面板类型)，如PANEL_TYPE_fd650_RUNDE
#已支持面板参考：include/bsp/panel_enum.h
#export DVB_PANEL_TYPE=PANEL_TYPE_fd650_RUNDE
export DVB_PANEL_TYPE=PANEL_TYPE_CT1642_JINYA
export PANEL_CLK_GPIO=13
export PANEL_DATA_GPIO=12
export PANEL_STANDBY_GPIO=11
export PANEL_LOCK_GPIO=24

#选用遥控器宏定义类型 keymap_xxx(市场)_(厂家)，如KEY_GONGBAN_NATIONALCHIP_NEW
#已支持面板参考:key_xml目录下的.xml group="KEY_GONGBAN_NATIONALCHIP_NEW"等，注意同一个kex(x).xml不能出现遥控器串键的情况（串键遥控器必须不不同key(x).xml中定义）
export DVB_KEY_TYPE=KEY_JINYA

# demod type 
#已支持demod参考：include/search/demod_enum.h
export DVB_DEMOD_TYPE=DVB_DEMOD_GX1001

# demod mode 
#参考：include/search/demod_enum.h
export DVB_DEMOD_MODE=DVB_DEMOD_DVBC

# select ts output : 0,1,2
export DVB_TS_SRC=0

# tuner enum ,参考 demod/include/tuners.h
export DVB_TUNER_TYPE=60

#已支持开机画面图片，参考：logo_jpg目录下的图片
#export DVB_LOGO_JPG=logo_nationalchip_gongban.jpg
export DVB_LOGO_JPG=logo_sichuan_chendu_sd.jpg
if [ "$1" = "csky-linux" ] ; then
# XML、图片等路径
export DVB_WORK_PATH='"/dvb/"'
# 广播背景图片路径
export DVB_LOGO_PATH='"/dvb/theme/logo.bin"'
export DVB_I_FRAME_PATH='"/dvb/theme/logo.bin"'
fi

if [ "$1" = "csky-ecos" ] ; then
# 广播背景图片路径
export DVB_LOGO_PATH='"/theme/logo.bin"'
export DVB_I_FRAME_PATH='"/theme/logo.bin"'
export DVB_NETWORK_FLAG=no
fi


#宏定义参数项
export DVB_RESOLUTION=HD
#音视频同步方式 0-PCR_RECOVER  1-AVPTS_RECOVER 
export DVB_SYS_MODE=0
# 全频视频X位置
export DVB_VIDEO_X=0
# 全频视频Y位置
export DVB_VIDEO_Y=0
# 全频视频宽大小
export DVB_VIDEO_W=1280
# 全频视频高大小
export DVB_VIDEO_H=720
# 卫星最大个数（有线、地面方案基本设置为1）
export DVB_SAT_MAX=1
# TP频点最大个数
export DVB_TP_MAX=200
# 节目最大个数
export DVB_SERVICE_MAX=1000
# 搜索PAT超时时间（ms）
export DVB_PAT_TIMEOUT=3000
# 搜索SDT超时时间（ms）
export DVB_SDT_TIMEOUT=8000
# 搜索NIT超时时间（ms）
export DVB_NIT_TIMEOUT=10000
# 搜索PMT超时时间（ms）
export DVB_PMT_TIMEOUT=8000
# prog without lcn num, begin from DVB_LCN_DEFAULT
export DVB_LCN_DEFAULT=500
export DVB_CENTER_FRE=315
export DVB_CENTER_SYMRATE=6875
export DVB_CENTER_QAM=2
#硬件自动测试开关。yes- 开启自动测试功能 no -关闭自动测试功能
export DVB_AUTO_TEST_FLAG=no

# echo export path
if [ -z "$CROSS_PATH" ] ; then
	echo
else
	echo CHIP=$CHIP
	echo PATH:
	echo RC_VERSION=$RC_VERSION
	echo GX_KERNEL_PATH=$GX_KERNEL_PATH
	echo GXLIB_PATH=$GXLIB_PATH
	echo DVB_MARKET=$DVB_MARKET
	echo DVB_CA_FLAG=$DVB_CA_FLAG
	if [ $DVB_CA_FLAG = "yes" ]; then
		echo DVB_CA_MARKET=$DVB_CA_MARKET
		echo DVB_CA_TYPE=$DVB_CA_TYPE
		echo DVB_CA_1_NAME=$DVB_CA_1_NAME
#		echo DVB_CA_1_LIB=$DVB_CA_1_LIB
#		echo DVB_CA_1_FLAG=$DVB_CA_1_FLAG
		echo DVB_CA_2_NAME=$DVB_CA_2_NAME
#		echo DVB_CA_2_LIB=$DVB_CA_2_LIB
#		echo DVB_CA_2_FLAG=$DVB_CA_2_FLAG
	fi
	if [ -z "$DVB_AD_NAME" ] ; then
		echo
	else
		echo DVB_AD_MARKET=$DVB_AD_MARKET
		echo DVB_AD_NAME=$DVB_AD_NAME
#		echo DVB_AD_FLAG=$DVB_AD_FLAG
#		echo DVB_AD_LIB=$DVB_AD_LIB
	fi
	echo DVB_OTA_TYPE=$DVB_OTA_TYPE
	echo DVB_JPG_LOGO=$DVB_JPG_LOGO
	echo DVB_HD_LIST=$DVB_HD_LIST
	echo DVB_USB_FLAG=$DVB_USB_FLAG
	echo DVB_PVR_FLAG=$DVB_PVR_FLAG
	echo DVB_SUBTITLE_FLAG=$DVB_SUBTITLE_FLAG
	echo AUDIO_DOLBY=$AUDIO_DOLBY
	echo DVB_NETWORK_FLAG=$DVB_NETWORK_FLAG
	echo LINUX_OTT_SUPPORT=$LINUX_OTT_SUPPORT
	echo DVB_PVR_SPEED_SUPPORT=$DVB_PVR_SPEED_SUPPORT
	echo DVB_PANEL_TYPE=$DVB_PANEL_TYPE
	echo DVB_KEY_TYPE=$DVB_KEY_TYPE
	echo DVB_LOGO_JPG=$DVB_LOGO_JPG
	echo DVB_TUNER_TYPE=$DVB_TUNER_TYPE
	echo DVB_DEMOD_TYPE=$DVB_DEMOD_TYPE
	echo DVB_DEMOD_MODE=$DVB_DEMOD_MODE
	echo DVB_TS_SRC=$DVB_TS_SRC
	echo DVB_RESOLUTION=$DVB_RESOLUTION
	echo DVB_SYS_MODE=$DVB_SYS_MODE
	echo DVB_CENTER_FRE=$DVB_CENTER_FRE
#	echo DVB_CENTER_SYMRATE=$DVB_CENTER_SYMRATE
#	echo DVB_CENTER_QAM=$DVB_CENTER_QAM
    echo DVB_AUTO_TEST_FLAG=$DVB_AUTO_TEST_FLAG
fi
echo SVN PATH:
echo  https://isvn.guoxintech.com/svn/3201_DVBC/gxapp_common
echo  https://isvn.guoxintech.com/svn/3201_DVBC/gongban/development
echo  https://isvn.guoxintech.com/svn/3201_DVBC/demod
echo  https://isvn.guoxintech.com/svn/3201_DVBC/goxceed-v1.9.2/gxloader
echo  https://isvn.guoxintech.com/svn/3201_DVBC/gxlowpower
echo  https://isvn.guoxintech.com/svn/3201_DVBC/goxceed-v1.9.2/gxota





 




