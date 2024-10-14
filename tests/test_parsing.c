/**
 * @file test_parsing.c
 * @brief Unit tests
 * @author Lilian VERLHAC
 * @version 1.0
 * @date 2020-04-18
 */

#include "../src/parse_procmaps.c"
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

void test_make_procmaps_array()
{
    char contents[] =
        "00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon\n"
        "00651000-00652000 r--p 00051000 08:02 173521      /usr/bin/dbus-daemon\n";
    FILE *stream = fmemopen(contents, sizeof(contents)-1 /* remove null byte */, "r");
    hr_procmaps **procmaps_array = make_procmaps_array(stream);
    assert(procmaps_array[0]);
    assert(procmaps_array[0]->addr_begin == 0x00400000);
    assert(procmaps_array[1]);
    assert(procmaps_array[1]->addr_begin == 0x00651000);
    assert(procmaps_array[2]==NULL);
    destroy_procmaps(procmaps_array);
}

void test_parse_line()
{
    char *mapstr = strdup("00400000-00452000 r-xp 00000000 08:02 173521      /usr/bin/dbus-daemon\n\0");
    char permissions = ((0 | PERMS_READ) | PERMS_EXECUTE) | PERMS_PRIVATE;
    procmaps_row_t *map = parse_procmaps_line(mapstr);

    assert(map != NULL);
    assert(map->addr_begin == 0x00400000);
    assert(map->addr_end == 0x00452000);
    assert(map->perms == permissions);
    assert(map->offset == 0);
    assert(map->dev.major == 8);
    assert(map->dev.minor == 2);
    assert(map->inode == 173521);
    assert(strcmp(map->pathname, "/usr/bin/dbus-daemon") == 0);
    free(mapstr);
    free(map->pathname);
    free(map);
}

int main(void)
{
    test_parse_line();
    test_make_procmaps_array();
    printf("All tests passed :)\n");
    return 0;
}
