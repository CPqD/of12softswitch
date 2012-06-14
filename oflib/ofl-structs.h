/* Copyright (c) 2011, TrafficLab, Ericsson Research, Hungary
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the Ericsson Research nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * Author: Zoltán Lajos Kis <zoltan.lajos.kis@ericsson.com>
 */

#ifndef OFL_STRUCTS_H
#define OFL_STRUCTS_H 1

#include <sys/types.h>
#include <stdio.h>

#include <netinet/icmp6.h>
#include "../include/openflow/openflow.h"
#include "ofl.h"
#include "ofl-actions.h"
#include "ofl-packets.h"
#include "../lib/hmap.h"


struct ofl_exp;

/****************************************************************************
 * Supplementary structure definitions.
 ****************************************************************************/
 
struct ofl_packet_queue {
    uint32_t   queue_id; /* id for the specific queue. */
    size_t                         properties_num;
    struct ofl_queue_prop_header **properties;
};


struct ofl_queue_prop_header {
    enum ofp_queue_properties   type; /* One of OFPQT_. */
};

struct ofl_queue_prop_min_rate {
    struct ofl_queue_prop_header   header; /* OFPQT_MIN_RATE */

    uint16_t   rate; /* In 1/10 of a percent; >1000 -> disabled. */
};

struct ofl_queue_prop_max_rate {
    struct ofl_queue_prop_header   header; /* OFPQT_MAX_RATE */

    uint16_t   rate; /* In 1/10 of a percent; >1000 -> disabled. */
};

struct ofl_queue_prop_experimenter {
    struct ofl_queue_prop_header prop_header; /* prop: OFPQT_EXPERIMENTER, len: 16. */
    uint32_t experimenter;
    uint8_t *data; /* Experimenter defined data. */
};


struct ofl_instruction_header {
    enum ofp_instruction_type   type; /* Instruction type */
};

struct ofl_instruction_goto_table {
    struct ofl_instruction_header   header; /* OFPIT_GOTO_TABLE */

    uint8_t   table_id; /* Set next table in the lookup pipeline */
};



struct ofl_instruction_write_metadata {
    struct ofl_instruction_header   header; /* OFPIT_WRITE_METADATA */
    
    uint64_t   metadata;      /* Metadata value to write */
    uint64_t   metadata_mask; /* Metadata write bitmask */
};


struct ofl_instruction_actions {
    struct ofl_instruction_header   header; /* OFPIT_WRITE|APPLY_ACTIONS */

    size_t                     actions_num;
    struct ofl_action_header **actions;
};

/* Instruction structure for experimental instructions */
struct ofl_instruction_experimenter {
    struct ofl_instruction_header   header; /* OFPIT_EXPERIMENTER */

    uint32_t   experimenter_id; /* Experimenter ID */
};

struct ofl_config {
    uint16_t   flags;         /* OFPC_* flags. */
    uint16_t   miss_send_len; /* Max bytes of new flow that datapath should
                                send to the controller. */
};

struct ofl_bucket {
    uint16_t   weight;      /* Relative weight of bucket. Only
                              defined for select groups. */
    uint32_t   watch_port;  /* Port whose state affects whether this
                              bucket is live. Only required for fast
                              failover groups. */
    uint32_t   watch_group; /* Group whose state affects whether this
                              bucket is live. Only required for fast
                              failover groups. */
    size_t                     actions_num;
    struct ofl_action_header **actions;
};

struct ofl_flow_stats { 
    uint8_t                         table_id;      /* ID of table flow came from. */
    uint32_t                        duration_sec;  /* Time flow has been alive in secs. */
    uint32_t                        duration_nsec; /* Time flow has been alive in nsecs
                                                     beyond duration_sec. */
    uint16_t                        priority;      /* Priority of the entry. Only meaningful
                                                     when this is not an exact-match entry. */
    uint16_t                        idle_timeout;  /* Number of seconds idle before
                                                     expiration. */
    uint16_t                        hard_timeout;  /* Number of seconds before expiration. */
    uint64_t                        cookie;        /* Opaque controller-issued identifier. */
    uint64_t                        packet_count;  /* Number of packets in flow. */
    uint64_t                        byte_count;    /* Number of bytes in flow. */
    struct ofl_match_header        *match;         /* Description of fields. */
    size_t                          instructions_num;
    struct ofl_instruction_header **instructions; /* Instruction set. */
};



