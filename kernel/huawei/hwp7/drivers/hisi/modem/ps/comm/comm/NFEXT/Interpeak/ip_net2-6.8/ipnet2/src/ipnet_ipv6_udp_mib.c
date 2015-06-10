/*
 ******************************************************************************
 *                     INTERPEAK SOURCE FILE
 *
 *   Document no: @(#) $Name: VXWORKS_ITER18A_FRZ10 $ $RCSfile: ipnet_ipv6_udp_mib.c,v $ $Revision: 1.17 $
 *   $Source: /home/interpeak/CVSRoot/ipnet2/src/ipnet_ipv6_udp_mib.c,v $
 *   $State: Exp $ $Locker:  $
 *
 *   INTERPEAK_COPYRIGHT_STRING
 *   Design and implementation by Markus Carlstedt <markus@interpeak.se>
 ******************************************************************************
 */

/*
 ****************************************************************************
 * 1                    DESCRIPTION
 ****************************************************************************
 */

/*
 *===========================================================================
 *                      IPV6-UDP-MIB
 *===========================================================================
 *
 * Autogenerated by 'ipmibc' with 'smidump 0.4.1' on 2003-11-21 at 13:57:50
 *
 * This file includes initialization code and stubs for MIB variables defined
 * in IPV6-UDP-MIB. Without any changes it should compile and run with the
 * behaviour that GetRequests succeed with the returned value being
 * zero. SetRequests also succeed but without any actual variables
 * being updated.
 *
 * Modifications to this file are necessary in order to enable read and
 * write operations on actual variables presented in the MIB interface.
 * Each segment in the code where a change is proposed is marked with a
 * 'MIB_TODO' comment together with a pseudo code example describing the
 * modification.
 *
 * This file shall normally be compiled together with the product to which
 * SNMP support is to be added and then linked with the IPSNMP libraries.
 * The MIB is initialized by calling the public initialization function defined
 * at the end of this file. Since no header files are generated its function
 * prototype should be copied into the the product that initializes the MIB.
 *
 */

/*
 ****************************************************************************
 * 2                    CONFIGURATION
 ****************************************************************************
 */

#ifndef IPCOM_KERNEL
#define IPCOM_KERNEL
#endif
#include "ipnet_config.h"


/*
 ****************************************************************************
 * 3                    INCLUDE FILES
 ****************************************************************************
 */

#define IPCOM_USE_CLIB_PROTO
#include <ipcom_type.h>
#include <ipcom_cstyle.h>
#include <ipcom_clib.h>
#include <ipcom_syslog.h>
#include <ipcom_err.h>

#ifdef IPSNMP
#include <ipsnmp_config.h>
#include <ipsnmp.h>
#include <ipnet_h.h>

#if defined(IPCOM_USE_MIB2) && defined(IPCOM_USE_INET6)


/*
 ****************************************************************************
 * 4                    DEFINES
 ****************************************************************************
 */

#ifdef IPCOM_SYSLOG_FACILITY
#undef IPCOM_SYSLOG_FACILITY
#endif
#define IPCOM_SYSLOG_FACILITY    IPCOM_LOG_IPSNMP

#ifdef IPCOM_SYSLOG_PRIORITY
#undef IPCOM_SYSLOG_PRIORITY
#endif
#define IPCOM_SYSLOG_PRIORITY    IPCOM_LOG_DEBUG


/*
 ****************************************************************************
 * 5                    TYPES
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 6                    EXTERNAL PROTOTYPES
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 7                    LOCAL PROTOTYPES
 ****************************************************************************
 */

IP_PUBLIC Ip_err ipnet_ipv6_udp_mib_init(void);


/*
 ****************************************************************************
 * 8                    DATA
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 9                    STATIC FUNCTIONS
 ****************************************************************************
 */

/*
 *===========================================================================
 *                      ipnet_ipv6_udp_mib_lock
 *===========================================================================
 * Description: Protects MIB variables in SNMPv2-MIB from multiple access
 * Parameters: lock - boolean that controls lock or unlock
 * Returns: Nothing
 *
 */
IP_STATIC void
ipnet_ipv6_udp_mib_lock(Ip_bool lock)
{
    if(lock == IP_TRUE)
    {
        IPNET_CODE_LOCK_WRITE();
    }
    else
    {
        IPNET_CODE_UNLOCK();
    }
}


/*
 *===========================================================================
 *                      ipnet_ipv6_udp_mib_table_search_ipv6UdpTable
 *===========================================================================
 * Description: Searches ipv6UdpTable for a matching entry
 * Parameters: id, buf, best, cmd
 * Returns: Row index of the found entry OR -1 if not found OR -2 for error
 *
 */
