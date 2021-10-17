#ifndef PYSOURCE_3_10_0_SOCKETMODULE_C
#define PYSOURCE_3_10_0_SOCKETMODULE_C

#ifdef USE_BLUETOOTH
static int
setbdaddr(const char *name, bdaddr_t *bdaddr)
{
    unsigned int b0, b1, b2, b3, b4, b5;
    char ch;
    int n;

    n = sscanf(name, "%X:%X:%X:%X:%X:%X%c",
               &b5, &b4, &b3, &b2, &b1, &b0, &ch);
    if (n == 6 && (b0 | b1 | b2 | b3 | b4 | b5) < 256) {

#ifdef MS_WINDOWS
        *bdaddr = (ULONGLONG)(b0 & 0xFF);
        *bdaddr |= ((ULONGLONG)(b1 & 0xFF) << 8);
        *bdaddr |= ((ULONGLONG)(b2 & 0xFF) << 16);
        *bdaddr |= ((ULONGLONG)(b3 & 0xFF) << 24);
        *bdaddr |= ((ULONGLONG)(b4 & 0xFF) << 32);
        *bdaddr |= ((ULONGLONG)(b5 & 0xFF) << 40);
#else
        bdaddr->b[0] = b0;
        bdaddr->b[1] = b1;
        bdaddr->b[2] = b2;
        bdaddr->b[3] = b3;
        bdaddr->b[4] = b4;
        bdaddr->b[5] = b5;
#endif

        return 6;
    } else {
        PyErr_SetString(PyExc_OSError, "bad bluetooth address");
        return -1;
    }
}
#endif

#ifdef USE_BLUETOOTH
static PyObject *
makebdaddr(bdaddr_t *bdaddr)
{
    char buf[(6 * 2) + 5 + 1];

#ifdef MS_WINDOWS
    int i;
    unsigned int octets[6];

    for (i = 0; i < 6; ++i) {
        octets[i] = ((*bdaddr) >> (8 * i)) & 0xFF;
    }

    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
        octets[5], octets[4], octets[3],
        octets[2], octets[1], octets[0]);
#else
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
        bdaddr->b[5], bdaddr->b[4], bdaddr->b[3],
        bdaddr->b[2], bdaddr->b[1], bdaddr->b[0]);
#endif

    return PyUnicode_FromString(buf);
}
#endif

