/*
 * NETBIOS protocol formats
 *
 * @(#) $Header: /cvsroot/uclinux533/uClinux-dist/user/tcpdump/netbios.h,v 1.2 2005/07/25 06:38:35 magicyang Exp $
 */

struct p8022Hdr {
    u_char	dsap;
    u_char	ssap;
    u_char	flags;
};

#define	p8022Size	3		/* min 802.2 header size */

#define UI		0x03		/* 802.2 flags */

