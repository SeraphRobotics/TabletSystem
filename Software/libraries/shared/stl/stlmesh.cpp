#include "STLMesh.h"
STLMesh::STLMesh() { }

STLMesh::STLMesh(STLMesh& s){
    foreach(STLFacet* facet,s.GetFacets()){
        STLFacet fct;
        fct.triangle = facet->triangle;
        fct.normal = facet->normal;
        AddFacet(fct);
    }
}


STLMesh::~STLMesh() {
  // delete all of the facets
  for (int i = 0; i < facets_.size(); i++) {
    delete facets_.at(i);
  }
}

void STLMesh::AddFacet(STLFacet& facet) {
  facets_.append(new STLFacet(facet));
}

const QList<STLFacet*>& STLMesh::GetFacets() const {
  return facets_;
}



