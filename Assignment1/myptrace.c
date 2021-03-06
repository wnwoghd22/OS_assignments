#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/user.h> //import user_regs_struct
#include <sys/ptrace.h>
#include <string.h>
#include <errno.h> //error handling

#ifdef __x86_64__ /* for 64bit */
#define __TABLE_LENGTH__ 323

char* syscall_table[] = {
"read", "write", "open", "close", "stat", "fstat", "lstat", "poll", "lseek",
"mmap", "mprotect", "munmap", "brk", "rt_sigaction", "rt_sigprocmask", "rt_sigreturn", "ioctl", "pread", "pwrite",
"readv", "writev", "access", "pipe", "select", "sched_yield", "mremap", "msync", "mincore", "madvise",
"shmget", "shmat", "shmctl", "dup", "dup2", "pause", "nanosleep", "getitimer", "alarm", "setitimer",
"getpid", "sendfile", "socket", "connect", "accept", "sendto", "recvfrom", "sendmsg", "recvmsg", "shutdown",
"bind", "listen", "getsockname", "getpeername", "socketpair", "setsockopt", "getsockopt", "clone", "fork", "vfork",
"execve", "exit", "wait4", "kill", "uname", "semget", "semop", "semctl", "shmdt", "msgget",
"msgsnd", "msgrcv", "msgctl", "fcntl", "flock", "fsync", "fdatasync", "truncate", "ftruncate", "getdents",
"getcwd", "chdir", "fchdir", "rename", "mkdir", "rmdir", "creat", "link", "unlink", "symlink",
"readlink", "chmod", "fchmod", "chown", "fchown", "lchown", "umask", "gettimeofday", "getrlimit", "getrusage",
"sysinfo", "times", "ptrace", "getuid", "syslog", "getgid", "setuid", "setgid", "geteuid", "getegid",
"setpgid", "getppid", "getpgrp", "setsid", "setreuid", "setregid", "getgroups", "setgroups", "setresuid", "getresuid",
"setresgid", "getresgid", "getpgid", "setfsuid", "setfsgid", "getsid", "capget", "capset", "rt_sigpending", "rt_sigtimedwait",
"rt_sigqueueinfo", "rt_sigsuspend", "sigaltstack", "utime", "mknod", "uselib", "personality", "ustat", "statfs", "fstatfs",
"sysfs", "getpriority", "setpriority", "sched_setparam", "sched_getparam", "sched_setscheduler", "sched_getscheduler", "sched_get_priority_max", "sched_get_priority_min", "sched_rr_get_interval",
"mlock", "munlock", "mlockall", "munlockall", "vhangup", "modify_ldt", "pivot_root", "_sysctl", "prctl", "arch_prctl",
"adjtimex", "setrlimit", "chroot", "sync", "acct", "settimeofday", "mount", "umount2", "swapon", "swapoff",
"reboot", "sethostname", "setdomainname", "iopl", "ioperm", "create_module", "init_module", "delete_module", "get_kernel_syms", "query_module",
"quotactl", "nfsservctl", "getpmsg", "putpmsg", "afs_syscall", "tuxcall", "security", "gettid", "readahead", "setxattr",
"lsetxattr", "fsetxattr", "getxattr", "lgetxattr", "fgetxattr", "listxattr", "llistxattr", "flistxattr", "removexattr", "lremovexattr",
"fremovexattr", "tkill", "time", "futex", "sched_setaffinity", "sched_getaffinity", "set_thread_area", "io_setup", "io_destroy", "io_getevents",
"io_submit", "io_cancel", "get_thread_area", "lookup_dcookie", "epoll_create", "epoll_ctl_old", "epoll_wait_old", "remap_file_pages", "getdents64", "set_tid_address",
"restart_syscall", "semtimedop", "fadvise64", "timer_create", "timer_settime", "timer_gettime", "timer_getoverrun", "timer_delete", "clock_settime", "clock_gettime",
"clock_getres", "clock_nanosleep", "exit_group", "epoll_wait", "epoll_ctl", "tgkill", "utimes", "vserver", "mbind", "set_mempolicy",
"get_mempolicy", "mq_open", "mq_unlink", "mq_timedsend", "mq_timedreceive", "mq_notify", "mq_getsetattr", "kexec_load", "waitid", "add_key",
"request_key", "keyctl", "ioprio_set", "ioprio_get", "inotify_init", "inotify_add_watch", "inotify_rm_watch", "migrate_pages", "openat", "mkdirat",
"mknodat", "fchownat", "futimesat", "newfstatat", "unlinkat", "renameat", "linkat", "symlinkat", "readlinkat", "fchmodat",
"faccessat", "pselect6", "ppoll", "unshare", "set_robust_list", "get_robust_list", "splice", "tee", "sync_file_range", "vmsplice",
"move_pages", "utimensat", "epoll_pwait", "signalfd", "timerfd", "eventfd", "fallocate", "timerfd_settime", "timerfd_gettime", "accept4",
"signalfd4", "eventfd2", "epoll_create1", "dup3", "pipe2", "inotify_init1", "preadv", "pwritev", "rt_tgsigqueueinfo", "perf_event_open",
"recvmmsg", "fanotify_init", "fanotify_mark", "prlimit64", "name_to_handle_at", "open_by_handle_at", "clock_adjtime", "syncfs", "sendmmsg", "setns",
"getcpu", "process_vm_readv", "process_vm_writev", "kcmp", "finit_module", "sched_setattr", "sched_getattr", "renameat2", "seccomp", "getrandom",
"memfd_create", "kexec_file_load", "bpf", "execveat"
};
#else /* for 32bit */
#define __TABLE_LENGTH__ 385

