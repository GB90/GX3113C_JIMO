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

export RC_VERSION=GOXCEED_v1_9_4
export GXLIB_PATH=/opt/goxceed/$CROSS_PATH
export GXSRC_PATH=`pwd`
#export GX_KERNEL_PATH=$GXSRC_PATH/../linux-2.6.27.55
export GX_KERNEL_PATH=$GXSRC_PATH/../../../rc7_1213/v1.9.2/linux-2.6.27.55


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
unset MOVE_FUNCTION_FLAG
unset DVB_ZOOM_RESTART_PLAY
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
unset DVB_SERIAL_SUPPORT
unset CUSTOMER
unset DVB_32MB
unset DVB_SERIAL_YUEHAI
unset FACTORY_SERIALIZATION_SUPPORT

export DVB_32MB=no

export CHIP=gx3113c
#宏开关、配置项

#市场:XXX(市场)_XXX(厂家),如sichuan_boxsam,shanxi_jizhong等。
#针对同一市场可能存在多个厂家。不同厂家因硬件平台、CA库等差异，需兼容差异化编译
#对应CA目录gxapp_common/cas/$DVB_CA_1_NAME/$DVB_CA_MARKET/; gxapp_common/cas/$DVB_CA_2_NAME/$DVB_CA_MARKET/;
export DVB_MARKET=gongban
export DVB_CA_MARKET=gongban
export DVB_AD_MARKET=gongban

#客户
#export CUSTOMER=CUST_NEIMENG  #长沙泰辉
#export CUSTOMER=CUST_SUKE
#export CUSTOMER=CUST_YUEHAI  #粤海
#export CUSTOMER=CUST_YULIN   #榆林
#export CUSTOMER=CUST_XINGTAI  #邢台
#export CUSTOMER=CUST_TAIHUI_MG
#export CUSTOMER=CUST_YUNNAN   #云南勐拉晨光
#export CUSTOMER=CUST_CANGZHOU  #山东沧州
#export CUSTOMER=CUST_MG  
export CUSTOMER=CUST_JIMO    #即墨  
#export CUSTOMER=CUST_SHANXI    #山西无线 
#export CUSTOMER=CUST_WUKA_LEDOU
#export CUSTOMER=CUST_LEDOU
echo DVB_CUSTOMER_NAME=$CUSTOMER
if [ $CUSTOMER = "CUST_NEIMENG" ]; then
	export DVB_LOGO_JPG=logo_etuokeqi.jpg		#neimeng
	export DVB_KEY_TYPE=KEY_TAIHUI
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_FLAG=yes
	#数视通
	export DVB_CA_TYPE=DVB_CA_TYPE_DIVITONE
	export DVB_CA_1_NAME=divitone_cas
	export DVB_CA_1_LIB=DiviGuardCalib-Taihui-GX3113C-release-20150409
	export DVB_CA_1_FLAG=DVB_CA_TYPE_DIVITONE_FLAG
	export DVB_CENTER_FRE=578
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_SUKE" ]; then

	export DVB_CA_FLAG=yes
	export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
	export DVB_CA_1_NAME=desai_cas50
	export DVB_CA_1_LIB=ca_52
	export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG
	#数视通
#	export DVB_CA_TYPE=DVB_CA_TYPE_DIVITONE
#	export DVB_CA_1_NAME=divitone_cas
	#export DVB_CA_1_LIB=DiviGuardCalib-Taihui-GX3113C-release-20150409
