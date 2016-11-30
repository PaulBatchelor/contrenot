#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <soundpipe.h>
#include <sporth.h>
#include <math.h>

#include "wtable.h"

#define SERIALPORT "/dev/ttyACM0"

#define BAUDRATE B115200

#define FSR_MAX 1
#define SP_MAX 26100

#ifndef max
#define max(A, B) ((A > B) ? A : B)
#endif

#ifndef min
#define min(A, B) ((A < B) ? A : B)
#endif

int plumber_set_var(plumber_data *pd, char *name, SPFLOAT *var);

typedef struct {
    struct termios toptions;
    int fd;
    speed_t brate;
    unsigned char b[4];
    SPFLOAT sp, fsr, rot, prot;
    sp_ftbl *wtable;
    sp_tabread *wt;
} serial_d;

void do_bitwise_magic(unsigned char *b, int16_t *sp, int16_t *fsr, char *rot)
{
    *sp = ((b[0] - 128) << 9) +
        (b[1] << 2) +
        (b[2] >> 5);
    
    *fsr = ((b[2] & 31) << 5) + 
        (b[3] >> 2);

    *rot = b[3] & 1;
}

static int serial_data_init(serial_d *sd, const char *portname)
{
    sd->fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (sd->fd == -1) {
        fprintf(stderr, "Serial: unable to open port\n");
        return -1;
    }

    if (tcgetattr(sd->fd, &sd->toptions) < 0) {
        fprintf(stderr, "Serial: Couldn't get term attributes\n");
        return -1;
    }

    sd->brate = BAUDRATE;

    cfsetispeed(&sd->toptions, sd->brate);
    cfsetospeed(&sd->toptions, sd->brate);
    /* 8N1 */
    sd->toptions.c_cflag &= ~PARENB;
    sd->toptions.c_cflag &= ~CSTOPB;
    sd->toptions.c_cflag &= ~CSIZE;
    sd->toptions.c_cflag |= CS8;
    /* no flow control */
    sd->toptions.c_cflag &= ~CRTSCTS;

    /* turn on READ & ignore ctrl lines */
    sd->toptions.c_cflag |= CREAD | CLOCAL;
    /* turn of s/w flow ctrl */
    sd->toptions.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* make raw*/
    sd->toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | IXANY);
    sd->toptions.c_oflag &= ~OPOST;

    /* see: http://unixwiz.net/techtips/termios-vmin-vtime.html */
    sd->toptions.c_cc[VMIN] = 0;
    sd->toptions.c_cc[VTIME] = 20;

    sd->b[0] = 0;
    sd->b[1] = 0;
    sd->b[2] = 0;
    sd->b[3] = 0;

    if (tcsetattr(sd->fd, TCSANOW, &sd->toptions) < 0) {
        close(sd->fd);
        fprintf(stderr, "Serial: Couldn't set term attributes\b");
        return -1;
    }

    return 0;
}


#ifdef STANDALONE

int main()
{
    serial_d sd;

    if(serial_data_init(&sd, SERIALPORT)) {
        return -1;
    }

    unsigned char b[4];
    ssize_t bytes;
    int16_t sp = 0;
    int16_t fsr = 0;
    char rot = 0; 
    while(1) {
        bytes = read(sd.fd, b, 4);
        if(bytes >= 4 && (b[0] & 128)) {
        //if(bytes >= 4) {
        //if((b[0] & 128)) {
            do_bitwise_magic(b, &sp, &fsr, &rot);
            sd.sp = (SPFLOAT)sp / SP_MAX;
            sd.fsr = (SPFLOAT)fsr / FSR_MAX;
            sd.rot = rot;
            printf("%g\t%g\t%g\t%g\n",floor(sd.sp * 50), sd.sp, sd.fsr, sd.rot);
        } 
        usleep(100);
    }
    return 0; 
}

#endif

