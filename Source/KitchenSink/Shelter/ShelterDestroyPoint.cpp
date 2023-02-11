#include "ShelterDestroyPoint.h"

AShelterDestroyPoint::AShelterDestroyPoint()
{
  auto mesh = GetStaticMeshComponent();
  mesh->SetStaticMesh(objFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cone.Cone")));
  SetActorHiddenInGame(true);
}
