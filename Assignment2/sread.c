#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/sched/xacct.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/fsnotify.h>
#include <linux/security.h>
#include <linux/pagemap.h>
#include <linux/splice.h>
#include <linux/compat.h>
#include <linux/mount.h>
#include <linux/fs.h>

#include <linux/uaccess.h>
#include <asm/unistd.h>

//#include <stdlib.h>

static inline loff_t file_pos_read(struct file* file) { return file -> f_pos; }
static inline void file_pos_write(struct file* file, loff_t pos) { file -> f_pos = pos; }

/* 
 * referred github.com/torvalds/linux (Linus Torvalds)
 * linux/fs/read_write.c line 542~ (define read)
 */
int sys_sread(int fd, char* buf, int len) {

	struct fd f;
	mm_segment_t old_fs;

	int ret = -EBADF;
	loff_t pos;
	int i;
	//char temp[100] = {0};
	char* temp = (char *)vmalloc(sizeof(char) * (len + 1));
	for(i = 0; i < len + 1; ++i) temp[i] = 0;

	f = fdget_pos(fd);

	if(f.file) {
		pos = file_pos_read(f.file);
		old_fs = get_fs(); set_fs(KERNEL_DS);
		ret = (int)vfs_read(f.file, (char __user *) temp, len, &pos);
		set_fs(old_fs);
	
		for(i = 0; i < len; ++i) if(temp[i] == '\0') break; else temp[i] = ~temp[i];
		copy_to_user(buf, temp, len);

		if(ret >= 0)
			file_pos_write(f.file, pos);
		fdput_pos(f);
	}
	kvfree(temp);

	return ret;
}

SYSCALL_DEFINE3(sread, int, fd, char*, buf, int, len) {
	return sys_sread(fd, buf, len);
}
