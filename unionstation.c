/*
 * unionstation.c 
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define procfs_name "unionstation"

#define DRIVER_AUTHOR "Zach Childers"
#define DRIVER_DESC "Hello world"


struct proc_dir_entry *Our_Proc_File;

static int procfile_read(char *buffer, char **buffer_location,
					off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;

	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

	if (offset > 0)
	{
		ret = 0;
	}
	else
	{
		ret = sprintf(buffer, "HelloWorld!\n");
	}
	return ret;

}

static int __init union_station_init(void)
{
 	printk (KERN_INFO "Hello \n");
 	Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);
 
 	if (Our_Proc_File == NULL)
 	{
 		remove_proc_entry(procfs_name, &proc_root);
 		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
 			procfs_name);
 		return -ENOMEM;
 	}

 	Our_Proc_File->read_proc = procfile_read;
 	Our_Proc_File->owner = THIS_MODULE;
 	Our_Proc_File->mode = S_IFREG | S_IRUGO;
 	Our_Proc_File->uid = 0;
 	Our_Proc_File->gid = 0;
 	Our_Proc_File->size = 37;

 	printk(KERN_INFO "/proc/%s created\n", procfs_name);

 	return 0;
}

static void __exit union_station_exit(void)
{
 	printk(KERN_INFO "Goodbye\n");
 	remove_proc_entry(procfs_name, &proc_root);

}

module_init(union_station_init);
module_exit(union_station_exit);

MODULE_LICENSE("GPL");



/* Lazy initialization  
   buffer file info to cache 
   the data you need
   look at open()
   all of the callbacks
*/