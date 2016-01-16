#ifndef __SYS_MENU_CONFIGURE_H__
#define __SYS_MENU_CONFIGURE_H__

/*
* 非common目录参数定义，仅针对不同方案使用
*/

#ifdef APP_SD

#define POP_DIALOG_X_START          210
#define POP_DIALOG_Y_START          200

#endif/*APP_SD*/

#ifdef APP_HD

#define POP_DIALOG_X_START          340
#define POP_DIALOG_Y_START          220

#endif /* APP_HD */
//#define TDAC7              1
//#define TDAC2              2
//#define SHARP6093          3
//#define CDT                4

/*
* 方案使用的TUNER
*/
//#define TUNER_TYPE           TDAC2

//#define GX1501             1
//#define ATBM8848           2

/*
* 方案使用的解调
*/
//#define DEMOD_TYPE           GX1501

#endif