struct ofl_table_stats {
    uint8_t    table_id;      /* Identifier of table. Lower numbered tables
                                are consulted first. */
    char      *name;
    uint32_t   wildcards;     /* Bitmap of OFPFMF_* wildcards that are
                                supported by the table. */
    uint32_t   match;         /* Bitmap of OFPFW ::::: OFPFMF_* that indicate
                                the fields the table can match on. */
    uint32_t   instructions;  /* Bitmap of OFPIT_* values supported. */
    uint32_t   write_actions; /* Bitmap of OFPAT_* that are supported
                               by the table with OFPIT_WRITE_ACTIONS. */
    uint32_t   apply_actions; /* Bitmap of OFPAT_* that are supported
                                by the table with OFPIT_APPLY_ACTIONS. */
    uint32_t   config;        /* Bitmap of OFPTC_* values */
    uint32_t   max_entries;   /* Max number of entries supported. */
    uint32_t   active_count;  /* Number of active entries. */
    uint64_t   lookup_count;  /* Number of packets looked up in table. */
    uint64_t   matched_count; /* Number of packets that hit table. */
};


struct ofl_match_header {
    uint16_t   type;             /* One of OFPMT_* */
    uint16_t   length;           /* Match length */
};

struct ofl_match {
    struct ofl_match_header   header; /* Match header */
    struct hmap match_fields;         /* Match fields. Contain OXM TLV's  */
};

struct ofl_match_tlv{
    
    struct hmap_node hmap_node;
    uint32_t header;    /* TLV header */
    uint8_t *value;     /* TLV value */
};

struct ofl_port_stats {
    uint32_t   port_no;
    uint64_t   rx_packets;   /* Number of received packets. */
    uint64_t   tx_packets;   /* Number of transmitted packets. */
    uint64_t   rx_bytes;     /* Number of received bytes. */
    uint64_t   tx_bytes;     /* Number of transmitted bytes. */
    uint64_t   rx_dropped;   /* Number of packets dropped by RX. */
    uint64_t   tx_dropped;   /* Number of packets dropped by TX. */
    uint64_t   rx_errors;    /* Number of receive errors. This is a super-set
                               of more specific receive errors and should be
                               greater than or equal to the sum of all
                               rx_*_err values. */
    uint64_t   tx_errors;    /* Number of transmit errors. This is a super-set
                               of more specific transmit errors and should be
                               greater than or equal to the sum of all
                               tx_*_err values (none currently defined.) */
    uint64_t   rx_frame_err; /* Number of frame alignment errors. */
    uint64_t   rx_over_err;  /* Number of packets with RX overrun. */
    uint64_t   rx_crc_err;   /* Number of CRC errors. */
    uint64_t   collisions;   /* Number of collisions. */
};

struct ofl_bucket_counter {
    uint64_t   packet_count; /* Number of packets processed by bucket. */
    uint64_t   byte_count;   /* Number of bytes processed by bucket. */
};

struct ofl_group_stats {
    uint32_t   group_id;
    uint32_t   ref_count;
    uint64_t   packet_count;
    uint64_t   byte_count;
    size_t                      counters_num;
    struct ofl_bucket_counter **counters;
};


struct ofl_port {
    uint32_t   port_no;
    uint8_t    hw_addr[OFP_ETH_ALEN];
    char      *name;

    uint32_t   config;        /* Bitmap of OFPPC_* flags. */
    uint32_t   state;         /* Bitmap of OFPPS_* flags. */

    uint32_t   curr;          /* Current features. */
    uint32_t   advertised;    /* Features being advertised by the port. */
    uint32_t   supported;     /* Features supported by the port. */
    uint32_t   peer;          /* Features advertised by peer. */

    uint32_t   curr_speed;    /* Current port bitrate in kbps. */
    uint32_t   max_speed;     /* Max port bitrate in kbps */
};



struct ofl_queue_stats {
    uint32_t   port_no;
    uint32_t   queue_id;   /* Queue i.d */
    uint64_t   tx_bytes;   /* Number of transmitted bytes. */
    uint64_t   tx_packets; /* Number of transmitted packets. */
    uint64_t   tx_errors;  /* Number of packets dropped due to overrun. */
};



