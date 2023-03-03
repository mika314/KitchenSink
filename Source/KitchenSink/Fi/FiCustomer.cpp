// (c) 2023 Mika Pi

#include "FiCustomer.h"

AFiCustomer::AFiCustomer()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "Quaternius/Mesh", "SM_Alpaking"));
  mesh->SetMobility(EComponentMobility::Movable);
}
