Purpose: to demonstrate relevant file system functions, including detecting disks, testing disk performance, formatting disks, writing and reading files from disks, and searching for files on disks.  
Command: ./sample_fs

Note: The file system automatically detects storage media and mounts the detected media to /tmp.
********************sg_main_list***********************
* 1 : Performance test of disks
* 2 : Test of formatting disks
* 3 : Test of reading and writing files in disks
* 4 : Test of searching files in disks
* 5 : Quit
******************************************************

********************DISKFS_LIST***********************
* 1 : [DISKFS]Interface test of the system time
* 2 : [DISKFS]Interface test of the folders in disks
* 3 : [DISKFS]Interface test of the files in disks
* 4 : [DISKFS]Back
******************************************************

Explanation:
********************sg_main_list***********************
* 1 : Testing disk performance 			Write or read 40 MB data from a disk and calculate the access speed.  
* 2 : Formatting disks			Not implemented.
* 3 : Writing and reading files from disks			Jump to the following DISKFS_LIST:
* 4 : Instance of searching for files on disks			Search for the three types of files under /tmp: folder, .jpg file, and .mp3 file. Then print the output information.
* 5 : Quit
******************************************************

********************DISKFS_LIST***********************
* 1 : [DISKFS] System time interface test		Set the system date and time and query the system date and time. If the queried results are the same as configured, the interface passes the test.
* 2 : [DISKFS] Disk folder service interface test	Create, delete, or rename a folder under /tmp. Change the folder attribute, detect the folder (DIR4/DIR5), or query the folder name (DEBUG). 
* 3 : [DISKFS] Disk file service interface test	Create, delete, rename, or truncate a file under /tmp/DIR4, or change the file attribute.
* 4 : [DISKFS]Back
******************************************************
