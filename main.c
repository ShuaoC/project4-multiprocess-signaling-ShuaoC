#include "main.h"
sem_t *semS1G;
sem_t *semS2G;
sem_t *semS1R;
sem_t *semS2R;
sem_t *sAdmin;
void *xyx;

int main(int argc, char *argv[])
{
    pid_t pidSH[ProcessH] = {0u};
    pid_t pidSG[ProcessG] = {0u};
    pid_t pidSR[ProcessR] = {0u};
    int size;
    int uaddre;
    int tSH;
    int tSG;
    int tSR;
    size = 1024;
    FILE *fp;
    fp = fopen(logfile, "w");
    xyx = IM(size);
    memset(xyx, 0x00, size);
    semS1G = xyx + SemSIG1G;
    semS2G = xyx + SemSIG2G;
    semS1R = xyx + SemSIG1R;
    semS2R = xyx + SemSIG2R;
    sAdmin = xyx + Admin;
    sem_init(semS1G, 1, 1);
    sem_init(semS2G, 1, 1);
    sem_init(semS1R, 1, 1);
    sem_init(semS2R, 1, 1);
    sem_init(sAdmin, 1, 1);
    memset(xyx + PID_HANDLING, 0x00, sizeof(int) * 4);
    memset(xyx + PID_GENERATING, 0x00, sizeof(int) * 3);
    memset(xyx + PID_REPORTING, 0x00, sizeof(int));
    tSH = ProcessH;
    tSG = ProcessG;
    tSR = ProcessR;
    setMV(xyx + 100, &tSG, sizeof(tSG));
    setMV(xyx + 200, &tSH, sizeof(tSH));
    setMV(xyx + 300, &tSR, sizeof(tSR));
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);
    printf("\nFour signal handling processes were Created: \n");
    fprintf(fp, "\nFour signal handling processes were Created: \n");
    for (uaddre = 0; uaddre < tSH; uaddre++)
    {
        pidSH[uaddre] = fork();
        if (pidSH[uaddre] < 0)
        {
            exit(0);
        }
        if (pidSH[uaddre] == 0)
        {
            handler(uaddre);
        }
        printf("    Process[%d]ID = %d\n", uaddre + 1, pidSH[uaddre]);
        fprintf(fp, "    Process[%d]ID = %d\n", uaddre + 1, pidSH[uaddre]);
    }

    printf("\nOne reporting process were Created: \n");
    fprintf(fp, "\nOne reporting process were Created: \n");

    pidSR[0] = fork();

    if (pidSR[0] < 0)
    {
        exit(0);
    }
    else if (pidSR[0] == 0)
    {
        reporter();
    }
    printf("    Process[%d]ID = %d\n", 0, pidSR[0]);
    fprintf(fp, "    Process[%d]ID = %d\n", 0, pidSR[0]);
    setMV(xyx + PID_REPORTING, pidSH, sizeof(pidSH));
    printf("\nThree signal generating processes were Created: \n");
    fprintf(fp, "\nThree signal generating processes were Created: \n");

    for (uaddre = 0; uaddre < tSG; uaddre++)
    {
        pidSG[uaddre] = fork();
        if (pidSG[uaddre] < 0)
        {
            perror("Error");
            exit(0);
        }
        if (pidSG[uaddre] == 0)
        {
            generator();
        }
        printf("    Process[%d]ID = %d\n", uaddre + 1, pidSG[uaddre]);
        fprintf(fp, "    Process[%d]ID = %d\n", uaddre + 1, pidSG[uaddre]);
    }
    printf("\n----------------------Loading-----------------------------------\n");
    fprintf(fp, "\n\n--------------------------------------------------------------\n");
    fclose(fp);
    setMV(xyx + PID_GENERATING, pidSG, sizeof(pidSG));
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);
    while (1)
        ;
    return 0;
}

int getSU(int iTpye)
{
    int LV = 0u;
    if (iTpye == SIGU1G)
    {
        sem_wait(semS1G);
        getMV(xyx + SIGU1G, &LV, sizeof(int));
        sem_post(semS1G);
    }
    else if (iTpye == SIGU2G)
    {
        sem_wait(semS2G);
        getMV(xyx + SIGU2G, &LV, sizeof(int));
        sem_post(semS2G);
    }
    else if (iTpye == SIGU1R)
    {
        sem_wait(semS1R);
        getMV(xyx + SIGU1R, &LV, sizeof(int));
        sem_post(semS1R);
    }
    else if (iTpye == SIGU2R)
    {
        sem_wait(semS2R);
        getMV(xyx + SIGU2R, &LV, sizeof(int));
        sem_post(semS2R);
    }

    return LV;
}

void S1Ginc()
{
    int LV = 0u;
    sem_wait(semS1G);
    getMV(xyx + SIGU1G, &LV, sizeof(int));
    LV++;
    setMV(xyx + SIGU1G, &LV, sizeof(int));
    sem_post(semS1G);
}

void S2Ginc()
{
    int LV = 0u;
    sem_wait(semS2G);
    getMV(xyx + SIGU2G, &LV, sizeof(int));
    LV++;
    setMV(xyx + SIGU2G, &LV, sizeof(int));
    sem_post(semS2G);
}

