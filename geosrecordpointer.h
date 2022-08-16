#ifndef GEOSRECORDPOINTER_H
#define GEOSRECORDPOINTER_H


class GeosRecordPointer {
    public:
        GeosRecordPointer(int track, int sector, int id);
        void set(int track, int sector);
        int track();
        int sector();
        int id();
    private:
        int Track;
        int Sector;
        int ID;
};

#endif // GEOSRECORDPOINTER_H
