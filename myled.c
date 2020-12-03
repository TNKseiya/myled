// SPDX-License-Identifier: GPL-2.0
// /*
//  * Copyright (C) 2020 Ryuichi Ueda and　Seiya Tanaka.  All rights reserved.


#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/io.h>
#include<linux/delay.h>

MODULE_AUTHOR("Ryuichi Ueda and Seiya Tanaka");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* flip, const char* buf, size_t count, loff_t* pos)
{
        char c;
        int i;
        if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

  	printk(KERN_INFO "receive %c\n",c);
	if(c == '0'){				
		for(i=0;i<10;i++){			
			gpio_base[7] = 1<< 19;				
			gpio_base[7] = 1<< 8;				
			gpio_base[7] = 1<< 25;				
			ssleep(0.2);	
			gpio_base[10] = 1<< 19;				
			gpio_base[10] = 1<< 8;				
			gpio_base[10] = 1<< 25;		
			ssleep(0.5);	
		}		
	}else if(c == '1'){
		gpio_base[7] = 1 << 19;		
		ssleep(1);			
		gpio_base[10] = 1 << 19;				
		gpio_base[7] = 1 << 8;				
		ssleep(1);		      
		gpio_base[7] = 1 << 19;		
		ssleep(1);			
		gpio_base[10] = 1 << 19;				
		gpio_base[10] = 1 << 8;				
		gpio_base[7] = 1 << 25;			
		ssleep(1);		
		gpio_base[7] = 1 << 19;				
		ssleep(1);		
		gpio_base[10] = 1 << 19;				
		gpio_base[7] = 1 << 8;				
		ssleep(1);				
		gpio_base[7] = 1 << 19;		
		ssleep(1);			
		for(i=0; i<3; i++){				
			gpio_base[7] = 1 << 19;				
			gpio_base[7] = 1 << 8;				
			gpio_base[7] = 1 << 25;				
			gpio_base[7] = 1 << 4;				
			ssleep(1);
			gpio_base[10] = 1 << 19;				
			gpio_base[10] = 1 << 8;			
			gpio_base[10] = 1 << 25;				
			gpio_base[10] = 1 << 4;			
			ssleep(1);	
		}		
	}				
	return 1;				
}
				
static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)				
{				
	int size = 0;				
	char sushi[] = {'s','u','s','h','i'};
				
	if(copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){				
		printk(KERN_INFO "sushi : copy_to_user failed\n");		
		return -EFAULT;
	}
				
	size += sizeof(sushi);
				
	return size;
				
}
						
static struct file_operations led_fops = {				
	.owner = THIS_MODULE,				
	.write = led_write,				
	.read = sushi_read				
};
							
static int __init init_mod(void)				
{				
	int retval;				
	
  	printk(KERN_INFO "%s is loaded.\n",__FILE__);				
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
				
	if(retval < 0){				
		printk(KERN_ERR "alloc_chrdev_region failde.\n");				
		return retval;				
	}
				
	printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
								
	cdev_init(&cdv, &led_fops);				
	retval = cdev_add(&cdv, dev, 1);
				
	if(retval < 0){				
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d\n",MAJOR(dev),MINOR(dev));				
		return retval;				
	}
				
	cls = class_create(THIS_MODULE,"myled");
				
	if(IS_ERR(cls)){				
		printk(KERN_ERR "class_create failed.");				
		return PTR_ERR(cls);
	}
								
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
								
	gpio_base = ioremap_nocache(0x3f200000, 0xA0);
								
	const u32 led =25;				
	const u32 led2=8;				
	const u32 led3=19;			
	const u32 sup =4;				
	const u32 index = led/10;				
	const u32 shift = (led%10)*3;				
	const u32 mask = ~(0x7 << shift);				
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);				
	const u32 index2 = led2/10;			
	const u32 shift2 = (led2%10)*3;				
	const u32 mask2 = ~(0x7 << shift2);		
	gpio_base[index2] = (gpio_base[index2] & mask2) | (0x1 << shift2);				
	const u32 index3 = led3/10;		
	const u32 shift3 = (led3%10)*3;				
	const u32 mask3 = ~(0x7 << shift3);				
	gpio_base[index3] = (gpio_base[index3] & mask3) | (0x1 << shift3);			
	const u32 index4 = sup/10;				
	const u32 shift4 = (sup%10)*3;				
	const u32 mask4 = ~(0x7 << shift4);			
	gpio_base[index4] = (gpio_base[index4] & mask4) | (0x1 << shift4);

	return 0;				
}
			
static void __exit cleanup_mod(void)				
{				
	cdev_del(&cdv);				
	device_destroy(cls, dev);				
	class_destroy(cls);				
	unregister_chrdev_region(dev, 1);				
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));				
}
								
module_init(init_mod);				
module_exit(cleanup_mod);

