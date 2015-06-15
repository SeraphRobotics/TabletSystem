#include "topcoatcontroller.h"
#include <math.h>
#include "scannerfunctions.h"
#include <QSettings>
#include "globals.h"
#include "Controllers/repaircontroller.h"
#include "Controllers/stlgeneration.h"

//#include "UnitTest/debugfunctions.h"

TopCoatController::TopCoatController(Orthotic* orth, QString dir, FAHMatrix4x4 transform, QObject *parent):
    QObject(parent),dir_(dir),transform_(transform)
{
    orth_=orth;
}


void TopCoatController::generateTopCoat(){
    Top_Coat t =  orth_->getTopCoat();
    if(t.style!=Top_Coat::kAuto){
        emit generatedCoatingFile("");
        return;
    }

    STLMesh* topcoat = new STLMesh();
    QList<FAHLoopInXYPlane*> inners;
    QSettings settings;
    float thickness = settings.value("printing/topcoat-thickness",5).toFloat();
    for(int i=0;i<orth_->topcoatgrid->nx();i++){
        for(int j=0;j<orth_->topcoatgrid->ny();j++){
            orth_->topcoatgrid->operator ()(i,j) = orth_->topcoatgrid->at(i,j)+thickness;
        }
    }
    FixedThicknessSTL(topcoat,orth_->topcoatgrid,orth_->getLoop(),inners,thickness);
    topcoat->transform(transform_);
    QString topcoat_file_name = dir_ + "/topcoat.stl";
    RepairController* rs = new RepairController(topcoat,topcoat_file_name);
    rs->repairMesh();



}
