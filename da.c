#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "logger.h"
#include <string.h>
#include "elist.h"
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
 #include <stdint.h>
 #include "util.h"

struct da_options {
    bool sort_by_time;
    unsigned int limit;
    char *directory;
};

struct da_options options = { false, 0, "." };

struct FileInfo {
    char fileName[100];
    size_t memory;
    time_t last_accessed;
};

/* Forward declarations: */
void print_usage(char *program_name);

void print_usage(char *program_name) {
fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", program_name);

fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

fprintf(stderr, "Options:\n"
"    * -a              Sort the files by time of last access (descending)\n"
"    * -h              Display help/usage information\n"
"    * -l limit        Limit the output to top N files (default=unlimited)\n"
"    * -s              Sort the files by size (default, ascending)\n\n"
);
}

int time_comparator(const void *a, const void *b) 
{
    struct FileInfo *file_a = (struct FileInfo *) a;
    struct FileInfo *file_b = (struct FileInfo *) b;
    if (file_a->last_accessed == file_b->last_accessed) 
    {
        return strcmp(file_a->fileName, file_b->fileName);
    }

    return file_a->last_accessed > file_b->last_accessed;
}

int size_comparator(const void *a, const void *b) 
{
    struct FileInfo *file_a = (struct FileInfo *) a;
    struct FileInfo *file_b = (struct FileInfo *) b;
    if (file_a->memory == file_b->memory) 
    {
        return strcmp(file_b->fileName, file_a->fileName) * -1;
    }
    return file_a->memory < file_b->memory;
}

int traverse_directory(struct da_options *opts,  char *directory, struct elist *list) 
{
    DIR *dir;
    dir = opendir(directory);
    if (dir == NULL) 
    {
        perror("opendir");
        return 1;
    }
    
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) 
    {   
        char *buf = malloc(strlen(directory) + strlen(entry->d_name) + 2);
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0) {
                free(buf);
                continue;
            }
        if (entry->d_type != DT_DIR) 
        {
           
            struct stat sb;
            sprintf(buf, "%s/%s", directory, entry->d_name);
            stat(buf, &sb);
            
            struct FileInfo * file = malloc(sizeof(struct FileInfo)); 
            memcpy(file->fileName, buf, sizeof(*buf) + 80);
            file->memory = sb.st_size; 
            file->last_accessed = sb.st_atime;

            elist_add(list, file);
            free(file);


        } 
        else if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            //buf = malloc(strlen(directory) + strlen(entry->d_name) + 2);
            sprintf(buf, "%s/%s", directory, entry->d_name);
            traverse_directory(opts, buf, list);
            
        }
        free(buf);
    }
  
    
    closedir(dir);
    return 0;
}


int main(int argc, char *argv[])
{
    struct da_options opts;
    opts = options;
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
            case 'a':
                options.sort_by_time = true;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case 's':
                options.sort_by_time = false;
                break;
            case 'l': {
                /*    ^-- to declare 'endptr' here we need to enclose this case
                 *    in its own scope with curly braces { } */
                char *endptr;
                long llimit = strtol(optarg, &endptr, 10);
                if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                    fprintf(stderr, "Invalid limit: %s\n", optarg);
                    print_usage(argv[0]);
                    return 1;
                }
                options.limit = (int) llimit;
                break;
                }
            case '?':
                if (optopt == 'l') {
                    fprintf(stderr,
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n", optopt);
                }
                print_usage(argv[0]);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
    }

    LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);

    struct elist *list = elist_create(10, sizeof(struct FileInfo));

    traverse_directory(&opts, options.directory, list);

    if (options.sort_by_time == true) 
    {
        elist_sort(list, time_comparator);
    }
    else 
    {
        elist_sort(list, size_comparator);
    }
    if (options.limit > 0) 
    {
        int i = options.limit - 1;
        while (i >= 0) 
        {
            struct FileInfo *file = elist_get(list, i);
            char size_buf[20];
            human_readable_size(size_buf, sizeof(size_buf), file->memory, 1);
            char date_buf[30];
            simple_time_format(date_buf, sizeof(date_buf), file->last_accessed);
            printf("%10s | %11s | %s\n", size_buf, date_buf, file->fileName);
            i--;
        }
    }
    else 
    {   
        int i = elist_size(list) - 1;
        while (i >= 0) 
        {
            struct FileInfo *file = elist_get(list, i);
            char size_buf[20];
            human_readable_size(size_buf, sizeof(size_buf), file->memory, 1);
            char date_buf[30];
            simple_time_format(date_buf, sizeof(date_buf), file->last_accessed);
            printf("%10s | %11s | %s\n", size_buf, date_buf, file->fileName);
            i--;
        }
    }
    elist_destroy(list);
    // free(list);
    return 0;
}