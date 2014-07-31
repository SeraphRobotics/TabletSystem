#pragma once

#include <QString>
#include <QList>
#include "stlfacet.h"


class STLMesh {
public:
  STLMesh();
  ~STLMesh();
  STLMesh(STLMesh& s);
  void operator = (STLMesh&);
  /**
  * Appends a facet to the list stored in this mesh; this method is used
  * when the mesh is being loaded from a file.
  */
  void AddFacet(STLFacet &facet);

  /**
  * Returns the list of all of the "facets" (triangle + normal pairs) in
  * this mesh.
  */
  const QList<STLFacet*>& GetFacets() const;

private:
  QList<STLFacet*> facets_;
  //STLMesh(STLMesh&);
  //void operator = (STLMesh&);
};
