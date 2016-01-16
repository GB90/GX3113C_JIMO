#ifndef _KP_CAS_H_
#define _KP_CAS_H_

#define KPCA_FLASH_SIZE                 (32*1024)
#define FLASH_SYNC_TIME_SEC             (10)


//#define KP_DEBUG    //≤‚ ‘ ± π”√

#ifdef KP_DEBUG
#ifndef KP_CA_FLAG
#define KP_CA_FLAG    1
#endif
#endif

#ifdef  KP_CA_FLAG
#define KP_Dbg(...)     \
                                do { \
                                    printf("[KP_cas]\t");  \
                                    printf(__VA_ARGS__);   \
                                } while(0)


#define KP_DUMP(str, ptr, size)                                                                    \
                        do {                                                                        \
                            int i;                                                                  \
                                printf("\t\%s  len=%d\n\t", (str), (size));     \
                            if (size != 0) {                                                        \
                                for (i = 0; i < (size); i++) {                                      \
                                    printf("0x%02x,", (ptr)[i]);                                      \
                                }                                                                   \
                                printf("\n\t\n");                               \
                            }                                                                       \
                        } while (0)

#else
#define KP_Dbg(...)    do {      \
                       } while(0)
#define KP_DUMP(str, ptr, size)     \
                    do {      \
                    } while (0)
#endif

typedef enum
{
	CA_KPCA_MESSAGE_SUCCESS = 0,
    CA_KPCA_MESSAGE_NO_ACTIVE,
    CA_KPCA_MESSAGE_AREA_NOT_MATCH,
    CA_KPCA_MESSAGE_AREA_FORCE,
    CA_KPCA_MESSAGE_ERROR_PRODUCT,
    CA_KPCA_MESSAGE_ERROR_PROVIDER,
    CA_KPCA_MESSAGE_NO_ENTITLE,
    CA_KPCA_MESSAGE_ENTITLE_EXPIRED,
    CA_KPCA_MESSAGE_ENTITLE_NOSTART,
    CA_KPCA_MESSAGE_PRODUCT_CLASH,
    CA_KPCA_MESSAGE_SERVICE_FORCE,
    CA_KPCA_MESSAGE_OUT_RATING,
    CA_KPCA_MESSAGE_OUT_WORKTIME,
    CA_KPCA_MESSAGE_FREEZE,
}KP_CA_STATUS;

typedef struct
{
    U32 datelen;
    U8 *pdata;
}SECTION_DATA;


void filter_list_init();
int filter_list_search(U16 pid,U16 tableid,U32 *pqueueid);
int filter_list_add(U32 filterid,U16 pid,U16 tableid,U32 queueid);
void filter_list_del(U16 pid,U16 tableid);
int filter_list_get(U32 filterid,U16 *ppid,U16 *ptableid);


void flash_init();
void flash_write2flash();

U16 flash_write2buf(U16 offsetAddr, U8 *data, U16 len);
U16  flash_readFrombuf(U16 offsetAddr, U8 *data, U16 len);

#endif