struct ofl_group_desc_stats {
    uint8_t             type;        /* One of OFPGT_*. */
    uint32_t            group_id;    /* Group identifier. */

    size_t              buckets_num;
    struct ofl_bucket **buckets;
};


/****************************************************************************
 * Utility functions to match structure
 ****************************************************************************/
void
ofl_structs_match_init(struct ofl_match *match);

void
ofl_structs_match_put8(struct ofl_match *match, uint32_t header, uint8_t value);

void
ofl_structs_match_put8m(struct ofl_match *match, uint32_t header, uint8_t value, uint8_t mask);

void
ofl_structs_match_put16(struct ofl_match *match, uint32_t header, uint16_t value);

void
ofl_structs_match_put16m(struct ofl_match *match, uint32_t header, uint16_t value, uint16_t mask);

void
ofl_structs_match_put32(struct ofl_match *match, uint32_t header, uint32_t value);

void
ofl_structs_match_put32m(struct ofl_match *match, uint32_t header, uint32_t value, uint32_t mask);

void
ofl_structs_match_put64(struct ofl_match *match, uint32_t header, uint64_t value);

void
ofl_structs_match_put64m(struct ofl_match *match, uint32_t header, uint64_t value, uint64_t mask);

void
ofl_structs_match_put_eth(struct ofl_match *match, uint32_t header, uint8_t value[ETH_ADDR_LEN]);

void
ofl_structs_match_put_eth_m(struct ofl_match *match, uint32_t header, uint8_t value[ETH_ADDR_LEN], uint8_t mask[ETH_ADDR_LEN]);

void 
ofl_structs_match_put_ipv6(struct ofl_match *match, uint32_t header, const struct in6_addr *value);

void 
ofl_structs_match_put_ipv6(struct ofl_match *match, uint32_t header, const struct in6_addr *value);

void 
ofl_structs_match_put_ipv6m(struct ofl_match *match, uint32_t header, const struct in6_addr *value, const struct in6_addr *mask);

int 
ofl_structs_match_ofp_total_len(struct ofl_match *match);

void
ofl_structs_match_convert_pktf2oflm(struct hmap * hmap_packet_fields, struct ofl_match * match);
/****************************************************************************
 * Functions for (un)packing structures
 ****************************************************************************/

size_t
ofl_structs_instructions_pack(struct ofl_instruction_header *src, struct ofp_instruction *dst, struct ofl_exp *exp);

size_t
ofl_structs_bucket_pack(struct ofl_bucket *src, struct ofp_bucket *dst, struct ofl_exp *exp);

size_t
ofl_structs_flow_stats_pack(struct ofl_flow_stats *src, uint8_t *dst, struct ofl_exp *exp);

size_t
ofl_structs_group_stats_pack(struct ofl_group_stats *src, struct ofp_group_stats *dst);

size_t
ofl_structs_queue_prop_pack(struct ofl_queue_prop_header *src, struct ofp_queue_prop_header *dst);

size_t
ofl_structs_packet_queue_pack(struct ofl_packet_queue *src, struct ofp_packet_queue *dst);

size_t
ofl_structs_port_stats_pack(struct ofl_port_stats *src, struct ofp_port_stats *dst);


size_t
ofl_structs_port_pack(struct ofl_port *src, struct ofp_port *dst);

size_t
ofl_structs_table_stats_pack(struct ofl_table_stats *src, struct ofp_table_stats *dst);


size_t
ofl_structs_queue_stats_pack(struct ofl_queue_stats *src, struct ofp_queue_stats *dst);

size_t
ofl_structs_group_desc_stats_pack(struct ofl_group_desc_stats *src, struct ofp_group_desc_stats *dst, struct ofl_exp *exp);

size_t
ofl_structs_bucket_counter_pack(struct ofl_bucket_counter *src, struct ofp_bucket_counter *dst);

size_t
ofl_structs_match_pack(struct ofl_match_header *src, struct ofp_match *dst, uint8_t* oxm_fields, struct ofl_exp *exp);


ofl_err
ofl_structs_instructions_unpack(struct ofp_instruction *src, size_t *len, struct ofl_instruction_header **dst, struct ofl_exp *exp);

