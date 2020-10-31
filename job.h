#ifndef __JOB_H
#define __JOB_H

void start_periodic_job(void (*job)(void), unsigned long period);
void stop_periodic_job(void);

#endif // __JOB_H
