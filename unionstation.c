/*
 * unionstation.c 
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


#define DRIVER_AUTHOR "Zach Childers"
#define DRIVER_DESC "Hello world"


static int __init union_station_init()
 {
 	printk (KERN_INFO "Hello \n");

 	return 0;
 }

static void __exit union_station_exit()
 {
 	printk(KERN_INFO "Goodbye\n");
 }

module_init(union_station_init);
module_exit(union_station_exit);

MODULE_LICENSE("GPL");