char* syscall_table[] = {
"restart_syscall", "exit", "fork", "read", "write", "open", "close", "waitpid", "creat", "link",
"unlink", "execve", "chdir", "time", "mknod", "chmod", "lchown", "break", "oldstat", "lseek",
"getpid", "mount", "umount", "setuid", "getuid", "stime", "ptrace", "alarm", "oldfstat", "pause",
"utime", "stty", "gtty", "access", "nice", "ftime", "sync", "kill", "rename", "mkdir",
"rmdir", "dup", "pipe", "times", "prof", "brk", "setgid", "getgid", "signal", "geteuid",
"getegid", "acct", "umount2", "lock", "ioctl", "fcntl", "mpx", "setpgid", "ulimit", "oldolduname",
"umask", "chroot", "ustat", "dup2", "getppid", "getpgrp", "setsid", "sigaction", "sgetmask", "ssetmask",
"setreuid", "setregid", "sigsuspend", "sigpending", "sethostname", "setrlimit", "getrlimit", "getrusage", "gettimeofday", "settimeofday",
"getgroups", "setgroups", "select", "symlink", "oldlstat", "readlink", "uselib", "swapon", "reboot", "readdir",
"mmap", "munmap", "truncate", "ftruncate", "fchmod", "fchown", "getpriority", "setpriority", "profil", "statfs",
"fstatfs", "ioperm", "socketcall", "syslog", "setitimer", "getitimer", "stat", "lstat", "fstat", "olduname",
"iopl", "vhangup", "idle", "vm86old", "wait4", "swapoff", "sysinfo", "ipc", "fsync", "sigreturn",
"clone", "setdomainname", "uname", "modify_ldt", "adjtimex", "mprotect", "sigprocmask", "create_module", "init_module", "delete_module",
"get_kernel_syms", "quotactl", "getpgid", "fchdir", "bdflush", "sysfs", "personality", "afs_syscall", "setfsuid", "setfsgid",
"_llseek", "getdents", "_newselect", "flock", "msync", "readv", "writev", "getsid", "fdatasync", "_sysctl",
"mlock", "munlock", "mlockall", "munlockall", "sched_setparam", "sched_getparam", "sched_setscheduler", "sched_getscheduler", "sched_yield", "sched_get_priority_max",
"sched_get_priority_min", "sched_rr_get_interval", "nanosleep", "mremap", "setresuid", "getresuid", "vm86", "query_module", "poll", "nfsservctl",
"setresgid", "getresgid", "prctl", "rt_sigreturn", "rt_sigaction", "rt_sigprocmask", "rt_sigpending", "rt_sigtimedwait", "rt_sigqueueinfo", "rt_sigsuspend",
"pread64", "pwrite64", "chown", "getcwd", "capget", "capset", "sigaltstack", "sendfile", "getpmsg", "putpmsg",
"vfork", "ugetrlimit", "mmap2", "truncate64", "ftruncate64", "stat64", "lstat64", "fstat64", "lchown32", "getuid32",
"getgid32", "geteuid32", "getegid32", "setreuid32", "setregid32", "getgroups32", "setgroups32", "fchown32", "setresuid32", "getresuid32",
"setresgid32", "getresgid32", "chown32", "setuid32", "setgid32", "setfsuid32", "setfsgid32", "pivot_root", "mincore", "madvise",
"getdents64", "fcntl64", "not implemented", "not implemented", "gettid", "readahead", "setxattr", "lsetxattr", "fsetxattr", "getxattr",
"lgetxattr", "fgetxattr", "listxattr", "llistxattr", "flistxattr", "removexattr", "lremovexattr", "fremovexattr", "tkill", "sendfile64",
"futex", "sched_setaffinity", "sched_getaffinity", "set_thread_area", "get_thread_area", "io_setup", "io_destroy", "io_getevents", "io_submit", "io_cancel",
"fadvise64", "not implemented", "exit_group", "lookup_dcookie", "epoll_create", "epoll_ctl", "epoll_wait", "remap_file_pages", "set_tid_address", "timer_create",
"timer_settime", "timer_gettime", "timer_getoverrun", "timer_delete", "clock_settime", "clock_gettime", "clock_getres", "clock_nanosleep", "statfs64", "fstatfs64",
"tgkill", "utimes", "fadvise64_64", "vserver", "mbind", "get_mempolicy", "set_mempolicy", "mq_open", "mq_unlink", "mq_timedsend",
"mq_timedreceive", "mq_notify", "mq_getsetattr", "kexec_load", "waitid", "not implemented", "add_key", "request_key", "keyctl", "ioprio_set",
"ioprio_get", "inotify_init", "inotify_add_watch", "inotify_rm_watch", "migrate_pages", "openat", "mkdirat", "mknodat", "fchownat", "futimesat",
"fstatat64", "unlinkat", "renameat", "linkat", "symlinkat", "readlinkat", "fchmodat", "faccessat", "pselect6", "ppoll",
"unshare", "set_robust_list", "get_robust_list", "splice", "sync_file_range", "tee", "vmsplice", "move_pages", "getcpu", "epoll_pwait",
"utimensat", "signalfd", "timerfd_create", "eventfd", "fallocate", "timerfd_settime", "timerfd_gettime", "signalfd4", "eventfd2", "epoll_create1",
"dup3", "pipe2", "inotify_init1", "preadv", "pwritev", "rt_tgsigqueueinfo", "perf_event_open", "recvmmsg", "fanotify_init", "fanotify_mark",
"prlimit64", "name_to_handle_at", "open_by_handle_at", "clock_adjtime", "syncfs", "sendmmsg", "setns", "process_vm_readv", "process_vm_writev", "kcmp",
"finit_module", "sched_setattr", "sched_getattr", "renameat2", "seccomp", "getrandom", "memfd_create", "bpf", "execveat", "socket",
"socketpair", "bind", "connect", "listen", "accept4", "getsockopt", "setsockopt", "getsockname", "getpeername", "sendto",
"sendmsg", "recvfrom", "recvmsg", "shutdown", "userfaultfd", "membarrier", "mlock2", "copy_file_range", "preadv2", "pwritev2",
"pkey_mprotect", "pkey_alloc", "pkey_free", "statx", "arch_prctl"
};
#endif

