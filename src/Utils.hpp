#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary
{
/// Import the polygonal mesh and test if the mesh is correct
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh);

/// Import the Cell0D properties from Cell0Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PolygonalMesh& mesh);

/// Import the Cell1D properties from Cell1Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh);

/// Import the Cell2D properties from Cell2Ds.csv file
/// mesh: a PolygonalMesh struct
/// return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh);

/// Test if each polygon has non-zero area
/// input: mesh and tolerance
/// return the result of the testing, true if is success, false otherwise
bool TestArea(PolygonalMesh& mesh, const double& tol);

///	Test if each edge has non-zero length
///	input: mesh and tolerance			 
/// return the result of the testing, true if is success, false otherwise
bool TestLength(PolygonalMesh& mesh, const double& tol);

}

