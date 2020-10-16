// Based on https://opensource.com/article/19/4/interprocess-communication-linux-storage
#define ByteSize 512
#define BackingFile "/shMemEx"
#define AccessPerms 0644
#define SemaphoreName "mysemaphore"
#define MemContents "This is the way the world ends...\n"
