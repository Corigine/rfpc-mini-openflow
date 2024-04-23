#include<stdint.h>
/* This is the aligned version of ip_addr_t,
   used as local variable, on the stack, etc. */

#define IP_HLEN 20

#define IP_PROTO_ICMP    1
#define IP_PROTO_IGMP    2
#define IP_PROTO_UDP     17
#define IP_PROTO_UDPLITE 136
#define IP_PROTO_TCP     6



#define IPH_V(hdr)  ((hdr)->_v_hl >> 4)
#define IPH_HL(hdr) ((hdr)->_v_hl & 0x0f)
#define IPH_TOS(hdr) ((hdr)->_tos)
#define IPH_LEN(hdr) ((hdr)->_len)
#define IPH_ID(hdr) ((hdr)->_id)
#define IPH_OFFSET(hdr) ((hdr)->_offset)
#define IPH_TTL(hdr) ((hdr)->_ttl)
#define IPH_PROTO(hdr) ((hdr)->_proto)
#define IPH_CHKSUM(hdr) ((hdr)->_chksum)

#define IPH_VHL_SET(hdr, v, hl) (hdr)->_v_hl = (((v) << 4) | (hl))
#define IPH_TOS_SET(hdr, tos) (hdr)->_tos = (tos)
#define IPH_LEN_SET(hdr, len) (hdr)->_len = (len)
#define IPH_ID_SET(hdr, id) (hdr)->_id = (id)
#define IPH_OFFSET_SET(hdr, off) (hdr)->_offset = (off)
#define IPH_TTL_SET(hdr, ttl) (hdr)->_ttl = (u8_t)(ttl)
#define IPH_PROTO_SET(hdr, proto) (hdr)->_proto = (u8_t)(proto)
#define IPH_CHKSUM_SET(hdr, chksum) (hdr)->_chksum = (chksum)


struct ip_addr {
  uint32_t addr;
};
typedef struct ip_addr ip_addr_t;
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT __attribute__ ((packed))

struct ip_hdr {
  /* version / header length */
  PACK_STRUCT_FIELD(uint8_t _v_hl);
  /* type of service */
  PACK_STRUCT_FIELD(uint8_t _tos);
  /* total length */
  PACK_STRUCT_FIELD(uint16_t _len);
  /* identification */
  PACK_STRUCT_FIELD(uint16_t _id);
  /* fragment offset field */
  PACK_STRUCT_FIELD(uint16_t _offset);
#define IP_RF 0x8000U        /* reserved fragment flag */
#define IP_DF 0x4000U        /* dont fragment flag */
#define IP_MF 0x2000U        /* more fragments flag */
#define IP_OFFMASK 0x1fffU   /* mask for fragmenting bits */
  /* time to live */
  PACK_STRUCT_FIELD(uint8_t _ttl);
  /* protocol*/
  PACK_STRUCT_FIELD(uint8_t _proto);
  /* checksum */
  PACK_STRUCT_FIELD(uint16_t _chksum);
  /* source and destination IP addresses */
  PACK_STRUCT_FIELD(ip_addr_t src);
  PACK_STRUCT_FIELD(ip_addr_t dest);
} PACK_STRUCT_STRUCT;

struct tcp_hdr {
  PACK_STRUCT_FIELD(uint16_t src);
  PACK_STRUCT_FIELD(uint16_t dest);
  PACK_STRUCT_FIELD(uint32_t seqno);
  PACK_STRUCT_FIELD(uint32_t ackno);
  PACK_STRUCT_FIELD(uint16_t _hdrlen_rsvd_flags);
  PACK_STRUCT_FIELD(uint16_t wnd);
  PACK_STRUCT_FIELD(uint16_t chksum);
  PACK_STRUCT_FIELD(uint16_t urgp);
} PACK_STRUCT_STRUCT;

struct udp_hdr {
  PACK_STRUCT_FIELD(uint16_t src);
  PACK_STRUCT_FIELD(uint16_t dest);  /* src/dest UDP ports */
  PACK_STRUCT_FIELD(uint16_t len);
  PACK_STRUCT_FIELD(uint16_t chksum);
} PACK_STRUCT_STRUCT;
