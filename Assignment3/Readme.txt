
STEPS TO EXECUTE THIS CODE-

make all 					// To build 
sudo insmod Assignment3.ko			// To insert module

sudo rmmod uas					
sudo rmmod usb_storage
		(or)				// To remove existing driver
sudo modprobe -rf usb_storage


sudo fdisk -l 					// To check registered block devices

sudo mount -t vfat /dev/name_of_directory /media/external -o uid=1000,gid=1000,utf8,dmask=027,fmask=137 	//To mount the pendrive after plugin

cd /media/external				// To go to pendrive directory

mkdir filename					// To create new directory

ls						// To get list of files that are present

rm filename					// To delete a file

echo "text words" > filename			// To edit a file with echo

rmmod Assignment3				// To remove module and only remove the pendrive

sudo umount /dev/name_of_directory		// To unmount the pendrive





