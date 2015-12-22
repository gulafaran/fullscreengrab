#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "fullscreen.h"

int main() {
  Display *dpy = XOpenDisplay(NULL);

  while(1) {
    fullscreencheck(dpy);
    sleep(10);
  }

  XCloseDisplay(dpy);
  return 0;
}
