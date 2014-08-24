#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include <asm/uaccess.h>

#include <linux/semaphore.h>
#include <linux/cdev.h>
#include <linux/kthread.h>

#include <asm/segment.h>
#include <linux/buffer_head.h>

static int Major;

struct device
{
	char array[100];
	int one;
	int two;
	int result;
	struct semaphore sem;
}char_arr;

//Dummy function to test threading
void math(int a)
{
	printk(KERN_INFO "In math thread");
	char_arr.result =  a + a;
}

//Handle file openings
struct file* file_open(const char* path, int flags, int rights)
{
	struct file* filp = NULL;
	mm_segment_t oldfs;
	int err = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	filp = file_open(path, flags, rights);
	set_fs(oldfs);
	if (IS_ERR(filp))
	{
		err = PIR_ERR(filp);
		return NULL;
	}
	return filp;
}

//File closing
void file_close(struct file* file)
{
	filp_close(file, NULL);
}

//Writing out
int file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size)
{



}

//Grab the semaphore
int open(struct inode *inode, struct file *filp)
{
	if (down_interruptible(&char_arr.sem)) //Try to hold the semaphore
	{
		printk(KERN_INFO " could not hold semaphore.");
		return -1;
	}

	return 0;
	
}

//read from the buffer
ssize_t read( struct file *filp, int* buff, size_t count, loff_t* offset)
{
	unsigned long ret;
	printk("Inside read \n");
	//char_arr.result = math(char_arr.one, char_arr.two);
	struct task_struct *thread = kthread_create(math, char_arr.one, "THREADasdf");
	wake_up_process(thread);
	printk("Result: %d \n", char_arr.result);
	ret = copy_to_user(buff, &char_arr.result, count);
	return ret;
}

//Writing to buffer
ssize_t write( struct file *filp, const int *buff, size_t count, loff_t *offp)
{
	unsigned long ret;
	printk(KERN_INFO "Inside write\n");
	int* tmp;
	ret = copy_from_user(&tmp, buff, count);
	printk("Copied %d", tmp);
	char_arr.one = tmp;
	char_arr.two = tmp;
	return ret;

}

int release(struct inode *inode, struct file *filp)
{
	printk (KERN_INFO "Inside close \n");
	printk (KERN_INFO "Releasing semaphore\n");
	up(&char_arr.sem);
	return 0;
}

//?
struct cdev *kernel_cdev;

//Struct for file operations supported by module
struct file_operations fops = { 
	read: read, 
	write: write, 
	open: open, 
	release: release,
	//stat,
	//fsync,
	//ioctl
};

int __init char_arr_init (void)
{
	int ret;
	dev_t dev_no, dev;
	kernel_cdev = cdev_alloc();
	kernel_cdev->ops = &fops;
	kernel_cdev->owner = THIS_MODULE;

	printk(" Inside init module\n");
	ret = alloc_chrdev_region( &dev_no, 0, 1, "chr_arr_dev");
	if (ret < 0)
	{
		printk("Major number allocation is failed\n");
		return ret;
	}

	Major = MAJOR(dev_no);
	dev = MKDEV(Major, 0); 
	sema_init(&char_arr.sem, 1); //Initialize the semaphor

	printk(" The major number for your device is %d\n", Major);
	ret = cdev_add ( kernel_cdev, dev, 1);
	if (ret < 0 )
	{
		printk(KERN_INFO "Unable to allocate cdev");
		return ret;
	}
	return 0;
}

static void __exit char_arr_exit(void)
{

	printk(KERN_INFO " Inside cleanup_module\n");
	cdev_del(kernel_cdev);
	unregister_chrdev_region(Major, 1);
}

MODULE_LICENSE("GPL");

module_init(char_arr_init);
module_exit(char_arr_exit);
