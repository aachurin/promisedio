#ifndef PYSOURCE_3_10_0_SOCKETMODULE_H
#define PYSOURCE_3_10_0_SOCKETMODULE_H


#ifndef MS_WINDOWS
#ifdef __VMS
#   include <socket.h>
# else
#   include <sys/socket.h>
# endif
# include <netinet/in.h>
# if !defined(__CYGWIN__)
#  include <netinet/tcp.h>
# endif

#else /* MS_WINDOWS */
# include <winsock2.h>

#ifdef AF_BTH
# include <ws2bth.h>
# include <pshpack1.h>

typedef ULONGLONG bdaddr_t;

struct SOCKADDR_BTH_REDEF {
    union {
        USHORT    addressFamily;
        USHORT    family;
    };

    union {
        ULONGLONG btAddr;
        bdaddr_t  bdaddr;
    };

    GUID      serviceClassId;

    union {
        ULONG     port;
        ULONG     channel;
    };

};
# include <poppack.h>
#endif

# ifdef PySocket_BUILDING_SOCKET
#  undef CMSG_LEN
# endif
# include <ws2tcpip.h>
# ifdef SIO_GET_MULTICAST_FILTER
#  include <mstcpip.h> /* for SIO_RCVALL */
#  define HAVE_ADDRINFO
#  define HAVE_SOCKADDR_STORAGE
#  define HAVE_GETADDRINFO
#  define HAVE_GETNAMEINFO
#  define ENABLE_IPV6
# else
typedef int socklen_t;
# endif /* IPPROTO_IPV6 */
#endif /* MS_WINDOWS */

#ifdef HAVE_SYS_UN_H
# include <sys/un.h>
#else
# undef AF_UNIX
#endif

#ifdef HAVE_LINUX_NETLINK_H
# ifdef HAVE_ASM_TYPES_H
#  include <asm/types.h>
# endif
# include <linux/netlink.h>
#else
#  undef AF_NETLINK
#endif

#ifdef HAVE_LINUX_QRTR_H
# ifdef HAVE_ASM_TYPES_H
#  include <asm/types.h>
# endif
# include <linux/qrtr.h>
#else
#  undef AF_QIPCRTR
#endif

#ifdef HAVE_BLUETOOTH_BLUETOOTH_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sco.h>
#include <bluetooth/hci.h>
#endif

#ifdef HAVE_BLUETOOTH_H
#include <bluetooth.h>
#endif

#ifdef HAVE_NET_IF_H
# include <net/if.h>
#endif

#ifdef HAVE_NETPACKET_PACKET_H
# include <sys/ioctl.h>
# include <netpacket/packet.h>
#endif

#ifdef HAVE_LINUX_TIPC_H
# include <linux/tipc.h>
#endif

#ifdef HAVE_LINUX_CAN_H
# include <linux/can.h>
#else
# undef AF_CAN
# undef PF_CAN
#endif

#ifdef HAVE_LINUX_CAN_RAW_H
#include <linux/can/raw.h>
#endif

#ifdef HAVE_LINUX_CAN_BCM_H
#include <linux/can/bcm.h>
#endif

#ifdef HAVE_LINUX_CAN_J1939_H
#include <linux/can/j1939.h>
#endif

#ifdef HAVE_SYS_SYS_DOMAIN_H
#include <sys/sys_domain.h>
#endif
#ifdef HAVE_SYS_KERN_CONTROL_H
#include <sys/kern_control.h>
#endif

#ifdef HAVE_LINUX_VM_SOCKETS_H
# include <linux/vm_sockets.h>
#else
# undef AF_VSOCK
#endif

#ifdef HAVE_SOCKADDR_ALG

# include <linux/if_alg.h>
# ifndef AF_ALG
#  define AF_ALG 38
# endif
# ifndef SOL_ALG
#  define SOL_ALG 279
# endif

# ifndef ALG_SET_AEAD_ASSOCLEN
#  define ALG_SET_AEAD_ASSOCLEN           4
# endif
# ifndef ALG_SET_AEAD_AUTHSIZE
#  define ALG_SET_AEAD_AUTHSIZE           5
# endif
# ifndef ALG_SET_PUBKEY
#  define ALG_SET_PUBKEY                  6
# endif

# ifndef ALG_OP_SIGN
#  define ALG_OP_SIGN                     2
# endif
# ifndef ALG_OP_VERIFY
#  define ALG_OP_VERIFY                   3
# endif

#endif /* HAVE_SOCKADDR_ALG */

#ifndef Py__SOCKET_H
#define Py__SOCKET_H
#ifdef __cplusplus
extern "C" {
#endif

#define PySocket_MODULE_NAME    "_socket"
#define PySocket_CAPI_NAME      "CAPI"
#define PySocket_CAPSULE_NAME   PySocket_MODULE_NAME "." PySocket_CAPI_NAME

#ifdef MS_WINDOWS
typedef SOCKET SOCKET_T;
#       ifdef MS_WIN64
#               define SIZEOF_SOCKET_T 8
#       else
#               define SIZEOF_SOCKET_T 4
#       endif
#else
typedef int SOCKET_T;
#       define SIZEOF_SOCKET_T SIZEOF_INT
#endif

#if SIZEOF_SOCKET_T <= SIZEOF_LONG
#define PyLong_FromSocket_t(fd) PyLong_FromLong((SOCKET_T)(fd))
#define PyLong_AsSocket_t(fd) (SOCKET_T)PyLong_AsLong(fd)
#else
#define PyLong_FromSocket_t(fd) PyLong_FromLongLong((SOCKET_T)(fd))
#define PyLong_AsSocket_t(fd) (SOCKET_T)PyLong_AsLongLong(fd)
#endif

#define PySocketModule_ImportModuleAndAPI() PyCapsule_Import(PySocket_CAPSULE_NAME, 1)

#ifdef __cplusplus
}
#endif
#endif /* !Py__SOCKET_H */

#endif