IP_STATIC Ip_s32
ipnet_ipv6_udp_mib_table_search_ipv6UdpTable(char *id,
                                             char *buf,
                                             char *best,
                                             Ip_s32 cmd)
{
    Ip_u8 *ipv6UdpLocalAddress;
    Ip_s32 ipv6UdpLocalAddress_len;
    Ip_s32 ipv6UdpLocalPort;
    Ip_s32 ipv6UdpIfIndex;
    Ip_s32 ipv6UdpTable_num_entries;
    Ip_s32 i, bestindex = -1;
    Ipnet_netif *netif;

    ipv6UdpTable_num_entries = ipnet_conf_max_sockets;

    for(i=0; i<ipv6UdpTable_num_entries; i++)
    {
        Ip_s32 j, lex, len = 0;

        buf[0] = '\0';

        if (ipnet->sockets[i] == IP_NULL || ipnet->sockets[i]->ipcom.domain != IP_AF_INET6
            || ipnet->sockets[i]->ipcom.type != IP_SOCK_DGRAM || ipnet->sockets[i]->proto != IP_IPPROTO_UDP
            || !IP_BIT_ISSET(ipnet->sockets[i]->flags, IPNET_SOCKET_FLAG_BOUND))
        {
            continue;
        }

        ipv6UdpLocalAddress = ipnet->sockets[i]->ip6->saddr.in6.addr8;
        ipv6UdpLocalAddress_len = 16;

        for(j=0; j<ipv6UdpLocalAddress_len; j++)
        {
            if(ipcom_snprintf(&buf[len], IPSNMP_CONFIG_MAX_OBJECT_ID-len, "%d.", ipv6UdpLocalAddress[j]) < 0)
                return -2;
            len = ipcom_strlen(buf);
        }
        len = ipcom_strlen(buf);

        ipv6UdpLocalPort = ipnet->sockets[i]->sport;

        if(ipcom_snprintf(&buf[len], IPSNMP_CONFIG_MAX_OBJECT_ID-len, "%d.", (int)ipv6UdpLocalPort) < 0)
            return -2;
        len = ipcom_strlen(buf);

        netif = ipnet_ip6_socket_to_netif(ipnet->sockets[i]);
        ipv6UdpIfIndex = netif == IP_NULL ? 0 : netif->ipcom.ifindex;

        if(ipcom_snprintf(&buf[len], IPSNMP_CONFIG_MAX_OBJECT_ID-len, "%d.", (int)ipv6UdpIfIndex) < 0)
            return -2;
        len = ipcom_strlen(buf);
        if(len)
            buf[len-1] = '\0';

        lex = ipsnmp_util_lexcmp_oid(buf, id);
        if(cmd == IPSNMP_MIB_COMMAND_NEXT)
        {
            if(lex > 0)
            {
                if(bestindex == -1 || ipsnmp_util_lexcmp_oid(buf, best) < 0)
                {
                    ipcom_strcpy(best, buf);
                    bestindex = i;
                }
            }
        }
        else
        {
            if(lex == 0)
            {
                ipcom_strcpy(best, buf);
                bestindex = i;
                break;
            }
        }
    }

    return bestindex;
}


/*
 *===========================================================================
 *                      ipnet_ipv6_udp_mib_handler_ipv6UdpIfIndex
 *===========================================================================
 * Description: MIB handler for ipv6UdpIfIndex
 * Parameters: See file 'ipsnmp.h'
 * Returns: IPSNMP_ERROR_XXX
 *
 */
IP_STATIC Ip_s32
ipnet_ipv6_udp_mib_handler_ipv6UdpIfIndex(Ip_s32 cmd,
                                          char *id,
                                          Ipsnmp_varbind *vb,
                                          Ip_s32 magic,
                                          struct Ipsnmp_node_object *nodeobj)
{
    Ip_s32 bestindex, ret = -1;
    Ip_s32 ipv6UdpIfIndex;
    char *iid;
    char *buf = ipcom_malloc(IPSNMP_CONFIG_MAX_OBJECT_ID);
    char *best = ipcom_malloc(IPSNMP_CONFIG_MAX_OBJECT_ID);
    Ipnet_netif *netif;

    (void)vb;
    if(buf == IP_NULL || best == IP_NULL)
    {
        ret = IPSNMP_ERROR_GENERROR;
        goto exit;
    }