#	export DVB_CA_1_LIB=DiviGuardCalib-Keneng-GX3201-release-20140402
#	export DVB_CA_1_FLAG=DVB_CA_TYPE_DIVITONE_FLAG

	export DVB_CENTER_FRE=578
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_TONGHUI
	export DVB_PANEL_TYPE=PANEL_TYPE_2KEY # PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_YUEHAI" ]; then
	export DVB_CA_FLAG=no
	export DVB_SERIAL_YUEHAI=yes
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_CENTER_FRE=578
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_YULIN" ]; then
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_LOGO_JPG=logo_yulin.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
	 export DVB_CA_1_NAME=desai_cas50
	 export DVB_CA_1_LIB=ca_52
	 export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG
	export DVB_AD_NAME=dsads
	export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
	export DVB_AD_LIB=dsad
	export DVB_CENTER_FRE=706
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_XINGTAI" ]; then
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_TYPE=DVB_CA_TYPE_TR
	export DVB_CA_1_NAME=tr_cas
	export DVB_CA_1_LIB=TrMCell_450_Gx3113c
	export DVB_CA_1_FLAG=DVB_CA_TYPE_TR_FLAG
	export DVB_CA_2_NAME=desai_cas50
	export DVB_CA_2_LIB=ca_52
	export DVB_CA_2_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG

	export DVB_AD_NAME=dsads
	export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
	export DVB_AD_LIB=dsad
	export DVB_CENTER_FRE=578
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_TAIHUI_MG" ]; then
	export DVB_KEY_TYPE=KEY_TAIHUI
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=578
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_CA_TYPE=DVB_CA_TYPE_MG312
	export DVB_CA_1_NAME=mg_cas312
	export DVB_CA_1_LIB=mgcaslib_v312-GX3113C-runde
	export DVB_CA_1_FLAG=DVB_CA_TYPE_MG312_FLAG
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_YUNNAN" ]; then
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=634
	export DVB_LOGO_JPG=logo_yunnan.jpg
	export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
	 export DVB_CA_1_NAME=desai_cas50
	 export DVB_CA_1_LIB=ca_52
	 export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG
	export DVB_AD_NAME=dsads
	export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
	export FACTORY_SERIALIZATION_SUPPORT=no
	export DVB_AD_LIB=dsad
elif [ $CUSTOMER = "CUST_CANGZHOU" ]; then
#	export DVB_KEY_TYPE=KEY_TAOYUAN
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=554
	export DVB_LOGO_JPG=logo_cangzhou.jpg
	export FACTORY_SERIALIZATION_SUPPORT=no
	export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
	 export DVB_CA_1_NAME=desai_cas50
	 export DVB_CA_1_LIB=ca_52
	 export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG
	export DVB_AD_NAME=dsads
	export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
	export DVB_AD_LIB=dsad
elif [ $CUSTOMER = "CUST_WUKA_LEDOU" ]; then
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=578
	export DVB_CA_FLAG=yes
	export DVB_CA_TYPE=DVB_CA_TYPE_DVB
	export DVB_CA_1_NAME=dvb_cas
	export DVB_CA_1_LIB=ca
	export DVB_CA_1_FLAG=DVB_CA_TYPE_DVB_FLAG
	export FACTORY_SERIALIZATION_SUPPORT=yes
elif [ $CUSTOMER = "CUST_LEDOU" ]; then
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=578
	export DVB_CA_FLAG=yes
	export DVB_CA_TYPE=DVB_CA_TYPE_DVB
	export DVB_CA_1_NAME=dvb_cas
	export DVB_CA_1_LIB=
	export DVB_CA_1_FLAG=DVB_CA_TYPE_DVB_FLAG
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_JIMO" ]; then
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_LOGO_JPG=logo_jimo.jpg
	export DVB_KEY_TYPE=KEY_JIMO
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
	 export DVB_CA_1_NAME=desai_cas50
	 export DVB_CA_1_LIB=ca_52
	 export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG
	export DVB_AD_NAME=dsads
	export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
	export DVB_AD_LIB=dsad
	export DVB_CENTER_FRE=714
	export FACTORY_SERIALIZATION_SUPPORT=no
elif [ $CUSTOMER = "CUST_SHANXI" ]; then
	export DVB_CA_FLAG=yes
	export DVB_SERIAL_YUEHAI=no
	export DVB_LOGO_JPG=logo_shanxi.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_CA_TYPE=DVB_CA_TYPE_MG
	export DVB_CA_1_NAME=mg_cas
	export DVB_CA_1_LIB=mgcaslib_v423-GX3201
	export DVB_CA_1_FLAG=DVB_CA_TYPE_MG_FLAG
	export DVB_CENTER_FRE=690
	export FACTORY_SERIALIZATION_SUPPORT=no
