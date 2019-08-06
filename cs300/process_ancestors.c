#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include <asm/uaccess.h>
#include <asm/errno.h>


asmlinkage long sys_process_ancestors(
                struct process_info info_array[],
                long size,
                long *num_filled);


asmlinkage long sys_process_ancestors(struct process_info info_array[], long size, long* num_filled)
{
	struct task_struct* curr_task = current;
	struct list_head* task_struct_static = NULL;
    struct process_info info;
	long tmp_num_filled = 0;
	long num_children = 0;
	long num_siblings = 0;

	if (size <= 0) {
		return -EINVAL;
	}

	if (!num_filled) {
		return -EFAULT;
	}

    // 1. Iterate until child == parent process
	while (curr_task->parent != curr_task)
	{
        if(tmp_num_filled > 0)
        {
		    curr_task = curr_task->parent;
        }

		// 2 a. Count num_children of this process
		num_children = 0;
		task_struct_static = &(curr_task->children);
        while((task_struct_static != NULL) && (task_struct_static != &(curr_task->children)) )
        {
            task_struct_static = task_struct_static->next;
            num_children++;
        }
		
		// 2 b. Count num_siblings of this process
		num_siblings = 0;
		task_struct_static = &(curr_task->sibling);
        while((task_struct_static != NULL) && (task_struct_static != &(curr_task->sibling)) )
        {
            task_struct_static = task_struct_static->next;
            num_siblings++;
        }

        // 2 c. Debugging: Print info to screen
		printk("pid: %li, name: %s, state: %li, uid: %li, nvcsw: %li, nivscw: %li, num_children: %li, num_siblings: %li\n",
		    (long)curr_task->pid, curr_task->comm, curr_task->state, (long)(curr_task->cred)->uid.val,
            curr_task->nvcsw, curr_task->nivcsw, num_children, num_siblings);
		
		// 3. Check size
		if (tmp_num_filled >= size) {
            break;
		}
        else // Store info if size is still in range
        {
            info.pid = (long) curr_task->pid;
			strcpy(info.name, curr_task->comm);
			info.state = curr_task->state;
			info.uid = (long) (curr_task->cred)->uid.val;
			info.nvcsw = curr_task->nvcsw;
			info.nivcsw = curr_task->nivcsw;
			info.num_children = num_children;
			info.num_siblings = num_siblings;

            // Error check: Problem's accessing array
			if(copy_to_user(&info_array[tmp_num_filled], &info, sizeof(struct process_info))){
				return -EFAULT;
			}

			tmp_num_filled++;
        }
	}

    // 4. Copy number of filled values in info_array[]
	if (copy_to_user(num_filled, &tmp_num_filled, sizeof(long))) {
		return -EFAULT;
	}
	
	return 0;
}