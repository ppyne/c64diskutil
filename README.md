# c64diskutil

Commodore 64 disk image utility.

C++ Qt6 (Qt 6.2.4 LTS) project under development (2022-08-16).

The purpose of this utility is to make it possible to manage Commodore 64 disk images (.d64, .d71 and .d81), to collect technical information from the disk image and the files that are stored on it, to manage them. Geos disks and files are recognized and supported.

The project is developped on macOS Monterey (12.1), but should compile and run fine on other platforms, and versions (Window, Linux, ...).

## Features developed so far

- Load d64, d71 and d81 image formats
- Load and explore disk header, BAM (Block Availability Map) and directory
- List native Commodore 64 files and Geos files with their resources (icon, decription, size, date, ...)
- Graphical BAM explorer
- Hexadecimal viewer of various data (File Content, VLIR, disk header, Geos file header, directory entries, ...)

## Todo

- Handle more image formats
- Create new images
- Manage files (delete, copy, move, import, export, convert, ...)
- Manage application files (Basic programs, Geo Write documents, images, fonts, ...)
- Work closely with the Vice emulator
- Repair disks and files (validate)
- Handle CBM and DIR file types

## Screenshots

### File listing

![Native C64 files listing](screenshots/filelisting01.png)

![Geos files listing](screenshots/filelisting02.png)

### Disk info

d81 Geos disk image information dialog

![d81 Geos disk](screenshots/diskinfo01.png)

d64 Native disk image information dialog

![d64 disk](screenshots/diskinfo02.png)

Graphical BAM explorer

![d64 BAM](screenshots/BAM01.png)

![d81 BAM](screenshots/BAM02.png)

### File info

File information dialog

![Geos file info](screenshots/fileinfo01.png)

![Native C64 file info](screenshots/fileinfo02.png)

### Hex viewer

![Hex disk directory](screenshots/hex_disk_directory.png)

![Hex disk header](screenshots/hex_disk_header.png)

![Hex file content](screenshots/hex_file_content.png)

![Hex Geos header block](screenshots/hex_file_header.png)

![Hex Geos VLIR](screenshots/hex_file_vlir.png)