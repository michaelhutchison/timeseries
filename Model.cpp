#include "Model.h"

/******************************
 * ObjectState
 *  Stores position and orientation for
 *  a single item in the scene.
 ******************************/
ObjectState::ObjectState() {
    objectID = 0;
    xPos = 0;    // x position
    yPos = 0;    // y position
    zPos = 0;    // z position
    xRot = 0;    // x of rotation vector
    yRot = 0;    // y of rotation vector
    zRot = 0;    // z of rotation vector
    aRot = 0;    // angle of rotation
}
ObjectState::~ObjectState() {

}
    /*****************************************************
     *  OBJECT STATE RECORD
     *  unsigned            object ID
     *  double              x position
     *  double              y position
     *  double              z position
     *  double              x of rotation vector
     *  double              y of rotation vector
     *  double              z of rotation vector
     *  double              angle of rotation
     *****************************************************/
void ObjectState::write(std::ofstream& fout) {
    // Write object ID
    fout.write((char*)&objectID, UNSIGNED);
    // Write position
    fout.write((char*)&xPos,DOUBLE);
    fout.write((char*)&yPos,DOUBLE);
    fout.write((char*)&zPos,DOUBLE);
    // Write orientation 
    fout.write((char*)&xRot,DOUBLE);
    fout.write((char*)&yRot,DOUBLE);
    fout.write((char*)&zRot,DOUBLE);
    fout.write((char*)&aRot,DOUBLE);
    /* 
    std::cout << "   object ID " << objectID << std::endl;
    std::cout << "   x pos " << xPos << std::endl;
    std::cout << "   y pos " << yPos << std::endl;
    std::cout << "   z pos " << zPos << std::endl;
    std::cout << "   x rot " << xRot << std::endl;
    std::cout << "   y rot " << yRot << std::endl;
    std::cout << "   z rot " << zRot << std::endl;
    std::cout << "   a rot " << aRot << std::endl;
    */
}
void ObjectState::read(std::ifstream& fin) {
    // Read object ID
    fin.read((char*)&objectID, UNSIGNED);
    // Read position
    fin.read((char*)&xPos, DOUBLE);
    fin.read((char*)&yPos, DOUBLE);
    fin.read((char*)&zPos, DOUBLE);
    // Read orientation
    fin.read((char*)&xRot, DOUBLE);
    fin.read((char*)&yRot, DOUBLE);
    fin.read((char*)&zRot, DOUBLE);
    fin.read((char*)&aRot, DOUBLE);
    /*
    std::cout << "   object ID " << objectID << std::endl;
    std::cout << "   x pos " << xPos << std::endl;
    std::cout << "   y pos " << yPos << std::endl;
    std::cout << "   z pos " << zPos << std::endl;
    std::cout << "   x rot " << xRot << std::endl;
    std::cout << "   y rot " << yRot << std::endl;
    std::cout << "   z rot " << zRot << std::endl;
    std::cout << "   a rot " << aRot << std::endl;
    */
}

/******************************
 * FrameState
 *  Stores position and orientation (ObjectState)
 *  for a collection of objects in a single 
 *  time slice of the scene.
 ******************************/
FrameState::FrameState(unsigned id) {
    frameID = id;
}
FrameState::~FrameState() {
    // Free dynamically allocated memory
    for (unsigned i=0; i < objects.size(); i++) {
        delete objects[i];
    }
}
ObjectState * FrameState::get_object(unsigned i) {
    if (i >= objects.size()) return 0;
    return objects[i];
}
void FrameState::add_object(Vec3 * p) {
    ObjectState * o = new ObjectState;
    o->set_x(p->x);
    o->set_y(p->y);
    o->set_z(p->z);
    add_object(o);
}
void FrameState::add_object(Vec3 p) {
    ObjectState * o = new ObjectState;
    o->set_x(p.x);
    o->set_y(p.y);
    o->set_z(p.z);
    add_object(o);
}
void FrameState::add_object(Vec3 position, Vec3 rotationVector, double rotationAngle) {
    ObjectState * o = new ObjectState;
    // Set position
    o->set_x(position.x);
    o->set_y(position.y);
    o->set_z(position.z);
    // Set rotation vector
    o->set_x_r(rotationVector.x);
    o->set_y_r(rotationVector.y);
    o->set_z_r(rotationVector.z);
    o->set_a_r(rotationAngle);
    add_object(o);
}
void FrameState::add_object(ObjectState * o) {
    objects.push_back(o);
}
void FrameState::write(std::ofstream& fout) {
    //std::cout << "FRAME " << frameID << std::endl;
    write_header(fout);
    for (unsigned i=0; i < objects.size(); i++) {
        objects[i]->write(fout);
    }
}
void FrameState::read(std::ifstream& fin){
    //std::cout << "FRAME IN" << std::endl;
    read_header(fin);
    for (unsigned i=0; i<objects.size(); i++){
        ObjectState * o = new ObjectState;
        objects[i] = o;
        objects[i]->read(fin);
    }
}
    /*****************************************************
     *  FRAME STATE HEADER
     *  unsigned long       frame size in bytes
     *  unsigned int        frame ID
     *  unsigned int        number of objects in frame
     *  unsigned short      length of each object in bytes
     *****************************************************/
