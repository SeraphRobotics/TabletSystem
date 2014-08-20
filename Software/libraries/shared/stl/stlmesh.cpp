/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Sean Cretella (sac76@cornell.edu)

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the <organization> nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNERS OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\*---------------------------------------------------------------------------*/
#include "STLMesh.h"
STLMesh::STLMesh() { }

STLMesh::STLMesh(STLMesh& M){
    QList<STLFacet*> facets= M.GetFacets();
    for(int i=0; i<facets.size();i++){
        STLFacet* f = new STLFacet();
        f->triangle = FAHTriangle(M.GetFacets().at(i)->triangle);
        f->normal = FAHVector3(M.GetFacets().at(i)->normal);
        facets_.append(f);
    }
}

STLMesh::STLMesh(STLMesh* M){
    QList<STLFacet*> facets= M->GetFacets();
    for(int i=0; i<facets.size();i++){
        STLFacet* f = new STLFacet();
        f->triangle = FAHTriangle(M->GetFacets().at(i)->triangle);
        f->normal = FAHVector3(M->GetFacets().at(i)->normal);
        facets_.append(f);
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


void STLMesh::scale(float px,float py,float pz){
    for(int i=0;i<facets_.size();i++){
        STLFacet* f= facets_[i];
        for(int j=0;j<3;j++){
            f->triangle.v[j].x=f->triangle.v[j].x*px;
            f->triangle.v[j].y=f->triangle.v[j].y*py;
            f->triangle.v[j].z=f->triangle.v[j].z*pz;
        }
    }
}