SPFLOAT calculate_note(SPFLOAT pos) {


    SPFLOAT weight = floor(pos * 1024);
    int index = (int) weight;
    int voice = wtable[(int)weight];

    if(voice == 25) return 0.0;
    wentry *entry = &wpositions[voice];

    /* get position relative to space */
    int rel_pos = index - entry->start;
    /* get total size */
    int size = entry->end - entry->start;

    SPFLOAT scale = 0.5 * ((2 * ((SPFLOAT) rel_pos / size)) - 1);

    //printf("start %d end %d pos %d \n", 
    //    entry->start, entry->end, index - entry->start);
    return voice + scale;
}

int sporth_contrenot(plumber_data *pd, sporth_stack *stack, void **ud)
{
    unsigned char b[4];
    ssize_t bytes;
    serial_d *sd;
    char *str;
    int16_t sp = 0;
    int16_t fsr = 0;
    char rot = 0; 
    int i;
    int nsize;
    char pnote;

    SPFLOAT weight = 0;
    switch(pd->mode) {
        case PLUMBER_CREATE: 
            sd = malloc(sizeof(serial_d));

            if(sporth_check_args(stack, "sss") != SPORTH_OK) {
                stack->error++;
                fprintf(stderr, "Wrong args for contrenot!\n");
            }

            if(serial_data_init(sd, SERIALPORT)) {
                return PLUMBER_NOTOK;
            }
            
            *ud = sd;
            plumber_ftmap_delete(pd, 0);

            /* rot */
            str = sporth_stack_pop_string(stack);
            plumber_set_var(pd, str, &sd->rot);

            /* fsr */
            str = sporth_stack_pop_string(stack);
            plumber_set_var(pd, str, &sd->fsr);

            /* soft pot */

            str = sporth_stack_pop_string(stack);
            plumber_set_var(pd, str, &sd->sp);


            plumber_ftmap_delete(pd, 1);

            /* calculate sizes */
            nsize = 0;
            pnote = wtable[0];
            wpositions[pnote].start = 0;
            for(i = 0; i < WTSIZE; i++) {
                nsize++;
                if(wtable[i] != pnote && wtable[i] != 25) {
                    wsize[pnote] = nsize;
                    nsize = 1;
                    wpositions[pnote].end = i;
                    wpositions[wtable[i]].start = i;
                } else if(wtable[i] == 25) {
                    break;
                }
                
                pnote = wtable[i];
            }

            break;
        case PLUMBER_INIT: 
            sporth_stack_pop_string(stack);
            sporth_stack_pop_string(stack);
            sporth_stack_pop_string(stack);
            break;
        case PLUMBER_COMPUTE: 
            sd = *ud;
            bytes = read(sd->fd, b, 4);
            if(bytes >= 4 && (b[0] & 128)) {
                do_bitwise_magic(b, &sp, &fsr, &rot);
                sd->sp = min(max(
                    ((SPFLOAT)sp / SP_MAX), 0.0), 1.0);
                //sd->wt->index = 1 - sd->sp;
                //sp_tabread_compute(pd->sp, sd->wt, NULL, &weight);
                sd->sp = exp(((1 - sd->sp) * 7.0) - 7);
                weight = floor(sd->sp * 1024);
                sd->fsr = fsr;
                //printf("sp: %g weight: %g computed: %d fsr: %g\n", 
                //    floor(sd->sp * 40), 
                //    weight,
                //    wtable[(int)weight], sd->fsr);
                //sd->sp = wtable[(int)weight];
                sd->sp = calculate_note(sd->sp);
                sd->rot = rot;
                //printf("%g %g %g\n", sd->sp, sd->fsr, sd->rot);
            } 
            break;
        case PLUMBER_DESTROY: 
            sd = *ud;
            free(sd); 
            break;
    }

    return PLUMBER_OK;
}

plumber_dyn_func sporth_return_ugen()
{
    return sporth_contrenot;
}