ofl_err
ofl_structs_bucket_unpack(struct ofp_bucket *src, size_t *len, uint8_t gtype, struct ofl_bucket **dst, struct ofl_exp *exp);

ofl_err
ofl_structs_flow_stats_unpack(struct ofp_flow_stats *src,uint8_t *buf, size_t *len, struct ofl_flow_stats **dst, struct ofl_exp *exp);

ofl_err
ofl_structs_queue_prop_unpack(struct ofp_queue_prop_header *src, size_t *len, struct ofl_queue_prop_header **dst);

ofl_err
ofl_structs_packet_queue_unpack(struct ofp_packet_queue *src, size_t *len, struct ofl_packet_queue **dst);

ofl_err
ofl_structs_port_unpack(struct ofp_port *src, size_t *len, struct ofl_port **dst);

ofl_err
ofl_structs_table_stats_unpack(struct ofp_table_stats *src, size_t *len, struct ofl_table_stats **dst);

ofl_err
ofl_structs_port_stats_unpack(struct ofp_port_stats *src, size_t *len, struct ofl_port_stats **dst);

ofl_err
ofl_structs_group_stats_unpack(struct ofp_group_stats *src, size_t *len, struct ofl_group_stats **dst);

ofl_err
ofl_structs_queue_stats_unpack(struct ofp_queue_stats *src, size_t *len, struct ofl_queue_stats **dst);

ofl_err
ofl_structs_group_desc_stats_unpack(struct ofp_group_desc_stats *src, size_t *len, struct ofl_group_desc_stats **dst, struct ofl_exp *exp);

ofl_err
ofl_structs_bucket_counter_unpack(struct ofp_bucket_counter *src, size_t *len, struct ofl_bucket_counter **dst);

ofl_err
ofl_structs_match_unpack(struct ofp_match *src,uint8_t *buf, size_t *len, struct ofl_match_header **dst, struct ofl_exp *exp);



/****************************************************************************
 * Functions for freeing action structures
 ****************************************************************************/

void
ofl_structs_free_packet_queue(struct ofl_packet_queue *queue);

void
ofl_structs_free_instruction(struct ofl_instruction_header *inst, struct ofl_exp *exp);

void
ofl_structs_free_table_stats(struct ofl_table_stats *stats);

void
ofl_structs_free_bucket(struct ofl_bucket *bucket, struct ofl_exp *exp);

void
ofl_structs_free_flow_stats(struct ofl_flow_stats *stats, struct ofl_exp *exp);

void
ofl_structs_free_port(struct ofl_port *port);

void
ofl_structs_free_group_stats(struct ofl_group_stats *stats);

void
ofl_structs_free_group_desc_stats(struct ofl_group_desc_stats *stats, struct ofl_exp *exp);

void
ofl_structs_free_match(struct ofl_match_header *match, struct ofl_exp *exp);



/****************************************************************************
 * Utility functions
 ****************************************************************************/

/* Given a list of structures in OpenFlow wire format, these functions return
 * the count of those structures in the passed in byte array. The functions
 * return an ofl_err in case of an error, or 0 on succes. */
