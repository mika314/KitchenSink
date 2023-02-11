#include "ShelterShelter.h"

AShelterShelter::AShelterShelter()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(OBJ_FINDER(StaticMesh, "1-Shelter", "SM_ShelterShelter"));
}
