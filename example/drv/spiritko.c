#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/fs.h>
#include "spirit_msg.h"

static const char * DEVICE_NAME = "spdrv";
#define DRIVER_VERSION "v1.0"

#define MAGIC 0xF239

#define IOCTL_SPIRIT_MSG		_IOWR(MAGIC, 0, SpiritMsg)

#define DEBUG(format, arg...) printk(KERN_INFO "SPIRIT MSG: " format "\n", ## arg)
#define ERROR(format, arg...) printk(KERN_INFO "SPIRIT ERR: " format "\n", ## arg)

MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);

static spinlock_t m_lock;

static int opened_cnt = 0;

static int device_open(struct inode *inode, struct file *filp)
{
	spin_lock(&m_lock);
	try_module_get(THIS_MODULE);
	opened_cnt += 1;
	filp->private_data = inode;
	DEBUG("Device opened %i times", opened_cnt);
	spin_unlock(&m_lock);

	return 0;
}

static int device_release(struct inode *inode, struct file *filp)
{
	spin_lock(&m_lock);
	filp->private_data = NULL;
	DEBUG("Device closed, was opened %i times", opened_cnt);
	module_put(THIS_MODULE);
	opened_cnt -= 1;
	spin_unlock(&m_lock);
	// return -EIO
	return 0;
}

ssize_t device_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	return -EIO;
}

ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	return -EIO;
}

static long device_ioctl(struct file *filp, unsigned int ioctl_num, unsigned long param)
{
	int result = 0;
	SpiritMsg spmsg;
	spin_lock(&m_lock);

	switch (ioctl_num)
    {
		case IOCTL_SPIRIT_MSG:
		{
			result = copy_from_user(&spmsg, (SpiritMsg*)param, sizeof(SpiritMsg));
			if (result == 0) {
				DEBUG("title = %d", spmsg.title);
				if (copy_to_user((SpiritMsg*)param, &spmsg, sizeof(SpiritMsg))) result = -EINVAL;
			} else {
				ERROR("copy_from_user failed with %d", result);
			}
			break;
		}
		default:
		{
			DEBUG("Unsupported ioctl code");
			result = -EINVAL;
		}

	}

	spin_unlock(&m_lock);
	return result;
}

static const struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.open			= device_open,
	.release		= device_release,
    .read			= device_read,
    .write			= device_write,
    .compat_ioctl	= device_ioctl,
    .unlocked_ioctl	= device_ioctl
};

static int __init drv_init(void)
{
	int result;
	DEBUG("Loading drv...");
	result = register_chrdev(0, DEVICE_NAME, &dev_fops);

	if (result < 0) {
		ERROR("Register chrdev failed");
	} else {
		DEBUG("Driver successfully loaded");
	}

	return 0;
}

static void __exit drv_exit(void)
{
	DEBUG("Unloading drv...");
	unregister_chrdev(0, DEVICE_NAME);
	DEBUG("Driver %s unloaded", DEVICE_NAME);
}

module_init(drv_init);
module_exit(drv_exit);
