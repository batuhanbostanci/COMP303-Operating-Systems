#define ProjectId 324
#define PathName  "que.h" /* any existing, accessible file would do */
#define MsgLen    10
#define MsgCount  6

typedef struct { 
  long type;                 /* must be of type long */ 
  char payload[MsgLen + 1];  /* bytes in the message */  
} queuedMessage;


#define SemRead "mysemaphore"
#define SemWrite "secondsemaphore"