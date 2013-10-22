#ifndef RECORDER_H
#define RECORDER_H

#include "vec3.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <time.h>

#define RANDOMDOUBLE(min,max) (  ((double)rand() / RAND_MAX) * (max-min) + min  ) 
#ifndef PI
#define PI 3.14159265
#endif
#define UNSIGNEDSHORT sizeof(unsigned short)
#define UNSIGNED sizeof(unsigned)
#define UNSIGNEDLONG sizeof(unsigned long)
#define SHORT sizeof(short)
#define INT sizeof(int)
#define LONG sizeof(long)
#define DOUBLE sizeof(double)

class ObjectState {
public:
    ObjectState();
    ~ObjectState();
    static const unsigned short objectLengthInBytes = UNSIGNEDLONG 
                                             + DOUBLE + DOUBLE + DOUBLE
                                             + DOUBLE + DOUBLE + DOUBLE + DOUBLE;
    /* GET OBJECT STATE */
    unsigned long get_object_id() {return objectID;}
    double x()      {return xPos;}
    double y()      {return yPos;}
    double z()      {return zPos;}
    double x_r()    {return xRot;}
    double y_r()    {return yRot;}
    double z_r()    {return zRot;}
    double a_r()    {return aRot;}
    /* SET OBJECT STATE */
    void set_object_id(unsigned long id) {objectID = id;}
    void set_x(double x) {xPos=x;}
    void set_y(double y) {yPos=y;}
    void set_z(double z) {zPos=z;}
    void set_x_r(double xR) {xRot=xR;}
    void set_y_r(double yR) {yRot=yR;}
    void set_z_r(double zR) {zRot=zR;}
    void set_a_r(double aR) {aRot=aR;}
    /* IO */
    void write(std::ofstream& fout);
    void read(std::ifstream& fin);
private:
    /* STATE DATA */
    unsigned long objectID;
    double xPos;    // x position
    double yPos;    // y position
    double zPos;    // z position
    double xRot;    // x of rotation vector
    double yRot;    // y of rotation vector
    double zRot;    // z of rotation vector
    double aRot;    // angle of rotation
};

class FrameState {
public:
    FrameState(unsigned long id);
    ~FrameState();
    void set_frame_id(unsigned long id) {frameID = id;}
    unsigned long get_frame_id()        {return frameID;}
    unsigned long get_num_objects()     {return objects.size();}
    ObjectState * get_object(unsigned long i);
    void add_object(Vec3 * p);
    void add_object(Vec3 p);
    void add_object(Vec3 positions, Vec3 rotationVector, double rotationAngle);
    void add_object(ObjectState * o);
    void write(std::ofstream& fout);
    void read(std::ifstream& fin);
private:
    std::vector<ObjectState *> objects;
    unsigned long frameID;
    void write_header(std::ofstream& fout);
    void read_header(std::ifstream& fin);
};


class TimeSeries {
public:
    TimeSeries();
    ~TimeSeries();
    /* Recording */
    void record_frame(Vec3 * positions, Vec3 * rotationVectors, double * rotationAngles, unsigned n);
    void write(const char * filename);
    /* Reading */
    void read(const char * filename);
    /* Playback */
    FrameState * get_frame(unsigned long i);
    FrameState * get_next_frame();
private:
    std::vector<FrameState *> frames;
    unsigned long nFrames;
    unsigned long nextFrameIndex;
};

class Model {
public:
    Model(unsigned long n);
    ~Model();
    void get_next_object_state(Vec3 ** pos, Vec3 ** rotationVector, double ** rotationAngle);
    void set_filename(char * newFilename);
    unsigned long get_num_objects() {return nObjects;}
    void init();
    void record_frame();
    void finalize_record()  {finalize_record(recordFilename);}
    void finalize_record(const char * filename);
    void advance_frame();
private:
    /* For real time animation */
    unsigned long next_object_index;

    char * recordFilename;
    TimeSeries timeseries;
    unsigned long nObjects;
    /* State data */
    Vec3 * positions;
    Vec3 * rotationVectors;
    double * rotationAngles;
    /* Transitional data */
    Vec3 * velocities;
    double * rotationalVelocities;
    /* Model-specific data */
    double * lengths; 
    double * angles;

};
#endif
