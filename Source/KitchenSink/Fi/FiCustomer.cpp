// (c) 2023 Mika Pi

#include "FiCustomer.h"

AFiCustomer::AFiCustomer()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "StarterContent/Shapes", "Shape_NarrowCapsule"));
  mesh->SetMobility(EComponentMobility::Movable);
}