void FrameState::write_header(std::ofstream& fout) {
   // Calculate data sizes
    unsigned short singleObjectLengthInBytes = ObjectState::objectLengthInBytes;
    unsigned long allObjectsLengthInBytes = singleObjectLengthInBytes * objects.size();
    unsigned long frameLengthInBytes = UNSIGNEDLONG 
                                      + UNSIGNED 
                                      + UNSIGNED
                                      + UNSIGNEDSHORT
                                      + allObjectsLengthInBytes;
    unsigned nObjects = objects.size();

    
    // Write frame size in bytes
    fout.write((char*)&frameLengthInBytes,UNSIGNEDLONG);
    // Write frame ID
    fout.write((char*)&frameID,UNSIGNED);
    // Write number of objects
    fout.write((char*)&nObjects,UNSIGNED);
    // Write length of an object
    fout.write((char*)&singleObjectLengthInBytes,UNSIGNEDSHORT);
    /*
    std::cout << " frame len " << frameLengthInBytes << std::endl;
    std::cout << " frame ID " << frameID << std::endl;
    std::cout << " nObjects " << nObjects << std::endl;
    std::cout << " object len " << singleObjectLengthInBytes << std::endl;
    */
}
void FrameState::read_header(std::ifstream& fin) {
    // Read frame size in bytes
    unsigned long frameLengthInBytes;
    fin.read((char*)&frameLengthInBytes,UNSIGNEDLONG);
    // Read frame ID
    fin.read((char*)&frameID,UNSIGNED);
    // Read number of objects
    unsigned nObjects;
    fin.read((char*)&nObjects,UNSIGNED);
    // Read length of an object
    unsigned short singleObjectLengthInBytes;
    fin.read((char*)&singleObjectLengthInBytes,UNSIGNEDSHORT);
    /*
    std::cout << " frame len " << frameLengthInBytes << std::endl;
    std::cout << " frame ID " << frameID << std::endl;
    std::cout << " nObjects " << nObjects << std::endl;
    std::cout << " object len " << singleObjectLengthInBytes << std::endl;
    */
    // Clean and re-size objects vector
    for (unsigned i=0; i<objects.size(); i++) {
        delete objects[i];
    }
    objects.resize(nObjects);
}


/******************************
 * TimeSeries
 *  Stores a series of precomputed
 *  frames composing an animation.
 ******************************/
TimeSeries::TimeSeries() {
    nFrames = 0;
    nextFrameIndex = 0;
}
TimeSeries::~TimeSeries() {
    // Release dynamically allocated memory
    for (unsigned i=0; i < frames.size(); i++) {
        delete frames[i];
    }
}
FrameState * TimeSeries::get_frame(unsigned i) {
    if (i >= frames.size()) return 0;
    return frames[i];
}
FrameState * TimeSeries::get_next_frame() {
    FrameState * nextFrame = get_frame(nextFrameIndex);
    nextFrameIndex++;
    if (nextFrameIndex >= frames.size()) nextFrameIndex = 0;
    return nextFrame;
}
void TimeSeries::record_frame(Vec3 * positions, Vec3 * rotationVectors, double * rotationAngles, unsigned n) {;
    // Create a new frame
    FrameState * frame = new FrameState(nFrames);
    frames.push_back(frame);
    nFrames++;

    // Record each object in the frame
    for (unsigned i=0; i<n; i++) {
        frame->add_object(positions[i],rotationVectors[i],rotationAngles[i]);
    }
}
    /*****************************************************
     *  TIME SERIES FILE HEADER
     *  unsigned short      header length in bytes
     *  unsigned long       number of frames in file
     *****************************************************/
