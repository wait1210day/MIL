#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    uint8_t buf;
    while (read(STDIN_FILENO, &buf, 1) > 0)
        printf("%02X ", buf);
    putchar('\n');
    /*
    for (int i = 0; i < sizeof(__moe_warakawa_showtime_hex); i++)
        putchar(__moe_warakawa_showtime_hex[i]);
    */
    return 0;
}
