#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include "spirit_msg.h"

#define DEVICE_NAME "spiritko"
#define DEVICE_CLASS "spirit_class"
//static const char * DEVICE_NAME = "spdrv";
#define DRIVER_VERSION "v1.0"
#define DEVICE_MAJOR 0

#define MAGIC 0xF239

#define IOCTL_SPIRIT_MSG		_IOWR(MAGIC, 0, SpiritMsg)

#define DEBUG(format, arg...) printk(KERN_INFO "SPIRIT MSG: " format "\n", ## arg)
#define ERROR(format, arg...) printk(KERN_INFO "SPIRIT ERR: " format "\n", ## arg)

MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERSION);
MODULE_DESCRIPTION("Spirit test");

static spinlock_t m_lock;

static int opened_cnt = 0;
static int dev_major = DEVICE_MAJOR;

static struct cdev cdev;

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
	int hep = 0;
	long long chan = 0x1234567890ABCDEFLL;
	spin_lock(&m_lock);

	switch (ioctl_num)
    {
		case IOCTL_SPIRIT_MSG:
		{
			result = copy_from_user(&spmsg, (SpiritMsg*)param, sizeof(SpiritMsg));
			if (result == 0) {
				DEBUG("title = %d", spmsg.title);
				result = copy_from_user(&hep, (int*)spmsg.data, spmsg.size);
				if (result == 0) {
					DEBUG("hep = %d", hep);
				} else {
					ERROR("Cannot retrive hep");
				}
				spmsg.size = sizeof(chan);
				//spmsg.data = &chan;
				if (copy_to_user((long long*)spmsg.data, &chan, sizeof(chan)) == 0) {
					if (copy_to_user((SpiritMsg*)param, &spmsg, sizeof(SpiritMsg))) result = -EINVAL;
				} else {
					ERROR("Cannot copy chan");
					result = -EINVAL;
				}
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

void device_class_release(struct class *cd)
{
	DEBUG("Device class released: %s", cd->name);
    return;
}

static struct class device_class = {
    .name = DEVICE_CLASS,
    .class_release = device_class_release
};

static int __init drv_init(void)
{
	//int result;
	spin_lock_init(&m_lock);

	DEBUG("Loading drv...");

	//if (class_register(&device_class) != 0) ERROR("Failed to register device class!");

	dev_major = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &dev_fops);

	if (dev_major < 0) {
		ERROR("Register chrdev failed");
	} else {
		DEBUG("Driver successfully loaded major = %d", dev_major);
	}

	cdev_init(&cdev, &dev_fops);
    cdev_add(&cdev, MKDEV(dev_major,0), 1);

	return 0;
}

static void __exit drv_exit(void)
{
	DEBUG("Unloading drv...");
	cdev_del(&cdev);
	unregister_chrdev(dev_major, DEVICE_NAME);
	//class_unregister(&device_class);
	DEBUG("Driver %s unloaded", DEVICE_NAME);
}

module_init(drv_init);
module_exit(drv_exit);
