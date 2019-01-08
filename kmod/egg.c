
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h> /* copy_from/to_user */
#include <stdbool.h>

MODULE_LICENSE("Dual BSD/GPL");

# define UINT32_MAX		(4294967295U)
typedef unsigned int uint32_t;
typedef long long unsigned int uint64_t;

enum operation_t
{
	operation_add,
	operation_sub,
	operation_mul,
	operation_div
};

int egg_open(struct inode *inode, struct file *filp);
int egg_release(struct inode *inode, struct file *filp);
ssize_t egg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t egg_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
void egg_exit(void);
int egg_init(void);
bool is_valid_operation(uint32_t arg1, enum operation_t operation, uint32_t arg2);
bool fibo(uint32_t n, uint32_t* result);



struct file_operations egg_fops = {
  read: egg_read,
  write: egg_write,
  open: egg_open,
  release: egg_release
};

module_init(egg_init);
module_exit(egg_exit);

int egg_major = 60;

int egg_init(void)
{
  int result = register_chrdev(egg_major, "egg", &egg_fops);
  if (result < 0) {
    printk(
      "egg module: cannot obtain major number %d\n", egg_major);
    return result;
  }

  printk("Inserting egg module\n"); 
  return 0;
}


void egg_exit(void)
{
  unregister_chrdev(egg_major, "egg");

  printk("Removing egg module\n");
}


int egg_open(struct inode *inode, struct file *filp)
{
  return 0;
}

int egg_release(struct inode *inode, struct file *filp)
{
  return 0;
}


ssize_t egg_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{ 
  return 0;
}


ssize_t egg_write( struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int parsing_result;
	unsigned long parsing_value;
  uint32_t value;

		if(count > 10)
		count = 10;

    parsing_result = kstrtoul_from_user(buf, count, 10u, &parsing_value);
    
		if (parsing_result)
		{
        printk("egg module: input value parsing failed\n");
        return parsing_result;
    } else
		{
			if(parsing_value > UINT32_MAX)
			{
				printk("egg module: uint32_t overflow\n");
				return count;
			}
			else
			{
				value = (uint32_t) parsing_value;
			}

			printk("egg module input = %u\n", value);			

			if(!fibo(value, &value))
			{
				printk("egg module: uint32_t overflow\n");
				return count;
			}

			printk("egg module output = %u\n", value);
		}
		
	return count;
}

bool is_valid_operation(uint32_t arg1, enum operation_t operation, uint32_t arg2)
{
	if(arg2 == (uint32_t)0 && operation == operation_div)
		return false;

	switch(operation)
	{
	case operation_add:
		return arg1 <= UINT32_MAX - arg2;
	case operation_sub:
		return arg2 <= arg1;
	case operation_mul:
		return arg1 <= UINT32_MAX / arg2;
	case operation_div:
		return true;
	default:
		return false;
	}
}

bool fibo(uint32_t n, uint32_t* result)
{
  uint32_t first_to_prev =0, prev = 1, i = 0;

	if(!result)
		return false;

    first_to_prev = 0;
    prev = 1;

	for(i = 0; i < (uint64_t)n + 1; ++i)
	{
		if(i <= 1)
		{
			*result = i;
		}
		else
		{
			if(!is_valid_operation(first_to_prev, operation_add, prev))
				return false;

			*result = first_to_prev + prev;
			first_to_prev = prev;
			prev = *result;
		}
	}

	return true;
}