ofl_err
ofl_utils_count_ofp_instructions(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_buckets(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_ports(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_flow_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_group_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_table_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_bucket_counters(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_port_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_queue_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_group_desc_stats(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_packet_queues(void *data, size_t data_len, size_t *count);

ofl_err
ofl_utils_count_ofp_queue_props(void *data, size_t data_len, size_t *count);


size_t
ofl_structs_instructions_ofp_total_len(struct ofl_instruction_header **instructions, size_t instructions_num, struct ofl_exp *exp);

size_t
ofl_structs_instructions_ofp_len(struct ofl_instruction_header *instruction, struct ofl_exp *exp);

size_t
ofl_structs_buckets_ofp_total_len(struct ofl_bucket ** buckets, size_t buckets_num, struct ofl_exp *exp);

size_t
ofl_structs_buckets_ofp_len(struct ofl_bucket *bucket, struct ofl_exp *exp);

size_t
ofl_structs_flow_stats_ofp_total_len(struct ofl_flow_stats ** stats, size_t stats_num, struct ofl_exp *exp);

size_t
ofl_structs_flow_stats_ofp_len(struct ofl_flow_stats *stats, struct ofl_exp *exp);

size_t
ofl_structs_group_stats_ofp_total_len(struct ofl_group_stats ** stats, size_t stats_num);

size_t
ofl_structs_group_stats_ofp_len(struct ofl_group_stats *stats);

size_t
ofl_structs_group_desc_stats_ofp_total_len(struct ofl_group_desc_stats ** stats, size_t stats_num, struct ofl_exp *exp);

size_t
ofl_structs_group_desc_stats_ofp_len(struct ofl_group_desc_stats *stats, struct ofl_exp *exp);

size_t
ofl_structs_queue_prop_ofp_total_len(struct ofl_queue_prop_header ** props, size_t props_num);

size_t
ofl_structs_queue_prop_ofp_len(struct ofl_queue_prop_header *prop);

size_t
ofl_structs_packet_queue_ofp_total_len(struct ofl_packet_queue ** queues, size_t queues_num);

size_t
ofl_structs_packet_queue_ofp_len(struct ofl_packet_queue *queue);

size_t
ofl_structs_match_ofp_len(struct ofl_match_header *match, struct ofl_exp *exp);


/****************************************************************************
 * Functions for printing structures
 ****************************************************************************/

char *
ofl_structs_port_to_string(struct ofl_port *port);

void
ofl_structs_port_print(FILE *stream, struct ofl_port *port);

char *
ofl_structs_instruction_to_string(struct ofl_instruction_header *inst, struct ofl_exp *exp);

void
ofl_structs_instruction_print(FILE *stream, struct ofl_instruction_header *inst, struct ofl_exp *exp);

char *
ofl_structs_match_to_string(struct ofl_match_header *match, struct ofl_exp *exp);

void
ofl_structs_match_print(FILE *stream, struct ofl_match_header *match, struct ofl_exp *exp);

void
print_oxm_tlv(FILE *stream, struct ofl_match_tlv *f, size_t *size);

char *
ofl_structs_config_to_string(struct ofl_config *c);

void
ofl_structs_config_print(FILE *stream, struct ofl_config *c);

char *
ofl_structs_bucket_to_string(struct ofl_bucket *b, struct ofl_exp *exp);

void
ofl_structs_bucket_print(FILE *stream, struct ofl_bucket *b, struct ofl_exp *exp);

char *
ofl_structs_queue_to_string(struct ofl_packet_queue *q);

void
ofl_structs_queue_print(FILE *stream, struct ofl_packet_queue *q);

char *
ofl_structs_queue_prop_to_string(struct ofl_queue_prop_header *p);

void
ofl_structs_queue_prop_print(FILE *stream, struct ofl_queue_prop_header *p);

char *
ofl_structs_flow_stats_to_string(struct ofl_flow_stats *s, struct ofl_exp *exp);

void
ofl_structs_flow_stats_print(FILE *stream, struct ofl_flow_stats *s, struct ofl_exp *exp);

char *
ofl_structs_bucket_counter_to_string(struct ofl_bucket_counter *s);

void
ofl_structs_bucket_counter_print(FILE *stream, struct ofl_bucket_counter *c);

char *
ofl_structs_group_stats_to_string(struct ofl_group_stats *s);

void
ofl_structs_group_stats_print(FILE *stream, struct ofl_group_stats *s);

char *
ofl_structs_table_stats_to_string(struct ofl_table_stats *s);

void
ofl_structs_table_stats_print(FILE *stream, struct ofl_table_stats *s);

char *
ofl_structs_port_stats_to_string(struct ofl_port_stats *s);

void
ofl_structs_port_stats_print(FILE *stream, struct ofl_port_stats *s);

char *
ofl_structs_queue_stats_to_string(struct ofl_queue_stats *s);

void
ofl_structs_queue_stats_print(FILE *stream, struct ofl_queue_stats *s);

char *
ofl_structs_group_desc_stats_to_string(struct ofl_group_desc_stats *s, struct ofl_exp *exp);

void
ofl_structs_group_desc_stats_print(FILE *stream, struct ofl_group_desc_stats *s, struct ofl_exp *exp);

#endif /* OFL_STRUCTS_H */