else
	export DVB_LOGO_JPG=logo_thinewtec.jpg
	export DVB_KEY_TYPE=KEY_THINEWTEC
	export DVB_PANEL_TYPE=PANEL_TYPE_fd650_THINEWTEC #PANEL_TYPE_2KEY 
	export DVB_SERIAL_YUEHAI=no
	export DVB_CENTER_FRE=578
	export FACTORY_SERIALIZATION_SUPPORT=no
	#是否编译CA，如值为no则为清流版本
	export DVB_CA_FLAG=no

	if [ $DVB_CA_FLAG = "yes" ]; then
		#CA枚举（与app_common_porting_stb_api.h中dvb_ca_type_t结构体中对应）
		#	export DVB_CA_TYPE=DVB_CA_TYPE_MG
		#	export DVB_CA_1_NAME=mg_cas
		#	export DVB_CA_1_LIB=mgcaslib_v423-GX3201
		#	export DVB_CA_1_FLAG=DVB_CA_TYPE_MG_FLAG
		#	export DVB_CA_TYPE=DVB_CA_TYPE_MG312
		#		export DVB_CA_1_NAME=mg_cas312
		#		export DVB_CA_1_LIB=mgcaslib_v312-GX3113C-runde
		#		export DVB_CA_1_FLAG=DVB_CA_TYPE_MG312_FLAG
		#全智
		#		export DVB_CA_TYPE=DVB_CA_TYPE_QZ
		#		export DVB_CA_1_NAME=qz_cas
		#		export DVB_CA_1_LIB=
		#		export DVB_CA_1_FLAG=DVB_CA_TYPE_QZ_FLAG
		# 博远
		#export DVB_CA_TYPE=DVB_CA_TYPE_BY
		#export DVB_CA_1_NAME=by_cas
		#export DVB_CA_1_LIB=byca
		#export DVB_CA_1_FLAG=DVB_CA_TYPE_BY_FLAG
		#永新
		#	export DVB_CA_TYPE=DVB_CA_TYPE_CDCAS30
		#	export DVB_CA_1_NAME=cd_cas30
		#	export DVB_CA_1_LIB=Y1120-tonghui-gx3001-20121212D
		#	export DVB_CA_1_FLAG=DVB_CA_TYPE_CD_CAS30_FLAG
		#数视通
		#	export DVB_CA_TYPE=DVB_CA_TYPE_DIVITONE
		#	export DVB_CA_1_NAME=divitone_cas
		#	export DVB_CA_1_LIB=DiviGuardCalib-Keneng-GX3201-release-20140402
		#export DVB_CA_1_LIB=DiviGuardCalib-Maike-Anyang-GX3113C-release-20150520
		#export DVB_CA_1_LIB=DiviGuardCalib-Maike-Xupu-GX3113C-release-20150520
		#	export DVB_CA_1_FLAG=DVB_CA_TYPE_DIVITONE_FLAG
		#desai    
		#export DVB_CA_TYPE=DVB_CA_TYPE_DSCAS50
		#export DVB_CA_1_NAME=desai_cas50
		#export DVB_CA_1_LIB=ca_52
		#export DVB_CA_1_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG

		#滕锐CA(公版) env.sh配置参考：
		#export DVB_CA_TYPE=DVB_CA_TYPE_TR
		#   export DVB_CA_1_NAME=tr_cas
		#  export DVB_CA_1_LIB=TrMCell_450_Gx3113c
		#  export DVB_CA_1_FLAG=DVB_CA_TYPE_TR_FLAG
		#ledou
		#export DVB_CA_TYPE=DVB_CA_TYPE_DVB
		#export DVB_CA_1_NAME=dvb_cas
		#export DVB_CA_1_LIB=ca
		#export DVB_CA_1_FLAG=DVB_CA_TYPE_DVB_FLAG

	fi

	#兼容双CA。多数情况下只有一个CA，第二个CA不用设置
	if [ $DVB_CA_FLAG = "yes" ]; then
		#export DVB_CA_2_NAME=by_cas
		#export DVB_CA_2_LIB=byca
		#export DVB_CA_2_FLAG=DVB_CA_TYPE_BY_FLAG
		#   export DVB_CA_2_NAME=desai_cas50
		#   export DVB_CA_2_LIB=ca_52
		#   export DVB_CA_2_FLAG=DVB_CA_TYPE_DS_CAS50_FLAG

	fi

