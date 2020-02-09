/////////////////////Assignment1////////////////////////////////////////////////
 #include<linux/init.h>  		 /*KERNEL HEADERS*/
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/random.h>
#include<linux/uaccess.h>
#include"adc.h"


static  dev_t first; //GLOBAL VARIABLES FOR FIRST DEVICE NUMBER
static struct cdev c_dev;
static struct class *cls;



static int my_open(struct inode *i,struct file *f)
	{
		printk(KERN_INFO "mychar: file_open()\n");
		return 0;
	}


int fetch_adc_val(void)
{
  
	unsigned int adc_val2 = 0;
 	get_random_bytes(&adc_val2, 2);
	adc_val2&= 0x03FF;
	adc_val2&= 0xFFC0;
   	return adc_val2;  
}

static ssize_t my_read(struct file *f, char __user *ioctl_param, size_t len, loff_t *off )//No. of files read; __user: then buf knows buf is goint to untrusted area(user space  
	{
		printk(KERN_INFO "mychar: file_read()\n");
		int adc_val;
   		args q;
		if(copy_from_user(&q,(args *)ioctl_param,sizeof(args))){
      		return -1;
		}
			
				if(q.val1==1){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC1>: %d\n",adc_val);
				}
				else if(q.val1==2){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC2>: %d\n",adc_val);
				}
				else if(q.val1==3){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC3>: %d\n",adc_val);
				}
				else if(q.val1==4){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC4>: %d\n",adc_val);
				}
				else if(q.val1==5){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC5>: %d\n",adc_val);
				}
				else if(q.val1==6){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC6>: %d\n",adc_val);
				}
				else if(q.val1==7){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC7>: %d\n",adc_val);
				}
				else if(q.val1==8){
				adc_val=fetch_adc_val();
         			printk(KERN_INFO "<channel ADC8>: %d\n",adc_val);
				}
				else{
					printk(KERN_INFO "<channel ADC8 should be between 1 and 8>:\n");
				}
  		q.val2=adc_val;
   		if(copy_to_user((args *)ioctl_param,&q,sizeof(args))){
 			return -1;
		}
		return 0;
	}

long my_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
   int dummy_read;
   pr_info("Inside IOCTL Function\n");
   switch (ioctl_num)
   {
      case ADC_CHANNEL:
         dummy_read =  my_read(file, (char *)ioctl_param, 100, 0);
         break;

      default:
         pr_err("Invalid command entered\n");
         return -1;
   }
   return 0;
}






static int my_close(struct inode *i,struct file *f)
	{
		printk(KERN_INFO "mychar: file_close()\n");
		return 0;
	}


static struct file_operations fops = {
   .owner          = THIS_MODULE,
   .open           = my_open,
   .read           = my_read,
   .unlocked_ioctl = my_ioctl,
   .release        = my_close
};







static int  __init mychar_init(void)  //CONSTRUCTOR	
{
	printk(KERN_INFO "Hey!!!! ADC8 Driver registered\n\n");

		///////////////////step1: For Dynamiccaly allocating Major and Minor Numbers/////////////////////
	if (alloc_chrdev_region(&first,0,1,"BITS-PILANI_HYD")<0)//FIRST-Global address of Global dev_t variable.0 -Minor Number count from 0 
											
	 {
		return -1;
	}
	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first),MINOR(first));




/////////////////////////*step2: For Creating Device Node*//////////////////////
	if((cls=class_create(THIS_MODULE, "chardrv"))==NULL)//getting cls from module "chardev"	
	{
		unregister_chrdev_region(first,1);
		return -1;
	}
	if(device_create(cls, NULL,first, NULL, "adc8" )==NULL)//This name "adc8" appears in dev directory.
	{
		class_destroy(cls);	//if condition satisfies then destroy the class and remove major and minor number
		unregister_chrdev_region(first,1);
		return -1;
	}



////////////////////////Step3: Link f_operations and c_dev structs to device node///////////////////////////
	cdev_init(&c_dev,&fops);
 
	if(cdev_add(&c_dev,first,1)==-1)
	{
		device_destroy(cls,first);
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}
	
	return 0;

}


static void __exit mychar_exit(void) //DESTRUCTOR
{
	cdev_del(&c_dev);	
        device_destroy(cls,first);
	class_destroy(cls);
	unregister_chrdev_region(first,1);
	printk(KERN_INFO "mychar unregistered");
}

module_init(mychar_init);//WHATEVER ARGUEMENTS WE PASS, THOSE BECOMES ENTRY FUNCTION HERE
module_exit(mychar_exit);//WHATEVER ARGUEMENTS WE PASS, THOSE BECOMES EXIT FUNCTION HERE

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MITILESH");
MODULE_DESCRIPTION("ADC8 Character Driver");

