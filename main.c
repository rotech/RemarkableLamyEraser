#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

#include "screenlocations.h"

#define PEN_DEVICE      "/dev/input/event1"
#define TOUCH_DEVICE    "/dev/input/event2"


void writeEvent(int fd, struct input_event event)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  write(fd, &event, sizeof(struct input_event));
}

int main(int argc, char *argv[]) {
    struct input_event ev_pen;
    int fd_pen, fd_touch;

    char name[256] = "Unknown";

    printf("RemarkableLamyEraser 1.1\n");

    /* Open Device: Pen */
    fd_pen = open(PEN_DEVICE, O_RDWR);
    if (fd_pen == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        exit(EXIT_FAILURE);
      }
    /* Open Device: Touch */
    fd_touch = open(TOUCH_DEVICE, O_WRONLY);
    if (fd_touch == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        exit(EXIT_FAILURE);
      }


    /* Print Device Name */
    ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
    printf("Using Devices:\n");
    printf("1. device file = %s\n", PEN_DEVICE);
    printf("   device name = %s\n", name);
    ioctl(fd_touch, EVIOCGNAME(sizeof(name)), name);
    printf("2. device file = %s\n", TOUCH_DEVICE);
    printf("   device name = %s\n", name);

    for (;;) {
        const size_t ev_pen_size = sizeof(struct input_event);
        read(fd_pen, &ev_pen, ev_pen_size); //note: read pauses until there is data

        if (ev_pen.code == BTN_STYLUS) {
            ev_pen.code = BTN_TOOL_RUBBER; //value will follow the button, so we can reuse the message
            writeEvent(fd_pen, ev_pen);
        }
      }
    return EXIT_SUCCESS;
}
