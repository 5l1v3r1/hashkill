/* threads.h
 *
 * hashkill - a hash cracking tool
 * Copyright (C) 2010 Milen Rangelov <gat3way@gat3way.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef OCLHASHTHREADS_H
#define OCLHASHTHREADS_H

#include <stdint.h>
#include "ocl-base.h"
#include "lzma/lzma.h"
#include "err.h"
#include "hashinterface.h"
#include "ocl_support.h"


#define MAXFOUND 1024*64

/* Used by rule engine GPU offload */
typedef void (*callback_final_t)(char *line, int self);



/* function prototypes */
hash_stat ocl_get_device();
hash_stat ocl_bruteforce();
hash_stat ocl_markov();
hash_stat ocl_rule();
hash_stat ocl_spawn_threads(unsigned int num, unsigned int queue_size);
hash_stat ocl_restore_counts(uint64_t current, uint64_t overall);
hash_stat ocl_is_supported_plugin(char *plugin);
void ocl_inc_inv(int cur, int csize, int vsize);
void rule_offload_add_set(callback_final_t cb,int self);
void rule_offload_add_cset(callback_final_t cb,int self);
void rule_offload_may_add_set(callback_final_t cb,int self);
void rule_offload_may_add_cset(callback_final_t cb,int self);
void rule_offload_add_markov(callback_final_t cb,int self);
void rule_offload_may_add_markov(callback_final_t cb,int self);
void rule_offload_add_numrange(callback_final_t cb,int self);
void rule_offload_add_none(callback_final_t cb,int self);
void rule_offload_perform(callback_final_t cb,int self);
void suggest_rule_attack(void);
void cancel_crack_threads(void);



/* globals */
volatile uint64_t invocations;
volatile uint64_t idealinvocations;
pthread_mutex_t listmutex;
pthread_mutex_t crackedmutex;
int ocl_threads;
int ocl_user_threads;
int interactive_mode;
int devicesnum;

/* Used by all kernels */
cl_platform_id platform[HASHKILL_MAXTHREADS];
cl_device_id device[HASHKILL_MAXTHREADS];
pthread_mutex_t biglock;
cl_context context[HASHKILL_MAXTHREADS];
pthread_t crack_threads[HASHKILL_MAXTHREADS];
cl_program program[HASHKILL_MAXTHREADS];
cl_program program16[HASHKILL_MAXTHREADS][16];

/* Used by markov/bruteforce kernels */
cl_uint *table;
cl_uint *bitmaps;
char reduced_charset[88];
int reduced_size;


/* Used by rule attack */
size_t ocl_rule_workset[HASHKILL_MAXTHREADS];
cl_command_queue rule_oclqueue[HASHKILL_MAXTHREADS];
size_t *rule_local_work_size;
cl_mem rule_buffer[HASHKILL_MAXTHREADS];
cl_kernel rule_kernel[HASHKILL_MAXTHREADS];
cl_kernel rule_kernel2[HASHKILL_MAXTHREADS];
cl_kernel rule_kernel16[16][HASHKILL_MAXTHREADS];
cl_kernel rule_kernel162[16][HASHKILL_MAXTHREADS];
int self_kernel16[HASHKILL_MAXTHREADS];
cl_mem rule_bitmaps_buf[HASHKILL_MAXTHREADS];
char *rule_ptr[HASHKILL_MAXTHREADS];
cl_mem rule_found_ind_buf[HASHKILL_MAXTHREADS];
cl_mem rule_found_buf[HASHKILL_MAXTHREADS];
cl_uint4 rule_singlehash[HASHKILL_MAXTHREADS];
cl_ulong4 rule_singlehash_long[HASHKILL_MAXTHREADS];
cl_ulong8 rule_singlehash_long8[HASHKILL_MAXTHREADS];

cl_mem rule_images_buf[HASHKILL_MAXTHREADS];
cl_mem rule_images2_buf[HASHKILL_MAXTHREADS];
cl_mem rule_sizes_buf[HASHKILL_MAXTHREADS];
cl_mem rule_sizes2_buf[HASHKILL_MAXTHREADS];
cl_mem rule_images16_buf[16][HASHKILL_MAXTHREADS];
cl_mem rule_images162_buf[16][HASHKILL_MAXTHREADS];
cl_mem rule_sizes16_buf[16][HASHKILL_MAXTHREADS];
cl_mem rule_sizes162_buf[16][HASHKILL_MAXTHREADS];


char *rule_images[HASHKILL_MAXTHREADS];
char *rule_images2[HASHKILL_MAXTHREADS];
int *rule_sizes[HASHKILL_MAXTHREADS];
int *rule_sizes2[HASHKILL_MAXTHREADS];
char *rule_images16[16][HASHKILL_MAXTHREADS];
char *rule_images162[16][HASHKILL_MAXTHREADS];
int *rule_sizes16[16][HASHKILL_MAXTHREADS];
int *rule_sizes162[16][HASHKILL_MAXTHREADS];

cl_uint *rule_found_ind[HASHKILL_MAXTHREADS];
int rule_counts[HASHKILL_MAXTHREADS][32]; // Padded to cacheline to avoid false sharing


#endif