void TimeSeries::write(const char * filename) {
    std::cout << "Writing timeseries data" << std::endl;
    // Open file for writing
    std::ofstream fout;
    fout.open(filename, std::ios::out|std::ios::binary);
    // Write header length
    unsigned short headerLengthInBytes = UNSIGNEDSHORT 
                                       + UNSIGNED;
    fout.write((char*)&headerLengthInBytes,UNSIGNEDSHORT);
    // Write number of frames
    fout.write((char*)&nFrames,UNSIGNED);
    /*
    std::cout << " header len " << headerLengthInBytes << std::endl;
    std::cout << " nFrames " << nFrames << std::endl;
    */
    // Write each frame
    for (unsigned i=0; i<frames.size(); i++) {
        frames[i]->write(fout);
    }
    // Cleanup
    fout.close();
}
void TimeSeries::read(const char * filename) {
    std::cout << "Loading timeseries data" << std::endl;
    // Reset: Clear frames and initialize metadata
    for (unsigned i=0; i < frames.size(); i++) {
        delete frames[i];
    }
    nFrames = 0;
    nextFrameIndex = 0;
    // Open file for reading
    std::ifstream fin;
    fin.open(filename, std::ios::in|std::ios::binary);
    // Read header length in bytes
    unsigned short headerLengthInBytes;
    fin.read((char*)&headerLengthInBytes, UNSIGNEDSHORT);
    // Read number of frames
    fin.read((char*)&nFrames, UNSIGNED);
    /*
    std::cout << " header len " << headerLengthInBytes << std::endl;
    std::cout << " nFrames " << nFrames << std::endl;
    */
    // Read each frame
    for (unsigned i=0; i<nFrames; i++){
        // Create a new frame
        FrameState * frame = new FrameState(i);
        frames.push_back(frame);
        frame->read(fin);
    }
    // Cleanup
    fin.close();
}


/******************************
 * Model
 *  
 ******************************/
Model::Model(unsigned n) {
    nObjects = n;
    recordFilename = new char[101];
    strncpy(recordFilename, "animation.rcd",100);
    next_object_index = 0;
    // Allocate memory
    positions = new Vec3[nObjects];
    rotationVectors = new Vec3[nObjects];
    rotationAngles = new double[nObjects];
    velocities = new Vec3[nObjects];
    rotationalVelocities = new double[nObjects];
    lengths = new double[nObjects];
    angles = new double[nObjects];
}
Model::~Model() {
    // Release dynamically allocated memory
    delete[] positions;
    delete[] rotationVectors;
    delete[] rotationAngles;
    delete[] velocities;
    delete[] rotationalVelocities;
    delete[] lengths;
    delete[] angles;
}

void Model::get_next_object_state(Vec3 ** pos, Vec3 ** rotationVector, double ** rotationAngle) {
    if (next_object_index < nObjects) {
        *pos = &positions[next_object_index];
        *rotationVector = &rotationVectors[next_object_index];
        *rotationAngle = &rotationAngles[next_object_index];
        next_object_index++;
    }
    else {
        pos = 0;
        rotationVector = 0;
        rotationAngle = 0;
    }
}
void Model::set_filename(char * newFilename) {
    strncpy(recordFilename, newFilename, 100);
}
void Model::init() {
    // randomize time
    srand (time(NULL));
    // Generate initial state
    for (unsigned i=0; i<nObjects; i++) {
        angles[i] = RANDOMDOUBLE(0, 2*PI);
        lengths[i] = RANDOMDOUBLE(0.1, 5.0);
        positions[i].set(lengths[i]*cos(angles[i]),
                         lengths[i]*sin(angles[i]),
                         RANDOMDOUBLE(-3.0,3.0) );
        rotationVectors[i].set(RANDOMDOUBLE(-1.0,1.0),
                               RANDOMDOUBLE(-1.0,1.0),
                               RANDOMDOUBLE(-1.0,1.0));
        rotationAngles[i] = RANDOMDOUBLE(0,2*PI);
        rotationalVelocities[i] = RANDOMDOUBLE(-5,5);
    }
}
void Model::record_frame() {
    // Save the current frame to the animation record
    timeseries.record_frame(positions,rotationVectors,rotationAngles, nObjects);

}
void Model::finalize_record(const char * filename) {
    // Write all saved timeseries frames to file
    timeseries.write(filename);
}
void Model::advance_frame() {
    /*
     * This method defines how the objects in the space change over time.
     *   objects rotate around the origin at a speed inversely proportional 
     *      to their distance from the origin.
     *   objects spin on a unique rotational axis at a steady angular velocity.
     */
    for (unsigned i=0; i<nObjects; i++) {
        // Update angle about the origin
        double angularVelocity = 0.05/lengths[i];
        if (angularVelocity <= 0) angularVelocity = 0.01;
        angles[i] += angularVelocity;
        if (angles[i] > 2*PI) angles[i] -= 2*PI;

        // Update position - fixed distance from origin at the new angle
        positions[i].x = lengths[i]*cos(angles[i]);
        positions[i].y = lengths[i]*sin(angles[i]);

        // Update rotation angle - rotation vector does not change
        rotationAngles[i] += rotationalVelocities[i];
        //while (rotationAngles[i] > 2*PI) rotationAngles[i] -= 2*PI;
        //while (rotationAngles[i] < -2*PI) rotationAngles[i] += 2*PI;
        //if (rotationAngles[i] > 2*PI) rotationAngles[i] -= 2*PI;
    }
    next_object_index = 0;
}