void S1Rinc()
{
    int LV = 0u;
    sem_wait(semS1R);

    getMV(xyx + SIGU1R, &LV, sizeof(int));
    LV++;
    setMV(xyx + SIGU1R, &LV, sizeof(int));
    sem_post(semS1R);
}

void S2Rinc()
{
    int LV = 0u;
    sem_wait(semS2R);
    getMV(xyx + SIGU2R, &LV, sizeof(int));
    LV++;
    setMV(xyx + SIGU2R, &LV, sizeof(int));
    sem_post(semS2R);
}

void generator()
{
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);

    sleep(1);

    while (1)
    {
        if (rand() % 2)
        {
            S1Ginc();
            kill(0, SIGUSR1);
        }
        else
        {
            S2Ginc();
            kill(0, SIGUSR2);
        }

        float timDelay = 10000 + (rand() % 90000);
        usleep(timDelay);
    }
}

void handler(int uaddre)
{
    sleep(10);
    struct sigaction sa;
    sigset_t sigmask;
    printf("\nSignal handling...");
    sa.sa_handler = rHanler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    int iSig = uaddre % 2 ? SIGUSR1 : SIGUSR2;
    sigaction(iSig, &sa, NULL);
    printf("\nSignal [%d] handling...", iSig);
    setpgid(0, 10);
    while (1)
    {
        sigfillset(&sigmask);
        sigdelset(&sigmask, iSig);
        sigdelset(&sigmask, SIGINT);
        signal(uaddre % 2 ? SIGUSR2 : SIGUSR1, SIG_IGN);
        sigsuspend(&sigmask);
    }
}

void reporter()
{
    sleep(10);
    struct sigaction sa;
    sigset_t sigmask;
    printf("\nSignal handling...");
    sa.sa_handler = rpHanler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    while (1)
    {
        sigfillset(&sigmask);
        sigdelset(&sigmask, SIGUSR1);
        sigdelset(&sigmask, SIGUSR2);
        sigdelset(&sigmask, SIGINT);
        sigsuspend(&sigmask);
    }
}

void rHanler(int lpl)
{
    if (lpl == SIGUSR1)
    {
        S1Rinc();
    }
    else if (lpl == SIGUSR2)
    {
        S2Rinc();
    }
    else
    {
        printf("\n Signal can not verify  %d  @ %d", lpl, getpid());
    }
}

void rpHanler(int lpl)
{
    time_t sTime;
    char *tiString;
    static int xxx = 0;
    long double processTime;
    static struct SignStorage data[10];
    data[xxx % 10].EEgg = lpl;
    gettimeofday(&data[xxx % 10].TA, NULL);

    if ((lpl == SIGUSR1 || lpl == SIGUSR2) && (xxx % 10 == 0))
    {
        int iUsr_Send_1, iUsr_Send_2;
        int iUsr_Recv_1, iUsr_Recv_2;

        time(&sTime);
        tiString = ctime(&sTime);
        iUsr_Send_1 = getSU(SIGU1G);
        iUsr_Send_2 = getSU(SIGU2G);
        iUsr_Recv_1 = getSU(SIGU1R);
        iUsr_Recv_2 = getSU(SIGU2R);

        for (int i = 1; i < 10; i++)
        {
            processTime = (data[i].TA.tv_sec - data[i - 1].TA.tv_sec) * 1e6;
            processTime = (processTime + (data[i].TA.tv_usec - data[i - 1].TA.tv_usec)) * 1e-6;
        }

        printf("\nSystem timestamp:    %s", tiString);
        printf("    USR1:     Generated = %d,       Received =  %d\n", iUsr_Send_1, iUsr_Recv_1);
        printf("    USR2:     Generated = %d,       Received =  %d\n", iUsr_Send_2, iUsr_Recv_2);
        printf("Average Arrival time = %Lf \n", processTime / 9);

        FILE *fp;
        fp = fopen(logfile, "a");
        fprintf(fp, "\nSystem timestamp:    %s", tiString);
        fprintf(fp, "  USR1:     Generated = %d,       Received =  %d\n", iUsr_Send_1, iUsr_Recv_1);
        fprintf(fp, "  USR2:     Generated = %d,       Received =  %d\n", iUsr_Send_2, iUsr_Recv_2);
        fprintf(fp, "Average Arrival time = %Lf \n", processTime / 9);
        fclose(fp);
    }
    xxx++;
}

void *IM(size_t size)
{
    int pt = PROT_READ | PROT_WRITE;
    int vis = MAP_SHARED | MAP_ANONYMOUS;

    void *ptr = mmap(NULL, size, pt, vis, -1, 0);

    if (ptr)
    {
        memset(ptr, 0x00, size);
    }
    return ptr;
}

void getMV(void *ptr, void *buffer, int bufflen)
{
    if (ptr && buffer)
    {
        memcpy(buffer, ptr, bufflen);
    }
}

void setMV(void *ptr, void *req, int msglen)
{
    if (ptr && req)
    {
        memcpy(ptr, req, msglen);
    }
}