#include "util.h"
#include <stdio.h>
#include <time.h>

/**
 * Converts a number of bytes into a human-readable unit string (MiB, GiB, etc).
 *
 * @param buf Destination character buffer to write the string to
 * @param buf_sz Size of the destination character buffer
 * @param size The size to convert (in bytes)
 * @param decimal_place The number of characters after the decimal place. E.g.,
 *        decimals = 3 would produce output like 4.123 GiB, and decimals = 1
 *        would produce 4.1 GiB.
*/
void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimal_place)
{
    const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
    int unit_index = 0;
    while (size >= 1024 && unit_index < sizeof(units) / sizeof(units[0]) - 1) {
        size /= 1024;
        unit_index++;
    }
    char format_string[11];
    snprintf(format_string, sizeof(format_string), "%%.%df %%s", (int)decimal_place);
    snprintf(buf, buf_sz, format_string, size, units[unit_index]);

}


/**
 * Given a UNIX timestamp, this function converts it to a human-readable string.
 * For example, 1677215473 => Feb 23 2023.
 * 
 * @param buf Destination character buffer to write the string to
 * @param buf_sz Size of the destination character buffer
 * @param time UNIX timestamp
*/
size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
  struct tm *tmp = localtime(&time);
  return strftime(buf, buf_sz, "%b %d %Y", tmp);
}
