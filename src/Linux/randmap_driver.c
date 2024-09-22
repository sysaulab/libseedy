#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define MYDEV_NAME "mrandom"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SysauLab");

static struct cdev my_cdev;

static int mrandom_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "Successfully opened device!\n");
    return 0;
}

static int mrandom_release(struct inode *inode, struct file *filp) 
{
    printk(KERN_INFO "Device closed!\n");    
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mrandom_open,
    .release = mrandom_release,   
};

static int __init mrandom_init(void) /* Constructor */
{
    printk(KERN_INFO "Loading mrandom driver...\n");

    int err;
    err = register_chrdev(0, MYDEV_NAME, &fops);

    if(err < 0)
    {
        printk(KERN_ERR "Failed to register device!\n");
        return err;
    }

    printk(KERN_INFO "Successfully registered character device.\n");  
    return 0;
}

static void __exit mrandom_exit(void) /* Destructor */
{
    unregister_chrdev(0, MYDEV_NAME); 
    printk(KERN_INFO "Goodbye from mrandom!\n");
}

module_init(mrandom_init);
module_exit(mrandom_exit);
