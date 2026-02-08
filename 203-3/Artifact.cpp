#include "Artifact.h"

Artifact::Artifact()
    : artifactID(-1),
      name(""),
      rarityLevel(1),
      researchValue(0),
      assignedToName("")
{
}

Artifact::Artifact(int id, const std::string &n, int rarity, int value)
    : artifactID(id),
      name(n),
      rarityLevel(rarity),
      researchValue(value),
      assignedToName("")
{
}

void Artifact::updateValueBasedOnUsage()
{
//TODO
  numAssignments++;
  researchValue += rarityLevel*numAssignments;  // updating with adding number of succesfull assignments * rarity level
}
