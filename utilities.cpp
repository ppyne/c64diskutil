#include <string.h>
#include <QString>

static QString humanSize(uint64_t bytes) {
    char suffix[5][3] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);

    int i = 0;
    double dblBytes = bytes;

    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i < length - 1; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    char output[200];
    sprintf(output, "%.02lf %s", dblBytes, suffix[i]);
    return output;
}