fi
# set ad support type
#export DVB_AD_NAME=dsads
#export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG
#export DVB_AD_LIB=dsad
#export DVB_AD_NAME=dsads41
#export DVB_AD_FLAG=DVB_AD_TYPE_DS_FLAG41
#export DVB_AD_LIB=dsad41
#export DVB_AD_NAME=divitoneads
#export DVB_AD_FLAG=DVB_AD_TYPE_DIVITONE_FLAG
#export DVB_AD_NAME=dvbads
#export DVB_AD_FLAG=DVB_AD_TYPE_DVB_FLAG
#set ota type
export DVB_OTA_TYPE=DVB_NATIONALCHIP_OTA

#set HD prog list yes,no
export DVB_HD_LIST=no

# set JPG LOGO SUPPORT yes, no
export DVB_JPG_LOGO=yes

# set audio dolby decode SUPPORT yes, no
export AUDIO_DOLBY=no

# set dvb pvr function support yes or no 
export DVB_PVR_FLAG=yes

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
export PANEL_CLK_GPIO=36
export PANEL_DATA_GPIO=35
export PANEL_STANDBY_GPIO=34
export PANEL_LOCK_GPIO=33

#选用遥控器宏定义类型 keymap_xxx(市场)_(厂家)，如KEY_GONGBAN_NATIONALCHIP_NEW
#已支持面板参考:key_xml目录下的.xml group="KEY_GONGBAN_NATIONALCHIP_NEW"等，注意同一个kex(x).xml不能出现遥控器串键的情况（串键遥控器必须不不同key(x).xml中定义）

# demod type 
export DVB_DEMOD_TYPE=DVB_DEMOD_ATBM886X

# demod mode 
export DVB_DEMOD_MODE=DVB_DEMOD_DTMB

# select ts output : 0,1,2
export DVB_TS_SRC=2

# tuner enum  demod/include/tuners.h
export DVB_TUNER_TYPE=62 #64 #tuner MXL608

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
export DVB_RESOLUTION=SD
#音视频同步方式 0-PCR_RECOVER 1-VPTS_RECOVER 2-APTS_RECOVER 3-AVPTS_RECOVER 4-NO_RECOVER
export DVB_SYS_MODE=0
# 全频视频X位置
export DVB_VIDEO_X=0
# 全频视频Y位置
export DVB_VIDEO_Y=0
# 全频视频宽大小
export DVB_VIDEO_W=720
# 全频视频高大小
export DVB_VIDEO_H=576
# 卫星最大个数（有线、地面方案基本设置为1）
export DVB_SAT_MAX=1
# TP频点最大个数
export DVB_TP_MAX=200
# 节目最大个数
export DVB_SERVICE_MAX=1000
# 搜索PAT超时时间（ms）
export DVB_PAT_TIMEOUT=3000
# 搜索SDT超时时间（ms）
export DVB_SDT_TIMEOUT=5000
# 搜索NIT超时时间（ms）
export DVB_NIT_TIMEOUT=10000
# 搜索PMT超时时间（ms）
export DVB_PMT_TIMEOUT=8000
# prog without lcn num, begin from DVB_LCN_DEFAULT
export DVB_LCN_DEFAULT=500
export DVB_CENTER_SYMRATE=6875
export DVB_CENTER_QAM=2

export DVB_AUTO_TEST_FLAG=no
# 视频缩放是否重新播放
export DVB_ZOOM_RESTART_PLAY=yes
#串口
export DVB_SERIAL_SUPPORT=no
#移动
export MOVE_FUNCTION_FLAG=no
export DVB_FLASH=8M
export LOGO_SHOW_DELAY=yes
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
	echo DVB_SERIAL_SUPPORT=$DVB_SERIAL_SUPPORT
	echo MOVE_FUNCTION_FLAG = $MOVE_FUNCTION_FLAG
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