    bestindex = ipnet_ipv6_udp_mib_table_search_ipv6UdpTable(id, buf, best, cmd);
    if(bestindex == -2)
    {
        ret = IPSNMP_ERROR_GENERROR;
        goto exit;
    }
    else if(bestindex == -1)
    {
        ret = IPSNMP_ERROR_NOSUCHNAME;
        goto exit;
    }

    if(cmd == IPSNMP_MIB_COMMAND_GET || cmd == IPSNMP_MIB_COMMAND_NEXT)
    {
        iid = ipsnmp_create_iid_direct(nodeobj->id, best);
        if(iid == IP_NULL)
        {
            ret = IPSNMP_ERROR_GENERROR;
            goto exit;
        }

        netif = ipnet_ip6_socket_to_netif(ipnet->sockets[bestindex]);
        ipv6UdpIfIndex = netif == IP_NULL ? 0 : netif->ipcom.ifindex;

        ret = ipsnmp_util_put_integer(magic, iid, ipv6UdpIfIndex);
        ipcom_free(iid);
    }

exit:
    if(buf != IP_NULL)
        ipcom_free(buf);
    if(best != IP_NULL)
        ipcom_free(best);
    return ret;
}


/*
 ****************************************************************************
 * 10                    GLOBAL FUNCTIONS
 ****************************************************************************
 */

/*
 ****************************************************************************
 * 11                    PUBLIC FUNCTIONS
 ****************************************************************************
 */

/*
 *===========================================================================
 *                      ipnet_ipv6_udp_mib_init
 *===========================================================================
 * Description: Initializes IPV6-UDP-MIB
 * Parameters: None
 * Returns: IPCOM_SUCCESS or IPCOM_ERR_FAILED
 *
 */
IP_PUBLIC Ip_err
ipnet_ipv6_udp_mib_init(void)
{
    struct Ipsnmp_node_object nodeobj;
    Ip_s32 ret;

    /* Assign lock function */
    nodeobj.lock = ipnet_ipv6_udp_mib_lock;

    /* Add node "ipv6UdpMIB" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_UNSPECIFIED;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.3.87";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpMIB";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_OTHER;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

#ifndef IPCOM_USE_INET
    /* Add node "udp" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_UNSPECIFIED;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.2.1.7";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "udp";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_OTHER;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }
#endif /* #ifndef IPCOM_USE_INET */

    /* Add node "ipv6UdpTable" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_UNSPECIFIED;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.2.1.7.6";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpTable";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_AGGREGATE;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

    /* Add node "ipv6UdpEntry" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_UNSPECIFIED;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.2.1.7.6.1";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpEntry";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_AGGREGATE;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

    /* Add node "ipv6UdpLocalAddress" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_OCTETSTRING;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.2.1.7.6.1.1";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpLocalAddress";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_COLUMNAR;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

    /* Add node "ipv6UdpLocalPort" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_NOACCESS;
    nodeobj.datatype = IPSNMP_DATA_TYPE_INTEGER;
    nodeobj.handler = IP_NULL;
    nodeobj.id = "1.3.6.1.2.1.7.6.1.2";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpLocalPort";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_COLUMNAR;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

    /* Add node "ipv6UdpIfIndex" to the MIB tree */
    nodeobj.access = IPSNMP_NODE_ACCESS_READONLY;
    nodeobj.datatype = IPSNMP_DATA_TYPE_INTEGER;
    nodeobj.handler = ipnet_ipv6_udp_mib_handler_ipv6UdpIfIndex;
    nodeobj.id = "1.3.6.1.2.1.7.6.1.3";
    nodeobj.instance = IP_NULL;
#ifdef IP_DEBUG
    nodeobj.name = "ipv6UdpIfIndex";
#endif
    nodeobj.nodetype = IPSNMP_NODE_TYPE_COLUMNAR;
    ret = ipsnmp_add_node(&nodeobj);
    if(ret != 0)
    {
        IPCOM_LOG2(ERR, "ipsnmp_add_node() :: failed to add %s (%d)", nodeobj.id, ret);
        return IPCOM_ERR_FAILED;
    }

    return IPCOM_SUCCESS;
}

#endif /* #if defined(IPCOM_USE_MIB2) && defined(IPCOM_USE_INET6) */
#endif /* #ifdef IPSNMP */

#if !defined(IPSNMP) || !defined(IPCOM_USE_MIB2)
void ipnet_ipv6_udp_mib_empty_file(void);
void ipnet_ipv6_udp_mib_empty_file(void) { }
#endif


/*
 ****************************************************************************
 *                     END OF FILE
 ****************************************************************************
 */