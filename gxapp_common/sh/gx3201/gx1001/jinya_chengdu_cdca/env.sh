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

#CAö�٣���app_common_porting_stb_api.h��dvb_ca_type_t�ṹ���ж�Ӧ��
unset DVB_CA_TYPE
#��Ӧ�г������ң�CA���ӿ����ƣ���libY1120-tonghui-gx3001-20121212D.a����ΪY1120-tonghui-gx3001-20121212D��
unset DVB_CA_1_LIB
unset DVB_CA_2_LIB
#��ӦCA�����к궨��
unset DVB_CA_1_FLAG
unset DVB_CA_2_FLAG
#��ӦCA���ƣ�Ŀ¼��,�磨cdcas3.0��
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
#�꿪�ء�������

#�г�:XXX(�г�)_XXX(����),��sichuan_boxsam,shanxi_jizhong�ȡ�
#���ͬһ�г����ܴ��ڶ�����ҡ���ͬ������Ӳ��ƽ̨��CA��Ȳ��죬����ݲ��컯����
#��ӦCAĿ¼gxapp_common/cas/$DVB_CA_1_NAME/$DVB_CA_MARKET/; gxapp_common/cas/$DVB_CA_2_NAME/$DVB_CA_MARKET/;
export DVB_MARKET=jinya_gongban
export DVB_CA_MARKET=gongban
export DVB_AD_MARKET=gongban
export DVB_AD_MARKET_1=gongban

#�Ƿ����CA����ֵΪno��Ϊ�����汾
export DVB_CA_FLAG=yes

if [ $DVB_CA_FLAG = "yes" ]; then
	#CAö�٣���app_common_porting_stb_api.h��dvb_ca_type_t�ṹ���ж�Ӧ��
	export DVB_CA_TYPE=DVB_CA_TYPE_CDCAS30
	export DVB_CA_1_NAME=cd_cas30
	export DVB_CA_1_LIB=Y1125-jinya-GX3201-20140801D
	export DVB_CA_1_FLAG=DVB_CA_TYPE_CD_CAS30_FLAG
fi

#����˫CA�����������ֻ��һ��CA���ڶ���CA��������
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

#�Ƿ�֧��USB����
export DVB_USB_FLAG=yes

#�Ƿ�֧��PVR���������
export DVB_PVR_SPEED_SUPPORT=0

#ѡ��ǰ������� panel_xxx(�г�)_xxx(����)_xxx(�������)����PANEL_TYPE_fd650_RUNDE
#��֧�����ο���include/bsp/panel_enum.h
#export DVB_PANEL_TYPE=PANEL_TYPE_fd650_RUNDE
export DVB_PANEL_TYPE=PANEL_TYPE_CT1642_JINYA
export PANEL_CLK_GPIO=13
export PANEL_DATA_GPIO=12
export PANEL_STANDBY_GPIO=11
export PANEL_LOCK_GPIO=24

#ѡ��ң�����궨������ keymap_xxx(�г�)_(����)����KEY_GONGBAN_NATIONALCHIP_NEW
#��֧�����ο�:key_xmlĿ¼�µ�.xml group="KEY_GONGBAN_NATIONALCHIP_NEW"�ȣ�ע��ͬһ��kex(x).xml���ܳ���ң�������������������ң�������벻��ͬkey(x).xml�ж��壩
export DVB_KEY_TYPE=KEY_JINYA

# demod type 
#��֧��demod�ο���include/search/demod_enum.h
export DVB_DEMOD_TYPE=DVB_DEMOD_GX1001

# demod mode 
#�ο���include/search/demod_enum.h
export DVB_DEMOD_MODE=DVB_DEMOD_DVBC

# select ts output : 0,1,2
export DVB_TS_SRC=0

# tuner enum ,�ο� demod/include/tuners.h
export DVB_TUNER_TYPE=60

#��֧�ֿ�������ͼƬ���ο���logo_jpgĿ¼�µ�ͼƬ
#export DVB_LOGO_JPG=logo_nationalchip_gongban.jpg
export DVB_LOGO_JPG=logo_sichuan_chendu_sd.jpg
if [ "$1" = "csky-linux" ] ; then
# XML��ͼƬ��·��
export DVB_WORK_PATH='"/dvb/"'
# �㲥����ͼƬ·��
export DVB_LOGO_PATH='"/dvb/theme/logo.bin"'
export DVB_I_FRAME_PATH='"/dvb/theme/logo.bin"'
fi

if [ "$1" = "csky-ecos" ] ; then
# �㲥����ͼƬ·��
export DVB_LOGO_PATH='"/theme/logo.bin"'
export DVB_I_FRAME_PATH='"/theme/logo.bin"'
export DVB_NETWORK_FLAG=no
fi


#�궨�������
export DVB_RESOLUTION=HD
#����Ƶͬ����ʽ 0-PCR_RECOVER  1-AVPTS_RECOVER 
export DVB_SYS_MODE=0
# ȫƵ��ƵXλ��
export DVB_VIDEO_X=0
# ȫƵ��ƵYλ��
export DVB_VIDEO_Y=0
# ȫƵ��Ƶ���С
export DVB_VIDEO_W=1280
# ȫƵ��Ƶ�ߴ�С
export DVB_VIDEO_H=720
# ���������������ߡ����淽����������Ϊ1��
export DVB_SAT_MAX=1
# TPƵ��������
export DVB_TP_MAX=200
# ��Ŀ������
export DVB_SERVICE_MAX=1000
# ����PAT��ʱʱ�䣨ms��
export DVB_PAT_TIMEOUT=3000
# ����SDT��ʱʱ�䣨ms��
export DVB_SDT_TIMEOUT=8000
# ����NIT��ʱʱ�䣨ms��
export DVB_NIT_TIMEOUT=10000
# ����PMT��ʱʱ�䣨ms��
export DVB_PMT_TIMEOUT=8000
# prog without lcn num, begin from DVB_LCN_DEFAULT
export DVB_LCN_DEFAULT=500
export DVB_CENTER_FRE=315
export DVB_CENTER_SYMRATE=6875
export DVB_CENTER_QAM=2
#Ӳ���Զ����Կ��ء�yes- �����Զ����Թ��� no -�ر��Զ����Թ���
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





 




