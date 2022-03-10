#include "obdisk.h"


// ob_listdir
// List filesystem directory with optional wildcard filter
//
void ob_listdir(FILE *fd, char *filearg, bool verbose)
{
    int count = 0;
    int totalsize = 0;

    // Callback to print a directory entry
    //
    void print_dir(direntry_t *d)
    {
        fileheader_t hdr;
        uint32_t sz;
        uint32_t dd, mm, yy, dt;
        uint32_t hh, mn, ss, tm;

        // Get file header from sector stored in directory
        ob_get_fileheader(fd, d->adr, &hdr);

        // Calculate file size
        sz = ob_get_filesize(&hdr);
        count ++;
        totalsize += sz;

        // Calculate file date
        dt = hdr.dt;
        dd = dt % 32; dt >>= 5;
        mm = dt % 16; dt >>= 4;
        yy = dt;

        // Calculate file time
        tm = hdr.tm;
        ss = tm % 64; tm >>= 6;
        mn = tm % 64; tm >>= 6;
        hh = tm;

        printf("%-32s %8d  %02d.%02d.%02d  %02d:%02d:%02d\n", 
            d->name, sz, dd, mm, yy, hh, mn, ss);
    }

    // Traverse the directory tree starting at its root
    ob_traverse(fd, dir_root_sector, filearg, print_dir);

    if (verbose)
    {
        printf("> %d byte%s total in %d file%s.\n", 
            totalsize,
            (totalsize != 1) ? "s" : "", 
            count,
            (count != 1) ? "s" : ""
        );
    }
}