void BubbleSort(int* _cArr, int* _sArr) {
    for(int i = 0; i < __TABLE_LENGTH__ - 1; ++i)
	for(int j = 0; j < __TABLE_LENGTH__ - i - 1; ++j)
	    if(_cArr[j] < _cArr[j + 1]) {
		int temp1 = _cArr[j], temp2 = _sArr[j];
		_cArr[j] = _cArr[j + 1]; _sArr[j] = _sArr[j + 1];
		_cArr[j + 1] = temp1; _sArr[j + 1] = temp2;
	    }	
}

int debug = 0, pragma = 1;

int main(int argc, char* argv[])
{
    if(argc < 2) { printf("no arguments\n"); return 0; }

    pid_t pid;
    struct user_regs_struct regs;
    int count = 0;

    int countArr[__TABLE_LENGTH__], sortArr[__TABLE_LENGTH__];
    for(int i = 0; i < __TABLE_LENGTH__; ++i) { countArr[i] = 0; sortArr[i] = i; }

    int waitstatus;
    int syscall_entry = 1;

    pid = fork();

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
        
	//char path[30] = "/bin/";
        //strcat(path, argv[1]);

	//execl(path, argv[1], NULL); 
	//execv(path, &argv[1]); //only for sh (absolute path)

	//execvp(argv[1], &argv[1]);
	
	if(execvp(argv[1], &argv[1]) == -1) {
	    printf("execute error : %s\n", strerror(errno));
	    return 1;
	}
    }

    else if (pid > 0) {
        wait(&waitstatus);
        while (1) {
	    //ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	    //printf("SystemCall %ld called\n", regs.orig_rax);

            if (syscall_entry == 1) {
		count++;
 
		if(pragma) {
		    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
#ifdef __x86_64__
		    if(debug) printf("SystemCall %lld : %s called\n", regs.orig_rax, syscall_table[regs.orig_rax]);
		    countArr[regs.orig_rax]++;
#else
		    if(debug) printf("SystemCall %ld : %s called\n", regs.orig_eax, syscall_table[regs.orig_eax]);
		    countArr[regs.orig_eax]++;
#endif
		}

                syscall_entry = 0;
            }
            else
                syscall_entry = 1;

            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);

            wait(&waitstatus);

            if (WIFEXITED(waitstatus))
                break;
        }
    }
    else
        printf("fork error\n");

    printf("Total number of syscalls: %d\n", count);

    if(pragma) {
	BubbleSort(countArr, sortArr);
	for(int i = 0; i < __TABLE_LENGTH__; ++i) {
		if(!countArr[i]) break;
		printf("%d %s\n", countArr[i], syscall_table[sortArr[i]]);
	}
    }

    return 0;
}