static PyObject *
makesockaddr(SOCKET_T sockfd, struct sockaddr *addr, size_t addrlen, int proto)
{
    if (addrlen == 0) {
        Py_RETURN_NONE;
    }

    switch (addr->sa_family) {

    case AF_INET:
    {
        const struct sockaddr_in *a = (const struct sockaddr_in *)addr;
        PyObject *addrobj = make_ipv4_addr(a);
        PyObject *ret = NULL;
        if (addrobj) {
            ret = Py_BuildValue("Oi", addrobj, ntohs(a->sin_port));
            Py_DECREF(addrobj);
        }
        return ret;
    }

#if defined(AF_UNIX)
    case AF_UNIX:
    {
        struct sockaddr_un *a = (struct sockaddr_un *) addr;
#ifdef __linux__
        size_t linuxaddrlen = addrlen - offsetof(struct sockaddr_un, sun_path);
        if (linuxaddrlen > 0 && a->sun_path[0] == 0) {  /* Linux abstract namespace */
            return PyBytes_FromStringAndSize(a->sun_path, linuxaddrlen);
        }
        else
#endif /* linux */
        {
            return PyUnicode_DecodeFSDefault(a->sun_path);
        }
    }
#endif /* AF_UNIX */

#if defined(AF_NETLINK)
       case AF_NETLINK:
       {
           struct sockaddr_nl *a = (struct sockaddr_nl *) addr;
           return Py_BuildValue("II", a->nl_pid, a->nl_groups);
       }
#endif /* AF_NETLINK */

#if defined(AF_QIPCRTR)
       case AF_QIPCRTR:
       {
           struct sockaddr_qrtr *a = (struct sockaddr_qrtr *) addr;
           return Py_BuildValue("II", a->sq_node, a->sq_port);
       }
#endif /* AF_QIPCRTR */

#if defined(AF_VSOCK)
       case AF_VSOCK:
       {
           struct sockaddr_vm *a = (struct sockaddr_vm *) addr;
           return Py_BuildValue("II", a->svm_cid, a->svm_port);
       }
#endif /* AF_VSOCK */

#ifdef ENABLE_IPV6
    case AF_INET6:
    {
        const struct sockaddr_in6 *a = (const struct sockaddr_in6 *)addr;
        PyObject *addrobj = make_ipv6_addr(a);
        PyObject *ret = NULL;
        if (addrobj) {
            ret = Py_BuildValue("OiII",
                                addrobj,
                                ntohs(a->sin6_port),
                                ntohl(a->sin6_flowinfo),
                                a->sin6_scope_id);
            Py_DECREF(addrobj);
        }
        return ret;
    }
#endif /* ENABLE_IPV6 */

#ifdef USE_BLUETOOTH
    case AF_BLUETOOTH:
        switch (proto) {

#ifdef BTPROTO_L2CAP
        case BTPROTO_L2CAP:
        {
            struct sockaddr_l2 *a = (struct sockaddr_l2 *) addr;
            PyObject *addrobj = makebdaddr(&_BT_L2_MEMB(a, bdaddr));
            PyObject *ret = NULL;
            if (addrobj) {
                ret = Py_BuildValue("Oi",
                                    addrobj,
                                    _BT_L2_MEMB(a, psm));
                Py_DECREF(addrobj);
            }
            return ret;
        }

#endif /* BTPROTO_L2CAP */

        case BTPROTO_RFCOMM:
        {
            struct sockaddr_rc *a = (struct sockaddr_rc *) addr;
            PyObject *addrobj = makebdaddr(&_BT_RC_MEMB(a, bdaddr));
            PyObject *ret = NULL;
            if (addrobj) {
                ret = Py_BuildValue("Oi",
                                    addrobj,
                                    _BT_RC_MEMB(a, channel));
                Py_DECREF(addrobj);
            }
            return ret;
        }

#ifdef BTPROTO_HCI
        case BTPROTO_HCI:
        {
            struct sockaddr_hci *a = (struct sockaddr_hci *) addr;
#if defined(__NetBSD__) || defined(__DragonFly__)
            return makebdaddr(&_BT_HCI_MEMB(a, bdaddr));
#else /* __NetBSD__ || __DragonFly__ */
            PyObject *ret = NULL;
            ret = Py_BuildValue("i", _BT_HCI_MEMB(a, dev));
            return ret;
#endif /* !(__NetBSD__ || __DragonFly__) */
        }

#if !defined(__FreeBSD__)
        case BTPROTO_SCO:
        {
            struct sockaddr_sco *a = (struct sockaddr_sco *) addr;
            return makebdaddr(&_BT_SCO_MEMB(a, bdaddr));
        }
#endif /* !__FreeBSD__ */
#endif /* BTPROTO_HCI */

        default:
            PyErr_SetString(PyExc_ValueError,
                            "Unknown Bluetooth protocol");
            return NULL;
        }
#endif /* USE_BLUETOOTH */

#if defined(HAVE_NETPACKET_PACKET_H) && defined(SIOCGIFNAME)
    case AF_PACKET:
    {
        struct sockaddr_ll *a = (struct sockaddr_ll *)addr;
        const char *ifname = "";
        struct ifreq ifr;
        if (a->sll_ifindex) {
            ifr.ifr_ifindex = a->sll_ifindex;
            if (ioctl(sockfd, SIOCGIFNAME, &ifr) == 0)
                ifname = ifr.ifr_name;
        }
        return Py_BuildValue("shbhy#",
                             ifname,
                             ntohs(a->sll_protocol),
                             a->sll_pkttype,
                             a->sll_hatype,
                             a->sll_addr,
                             (Py_ssize_t)a->sll_halen);
    }
#endif /* HAVE_NETPACKET_PACKET_H && SIOCGIFNAME */

#ifdef HAVE_LINUX_TIPC_H
    case AF_TIPC:
    {
        struct sockaddr_tipc *a = (struct sockaddr_tipc *) addr;
        if (a->addrtype == TIPC_ADDR_NAMESEQ) {
            return Py_BuildValue("IIIII",
                            a->addrtype,
                            a->addr.nameseq.type,
                            a->addr.nameseq.lower,
                            a->addr.nameseq.upper,
                            a->scope);
        } else if (a->addrtype == TIPC_ADDR_NAME) {
            return Py_BuildValue("IIIII",
                            a->addrtype,
                            a->addr.name.name.type,
                            a->addr.name.name.instance,
                            a->addr.name.name.instance,
                            a->scope);
        } else if (a->addrtype == TIPC_ADDR_ID) {
            return Py_BuildValue("IIIII",
                            a->addrtype,
                            a->addr.id.node,
                            a->addr.id.ref,
                            0,
                            a->scope);
        } else {
            PyErr_SetString(PyExc_ValueError,
                            "Invalid address type");
            return NULL;
        }
    }
#endif /* HAVE_LINUX_TIPC_H */

#if defined(AF_CAN) && defined(SIOCGIFNAME)
    case AF_CAN:
    {
        struct sockaddr_can *a = (struct sockaddr_can *)addr;
        const char *ifname = "";
        struct ifreq ifr;
        if (a->can_ifindex) {
            ifr.ifr_ifindex = a->can_ifindex;
            if (ioctl(sockfd, SIOCGIFNAME, &ifr) == 0)
                ifname = ifr.ifr_name;
        }

        switch (proto) {
#ifdef CAN_ISOTP
          case CAN_ISOTP:
          {
              return Py_BuildValue("O&kk", PyUnicode_DecodeFSDefault,
                                          ifname,
                                          a->can_addr.tp.rx_id,
                                          a->can_addr.tp.tx_id);
          }
#endif /* CAN_ISOTP */
#ifdef CAN_J1939
          case CAN_J1939:
          {
              return Py_BuildValue("O&KkB", PyUnicode_DecodeFSDefault,
                                          ifname,
                                          a->can_addr.j1939.name,
                                          a->can_addr.j1939.pgn,
                                          a->can_addr.j1939.addr);
          }
#endif /* CAN_J1939 */
          default:
          {
              return Py_BuildValue("(O&)", PyUnicode_DecodeFSDefault,
                                        ifname);
          }
        }
    }
#endif /* AF_CAN && SIOCGIFNAME */

#ifdef PF_SYSTEM
    case PF_SYSTEM:
        switch(proto) {
#ifdef SYSPROTO_CONTROL
        case SYSPROTO_CONTROL:
        {
            struct sockaddr_ctl *a = (struct sockaddr_ctl *)addr;
            return Py_BuildValue("(II)", a->sc_id, a->sc_unit);
        }
#endif /* SYSPROTO_CONTROL */
        default:
            PyErr_SetString(PyExc_ValueError,
                            "Invalid address type");
            return 0;
        }
#endif /* PF_SYSTEM */

#ifdef HAVE_SOCKADDR_ALG
    case AF_ALG:
    {
        struct sockaddr_alg *a = (struct sockaddr_alg *)addr;
        return Py_BuildValue("s#s#HH",
            a->salg_type,
            strnlen((const char*)a->salg_type,
                    sizeof(a->salg_type)),
            a->salg_name,
            strnlen((const char*)a->salg_name,
                    sizeof(a->salg_name)),
            a->salg_feat,
            a->salg_mask);
    }
#endif /* HAVE_SOCKADDR_ALG */

    default:
        return Py_BuildValue("iy#",
                             addr->sa_family,
                             addr->sa_data,
                             sizeof(addr->sa_data));

    }
}

static PyObject*
sock_decode_hostname(const char *name)
{
#ifdef MS_WINDOWS
    return PyUnicode_DecodeMBCS(name, strlen(name), "surrogatepass");
#else
    return PyUnicode_FromString(name);
#endif
}

#endif