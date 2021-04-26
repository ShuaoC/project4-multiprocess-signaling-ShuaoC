#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>

#define logfile "log.txt"
#define RUN_TIME 1000
#define ProcessH 4
#define ProcessG 3
#define ProcessR 1
#define SIGU1G 0
#define SIGU2G 4
#define SIGU1R 8
#define SIGU2R 12
#define PID_GENERATING 100
#define PID_HANDLING 200
#define PID_REPORTING 300
#define SemSIG1G 1000
#define SemSIG2G 1032
#define SemSIG1R 1064
#define SemSIG2R 1096
#define Admin 1128

struct SignStorage
{
    int EEgg;
    struct timeval TA;
} SignStorage;

void *IM(size_t size);
void getMV(void *ptr, void *buffer, int bufflen);
void setMV(void *ptr, void *req, int msglen);
int getSU(int iTpye);
void S1Ginc();
void S2Ginc();
void S1Rinc();
void S2Rinc();
void rHanler(int);
void rpHanler(int signo);
void generator(void);
void handler(int index);
void reporter(void);
