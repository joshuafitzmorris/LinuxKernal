#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <asm/errno.h>

asmlinkage long sys_array_stats(
            struct array_stats *stats,
            long data[],
            long size);

asmlinkage long sys_array_stats(struct array_stats* stats, long data[], long size)
{
	struct array_stats tmp_stats;
	long i = 0;	
	long val = 0;

	if(size <= 0) { // Passed by value
		return -EINVAL;
	}

	if(!stats)
	{
		return -EFAULT;
	}
	
	if(!data) {
		return -EFAULT;
	}
	
	// 1. Need to copy data from user level using copy_from_user() to ensure safe copy
	if(copy_from_user(&val, &data[0], sizeof(long))) {
		// If problems accessing data return error 
		return -EFAULT;
	}

	tmp_stats.min = val;
	tmp_stats.max = val;
	tmp_stats.sum = val;

	// 2. Calculate stats		
	for(i=1; i<size; i++) {

		if(copy_from_user(&val, &data[i], sizeof(long))) {
			return -EFAULT;
		}

		if(val < tmp_stats.min){
			tmp_stats.min = val;
		}
		else if(val > tmp_stats.max){
			tmp_stats.max = val;
		}

		tmp_stats.sum += val;

	}

	// 3. Copy values
	if (copy_to_user(&(stats->sum), &(tmp_stats.sum), sizeof(long))){
		return -EFAULT;
	}

	if (copy_to_user(&(stats->min), &(tmp_stats.min), sizeof(long))){
		return -EFAULT;
	}

	if (copy_to_user(&(stats->max), &(tmp_stats.max), sizeof(long))){
		return -EFAULT;
	}

	// Debug
	printk("Min:%li Max:%li Sum:%li\n", stats->min, stats->max, stats->sum);
	
	return 0;		 
}