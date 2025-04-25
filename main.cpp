#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
	PolygonalMesh mesh;
	
	if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
	
	
	/* PARTE DEI TEST */
	
	
	const double tol = 1e-8;
	TestArea(mesh,tol);
	cout << '\n' << endl;
	TestLength(mesh,tol);
	cout << '\n' << endl;
	
	cout << "Controllo markers delle celle 0D" << '\n' << endl;
	for (const auto& pair : mesh.MarkerCell0Ds)
	{
		unsigned int marker = pair.first;
		const list<unsigned int>& ids = pair.second;
		cout << "Marker: " << marker << "  ";
		for (unsigned int id : ids)
		{
			cout << "ID: " << id << " ";
		}
		cout << endl;
	}
	cout << "**************************************************************************************************" << endl;
	
	cout << "Controllo markers delle celle 1D" << '\n' << endl;
	for (const auto& pair : mesh.MarkerCell1Ds)
	{
		unsigned int marker = pair.first;
		const list<unsigned int>& ids = pair.second;
		cout << "Marker: " << marker << "  ";
		for (unsigned int id : ids)
		{
			cout << "ID: " << id << " ";
		}
		cout << endl;
	}
	cout << "**************************************************************************************************" << endl;
	
	cout << "Controllo markers delle celle 2D" << '\n' << endl;
	for (const auto& pair : mesh.MarkerCell2Ds)
	{
		unsigned int marker = pair.first;
		const list<unsigned int>& ids = pair.second;
		cout << "Marker: " << marker << "  ";
		for (unsigned int id : ids)
		{
			cout << "ID:" << id << " ";
		}
		cout << endl;
	}
//**************************************************************************************************
    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp", mesh.Cell0DsCoordinates);
    utilities.ExportSegments("./Cell1Ds.inp", mesh.Cell0DsCoordinates, mesh.Cell1DsExtrema);

    return 0;
}
