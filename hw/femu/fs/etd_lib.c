/* lib.c - Some basic library functions (printf, strlen, etc.)
 * vim:ts=4 noexpandtab */

#include "etd_lib.h"

/* void test_interrupts(void)
 * Inputs: void
 * Return Value: void
 * Function: increments video memory. To be used to test rtc */
void test_interrupts(void) {
    etd_int32_t i;
    for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        video_mem[i << 1]++;
    }
}

// TODO
void die(etd_int8_t* s) {
    cli();
    change_process_screen(view_screen);
    putc('\n');
    puts(s);
    while(1);
}

// TODO
int atoi(etd_int8_t* s, int* num) {
    int i;
    int n = 0;
    for (i = 0; '0' <= s[i] && s[i] <= '9'; ++i) {
        n = n * 10 + (s[i] - '0');
    }
    *num = n;
    return i;
}

// TODO
etd_uint32_t min(etd_uint32_t a, etd_uint32_t b) {
    if (a < b) return a;
    return b;
}
