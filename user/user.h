struct stat;
struct sysinfo;

// system calls*********

//new son_process
int fork(void);
//end status
int exit(int) __attribute__((noreturn));
//wait for son_process and get status
int wait(int*);
//create pipe
int pipe(int*);
//
int write(int, const void*, int);
//
int read(int, void*, int);
//close file_signal
int close(int);
//end int_process
int kill(int);
//replace space and execute exe
int exec(const char*, char**);
//open or create file
int open(const char*, int);
//create file_system node
int mknod(const char*, short, short);
//delete file
int unlink(const char*);
//get file_signal for file_status
int fstat(int fd, struct stat*);
//create strong link
int link(const char*, const char*);
//create dirc
int mkdir(const char*);
//change now_dirc
int chdir(const char*);
//copy file_signal
int dup(int);
//
int getpid(void);
//adjust data_capacity and distribute memory
char* sbrk(int);
//
int sleep(int);
//get system_time from start
int uptime(void);
//trace
int trace(int);


// ulib.c**********user lib

/*get order file_status
struct stat {
    dev_t     st_dev;     // 文件的设备ID 
    ino_t     st_ino;     // inode号 
    mode_t    st_mode;    // 文件类型和模式 
    nlink_t   st_nlink;   // 硬链接数 
    uid_t     st_uid;     // 所有者用户ID 
    gid_t     st_gid;     // 所有者组ID 
    dev_t     st_rdev;    // 设备ID（如果这是一个特殊文件） 
    off_t     st_size;    // 文件大小，以字节为单位 
    blksize_t st_blksize; // 文件系统I/O块大小 
    blkcnt_t  st_blocks;  // 已分配的512字节块数 
    time_t    st_atime;   // 最后访问时间 
    time_t    st_mtime;   // 最后修改时间 
    time_t    st_ctime;   // 最后状态更改时间
};*/
int stat(const char*, struct stat*);
//copy string
char* strcpy(char*, const char*);
//safely copy memory area
void *memmove(void*, const void*, int);
//find char from string
char* strchr(const char*, char c);
//compare string
int strcmp(const char*, const char*);
//stand output to file_signal
void fprintf(int, const char*, ...);
//stand output
void printf(const char*, ...);
//read line from stand_input
char* gets(char*, int max);
//get string_len
uint strlen(const char*);
//set memory_area number
void* memset(void*, int, uint);
//distribu memory
void* malloc(uint);
//free memory
void free(void*);
//string to int
int atoi(const char*);
//compare memory_area
int memcmp(const void *, const void *, uint);
//copy memory_area
void *memcpy(void *, const void *, uint);
//sysinfo
int sysinfo(struct sysinfo*);

