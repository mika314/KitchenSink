#!/bin/bash

function rand_suffix() {
    echo -n $RANDOM
}

# get the script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo Script direcotry ${DIR}

# exctract the top direcotry name
PrjName=$(basename ${DIR})

echo Top directory ${PrjName}

UNREALROOTPATH=/mnt/Texas/prj/ue5

cd ${UNREALROOTPATH}
source Engine/Build/BatchFiles/Linux/SetupEnvironment.sh -dotnet Engine/Build/BatchFiles/Linux

dotnet Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll \
       -ModuleWithSuffix=${PrjName},$(rand_suffix) \
       ${PrjName}Editor \
       Linux \
       Development \
       -Project="${DIR}/${PrjName}.uproject" \
       "${DIR}/${PrjName}.uproject"  \
       -IgnoreJunk \
       -progress
