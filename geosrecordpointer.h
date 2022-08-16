#ifndef GEOSRECORDPOINTER_H
#define GEOSRECORDPOINTER_H


class GeosRecordPointer {
    public:
        GeosRecordPointer(int track, int sector);
        void set(int track, int sector);
        int track();
        int sector();
    private:
        int Track;
        int Sector;
};

#endif // GEOSRECORDPOINTER_H
