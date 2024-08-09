#ifndef FLAGS_H
#define FLAGS_H

#define FLAGS_ttl 5
#define FLAGS_cleaner_timeout 2
#define FLAGS_log_file "capturelogs.txt"

#ifdef __linux__
    #define FLAGS_log_path "/tmp"
#endif

#endif
