/**
 * vi: ts=4:sw=4
 *
 * @file
 * openflow.c
 *
 * This file contains the main OpenFlow functions
 *
 */

/*
 * This file is part of the Zodiac FX firmware.
 * Copyright (c) 2016 Northbound Networks.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Paul Zanna <paul@northboundnetworks.com>
 *
 */
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "trace.h"
#include "openflow.h"
#include "timers.h"
#include "ip_addr.h"


#define TCP_MSS           536

// Global variables
extern uint8_t port_status[TOTAL_PORTS];
//used for packet stats for every port
struct ofp10_port_stats phys10_port_stats[TOTAL_PORTS];
struct ofp13_port_stats phys13_port_stats[TOTAL_PORTS];

// Local Variables
struct ofp_switch_config Switch_config;
struct ofp_flow_mod *flow_match10[MAX_FLOWS_10];
struct ofp13_flow_mod *flow_match13[MAX_FLOWS_13];
struct group_entry13 group_entry13[MAX_GROUPS];
struct action_bucket action_bucket[MAX_BUCKETS];
uint8_t *ofp13_oxm_match[MAX_FLOWS_13];
uint8_t *ofp13_oxm_inst[MAX_FLOWS_13];
uint16_t ofp13_oxm_inst_size[MAX_FLOWS_13];
struct flows_counter flow_counters[MAX_FLOWS_13];
struct flow_tbl_actions *flow_actions10[MAX_FLOWS_10];
struct table_counter table_counters[MAX_TABLES];
int iLastFlow = 0;
int iLastMeter = 0;
uint8_t port_status[TOTAL_PORTS] = {1,1,1,1}; //al port set to up
uint8_t NativePortMatrix = 0; //bitmap for  port status
struct meter_entry13 *meter_entry[MAX_METER_13];
struct meter_band_stats_array band_stats_array[MAX_METER_13];
uint8_t shared_buffer[SHARED_BUFFER_LEN];
char sysbuf[64];
struct ip_addr serverIP;
int OF_Version = 0x00;
struct tcp_pcb *tcp_pcb;
struct tcp_pcb *tcp_pcb_check;
int fast_of_timer = 0;
int tcp_con_state = -1;
int tcp_wait = 0;
int totaltime = 0;
int heartbeat = 0;
uint32_t reply_more_xid = 0;
bool reply_more_flag = false;
bool rcv_freq;

// Buffer for multi-segment messages
#define PACKET_BUFFER_SIZE		(2*TCP_MSS+64)	// TODO: Ideally would be (2*1536)
static uint8_t packet_buffer[PACKET_BUFFER_SIZE];
static unsigned int packet_buffer_off = 0;
static unsigned int packet_buffer_len = 0;

// Internal Functions
void OF_hello(void);
void echo_request(void);
void echo_reply(struct ofp_header *ofph, int len);

/*
*	Converts a 64bit value from host to network format
*
*	@param n - value to convert.
*
*/
static inline uint64_t (HTONLl)(uint64_t n)
{
	return HTONL(1) == 1 ? n : ((uint64_t) HTONL(n) << 32) | HTONL(n >> 32);
}

/*
*	Main OpenFlow table lookup Function
*
*	@param p_uc_data - pointer to the packet buffer.
*	@param ul_size - Size of the packet.
*	@param port	- In Port.
*
*/
void nnOF_tablelookup(uint8_t *p_uc_data, uint32_t *ul_size, int port)
{
	if (OF_Version == 0x01) nnOF10_tablelookup(p_uc_data, ul_size, port);
	if (OF_Version == 0x04) nnOF13_tablelookup(p_uc_data, ul_size, port);
	return;
}

void gmac_write(uint8_t *p_buffer, uint16_t ul_size, uint8_t port)
{
	return;
}


uint32_t sys_get_ms(void)
{
	return 0;
}

void sendtcp(const void *buffer, uint16_t len, uint8_t push)
{
	return;
}