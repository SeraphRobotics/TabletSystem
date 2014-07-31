#pragma once

#include <QString>
#include <QImage>
#include <qfile.h>
#include "STLMesh.h"

class AMFFile;

class STLFile {
public:

  /**
   * Detects an STL file as either binary or ASCII and reads it in.
   * Returns whether the file was successfully loaded.
   */
  bool read(QString path);

  /**
   * Reads in an STL file with a specified path, validates it, and
   * stores its data as a reference to a list of facet pointers.
   */
  bool readASCII(QString path);

  /**
   * Reads in an binary STL file with a specified path, validates it, and
   * stores its data as a reference to a list of facet pointers.
   */
  bool readBinary(QString path);
	
  /**
   * Writes facet data from a mesh into an STL file
   * with the specified output path.
   */
  bool WriteASCII(QString path);

  /**
   * Writes facet data from a BMP file, outputing a mesh outlining the
   * edges of the input file
   */
  bool ReadBMP(QString path);

  /**
   * Gets the mesh of this STL file
   */
  const STLMesh& GetMesh();

  /**
   * Translates this STL into an AMF model
   */
  void convertToAMF(AMFFile* output);

private:

  /**
   * Traverses the path specified by (dx,dy,0) starting at point
   * (x,y,z) in the image Im, and writes points A, B, C, and D.
   */
  void traversePath(FAHVector3* A, FAHVector3* B, FAHVector3* C, FAHVector3* D,
                    const QImage* Im, bool** visited, const int& x,
                    const int& y, const int& z, const int dx, const int dy,
                    const bool out);

  STLMesh mesh_;
};
