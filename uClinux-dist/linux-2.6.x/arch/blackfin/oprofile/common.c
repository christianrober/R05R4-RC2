/*
 * Copyright (C) 2004 Anton Blanchard <anton@au.ibm.com>, IBM
 *
 * Based on alpha version.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/oprofile.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/errno.h>

#include <asm/ptrace.h>
#include <asm/system.h>
#include <asm/semaphore.h>
#include <asm/blackfin.h>
#include <asm/irq.h>
#include <asm/io.h>

#include "op_blackfin.h"

#define BFIN_533_ID  0xE5040003
#define BFIN_537_ID  0xE5040002 

static int pfmon_enabled;
static struct semaphore pfmon_sem;

extern struct op_bfin533_model op_model_bfin533;
struct op_bfin533_model *model;

struct op_counter_config ctr[OP_MAX_COUNTER];

static int op_bfin_setup(void)
{
	int ret;
  
	/* Pre-compute the values to stuff in the hardware registers.  */
	spin_lock(&oprofilefs_lock);
	ret = model->reg_setup(ctr);  
	spin_unlock(&oprofilefs_lock);
 
	return ret;
}

static void op_bfin_shutdown(void)
{
#if 0
     /* what is the difference between shutdown and stop? */
#endif
}

static int op_bfin_start(void)
{
	int ret = -EBUSY;
	printk("KSDBG:in %s\n",__FUNCTION__); 
	down(&pfmon_sem);
	if (!pfmon_enabled) {
		ret = model->start(ctr);
		pfmon_enabled = !ret;
	}
	up(&pfmon_sem);
  
	return ret;
}

static void op_bfin_stop(void)
{
	down(&pfmon_sem);
	if (pfmon_enabled) {
		model->stop();
		pfmon_enabled = 0;
	}
	up(&pfmon_sem);    
}

static int op_bfin_create_files(struct super_block *sb, struct dentry *root)
{
	int i;

	for (i = 0; i < model->num_counters; ++i) {
		struct dentry *dir;
		char buf[3];
		printk("Oprofile: creating files... \n");
		
		snprintf(buf, sizeof buf, "%d", i);
		dir = oprofilefs_mkdir(sb, root, buf);
		
		oprofilefs_create_ulong(sb, dir, "enabled", &ctr[i].enabled);
		oprofilefs_create_ulong(sb, dir, "event", &ctr[i].event);
		oprofilefs_create_ulong(sb, dir, "count", &ctr[i].count);
		/*
		 * We dont support per counter user/kernel selection, but
		 * we leave the entries because userspace expects them
		 */
	oprofilefs_create_ulong(sb, dir, "kernel", &ctr[i].kernel);
	oprofilefs_create_ulong(sb, dir, "user", &ctr[i].user);
	oprofilefs_create_ulong(sb, dir, "unit_mask", &ctr[i].unit_mask);
	}
	
	return 0;
}
int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	unsigned int dspid;

	init_MUTEX(&pfmon_sem);

	dspid = *pDSPID;

        printk("Oprofile got the cpu id is 0x%x. \n", dspid);

	switch (dspid) {
		case BFIN_533_ID:
		     model = &op_model_bfin533;
		     model->num_counters = 2;
		     break;
		case BFIN_537_ID:
		     model = &op_model_bfin533;
                     model->num_counters = 2;
		     break;
		default:
		     return -ENODEV;
	}	

	ops->cpu_type = model->name;
	ops->create_files = op_bfin_create_files;
	ops->setup = op_bfin_setup;
	ops->shutdown = op_bfin_shutdown;
	ops->start = op_bfin_start;
	ops->stop = op_bfin_stop;
	

	printk(KERN_INFO "oprofile: using %s performance monitoring.\n",
        ops->cpu_type);

	return 0;
}

void oprofile_arch_exit(void)
{
}
