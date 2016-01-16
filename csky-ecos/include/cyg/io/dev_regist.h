#ifndef DEV_REGIST_H
#define DEV_REGIST_H
/*==========================================================================
name:        dev_regist.h
Author  :    gaohaibo
Date    :    2009-08-27
usage   :    #include <cyg/io/dev_regist.h>
==========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include <cyg/io/devtab.h>
#ifdef CYGPKG_NET
#include <cyg/io/eth/netdev.h>
#endif
#include <cyg/io/flash_dev.h>

#define DISK_INFO_MAX 10
#define MAX_MODULE_NUM 30	//ecosע������ģ����
#define MAX_FS_SUPPORT_NUM	8	//֧��ע��������ļ�ϵͳ��Ŀ
typedef struct disk_and_fs_info{
	 	int   disk_type;		//��������:usb�豸��sd��
		int   disk_status;		//���̵Ķ�������:in or out
		int   index;			//���������ţ���Ӧ��Ӧ�Ĺ���Ŀ¼:���غ�ж��Ӧ��ͬһ��������.
		int   total_partition_num;   //�����ܷ�����
	    char *disk_name;		//���̵��豸����			
		char *fs_type[24];      //�����ļ�ϵͳ����
unsigned char product_name[16];	//���̲�Ʒ����	
}disk_and_fs_info;

#define USBWIFI_INFO_MAX 10
typedef struct usbwifi_dev_info {
	int   dev_status;  // ��������:in or out
	char *dev_name;    // �豸����
	int   index;       // ������
} usbwifi_dev_info;


typedef struct system_info{
	cyg_uint32 platform_type;	//cpu type
	cyg_uint32 clock_cpu;
	cyg_uint32 clock_sdram;
	cyg_uint32 clock_DTO;
	cyg_uint32 clock_VIDEO;
	cyg_uint32 clock_APB;
	cyg_uint32 clock_CK_device;	//ckcore ����ʱ��
		char  *sys_module[MAX_MODULE_NUM]; //the pointer of all the registered devices' name
		char  *fs_module[MAX_FS_SUPPORT_NUM];	//all the registered file systems
}system_info;

typedef struct disk_dynamic_manage{
	volatile int disk_insert_index;
	volatile int disk_remove_index;
			 int disk_type;
	volatile int flag;
			char struct_full_flag;
			char partition_num;
			int  partition_type[24];//max partition num support
  unsigned char product_name[16];
}disk_dynamic_manage;

extern cyg_sem_t	disk_detect_sem;
extern disk_dynamic_manage disk_detect_info[DISK_INFO_MAX];

typedef struct usbwifi_dynamic_manage{
	volatile int insert_index;
	volatile int remove_index;
	int disk_type;
	volatile int flag;
	char struct_full_flag;
} usbwifi_dynamic_manage;

extern cyg_sem_t usbwifi_detect_sem;
extern usbwifi_dynamic_manage usbwifi_detect_info[USBWIFI_INFO_MAX];

Cyg_ErrNo char_dev_register(cyg_devtab_entry_t *ops, char *name, void *priv_dev);
Cyg_ErrNo block_dev_register(cyg_devtab_entry_t *ops, char *name, void *priv_dev);
#ifdef CYGPKG_NET
Cyg_ErrNo net_dev_register(cyg_netdevtab_entry_t *tab);
#endif
Cyg_ErrNo flash_dev_register(struct cyg_flash_dev *tab);
Cyg_ErrNo char_dev_unregister(char *name);
Cyg_ErrNo block_dev_unregister(char *name);
#ifdef CYGPKG_NET
Cyg_ErrNo net_dev_unregister(char *name);
#endif
cyg_int32 fs_type_get(disk_and_fs_info *disk_info);
Cyg_ErrNo disk_detect(disk_and_fs_info *disk_info);
Cyg_ErrNo usbwifi_detect(usbwifi_dev_info *usbwifi_info);
Cyg_ErrNo sys_info_get(system_info *info);
Cyg_ErrNo registered_mod_list(system_info *info);
int registed_mod_list_exist(char *fs_type);
#ifdef __cplusplus
}
#endif

#endif
