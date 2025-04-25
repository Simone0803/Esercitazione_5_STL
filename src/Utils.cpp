#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
	string FilePathInput = "./Cell0Ds.csv";
	ifstream file(FilePathInput);
	list<string> listLines;

	if(file.fail()) {
		return false;
	}
	
	string line;
	while (getline(file, line))
		listLines.push_back(line);
	
	file.close();
	listLines.pop_front();

	mesh.NumCell0Ds = listLines.size();

	
	if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;

        converter >> id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);
        mesh.Cell0DsId.push_back(id);

			auto it = mesh.MarkerCell0Ds.find(marker);
			if(it!=mesh.MarkerCell0Ds.end())
			{
				((*it).second).push_back(id);
			}
			else {
				mesh.MarkerCell0Ds.insert({marker, {id}});
			}

    }
	
    return true;
}


// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
	string FilePathInput = "./Cell1Ds.csv";
	ifstream file(FilePathInput);
	list<string> listLines;

    if(file.fail())
        return false;
	
	else
	{
		string line;
		while (getline(file, line))
			listLines.push_back(line);

		// remove header
		listLines.pop_front();

		mesh.NumCell1Ds = listLines.size();
		file.close();
	}
	
	if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;

        converter >> id >> marker >> mesh.Cell1DsExtrema(0, id) >>  mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);

			auto it = mesh.MarkerCell1Ds.find(marker);
			if(it != mesh.MarkerCell1Ds.end())
			{
				mesh.MarkerCell1Ds[marker].push_back(id);
				// (*it).second.push_back(id);
			}
			else
				mesh.MarkerCell1Ds.insert({marker, {id}});
    }

    return true;
	
}

// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (string& line : listLines)
    {
		replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);


        unsigned int id;
		unsigned int marker;
		unsigned int NumVertices;
		unsigned int NumEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

		converter >> id >> marker >> NumVertices;
        vertices.resize(NumVertices);
		for (unsigned int i = 0; i < NumVertices; ++i)
			converter >> vertices[i];

		converter >> NumEdges;
		edges.resize(NumEdges);
		for (unsigned int i = 0; i < NumEdges; ++i)
			converter >> edges[i];
        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
		
		/// Memorizza i marker
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                mesh.MarkerCell2Ds[marker].push_back(id);
                //it->second.push_back(id);
            }
    }

    return true;
}

// ***************************************************************************

bool TestArea(PolygonalMesh& mesh, const double& tol)
{
	double AreaTol = tol*tol*sqrt(3)/4;
	unsigned int NumVertices;
	vector<unsigned int> vertices;
	double x1_tmp, y1_tmp, x2_tmp, y2_tmp;
	for(unsigned int id = 0; id < mesh.NumCell2Ds; id++)
	{
		double Area = 0.0;
		double sum = 0.0;
		NumVertices = mesh.Cell2DsVertices[id].size();
		vertices.resize(NumVertices);
		vertices = mesh.Cell2DsVertices[id];
		for(unsigned int j = 0; j < NumVertices-1; j++)
		{
			x1_tmp = mesh.Cell0DsCoordinates(0,vertices[j]);
			x2_tmp = mesh.Cell0DsCoordinates(0,vertices[j+1]);
			y1_tmp = mesh.Cell0DsCoordinates(1,vertices[j]);
			y2_tmp = mesh.Cell0DsCoordinates(1,vertices[j+1]);
			sum += (x1_tmp*y2_tmp - x2_tmp*y1_tmp);
		}
		x1_tmp = mesh.Cell0DsCoordinates(0, vertices[NumVertices-1]);
		y1_tmp = mesh.Cell0DsCoordinates(1, vertices[NumVertices-1]);
		x2_tmp = mesh.Cell0DsCoordinates(0, vertices[0]);
		y2_tmp = mesh.Cell0DsCoordinates(1, vertices[0]);
		sum += (x1_tmp*y2_tmp - x2_tmp*y1_tmp);
		Area = 0.5*abs(sum);
		if (Area < AreaTol)
		{
			cerr << "ci sono poligoni con area troppo piccola" << endl;
			return false;
		}
	}
	cout << "Tutti i poligoni hanno un'area sufficientemente grande." << endl;
	return true;
}

// ***************************************************************************

bool TestLength(PolygonalMesh& mesh, const double& tol)
{
    for(unsigned int id = 0; id < mesh.NumCell1Ds; id++)
		{
			int A1 = mesh.Cell1DsExtrema(0, id);
			int A2 = mesh.Cell1DsExtrema(1, id);
			double x1 = mesh.Cell0DsCoordinates(0, A1);
			double y1 = mesh.Cell0DsCoordinates(1, A1);
			double x2 = mesh.Cell0DsCoordinates(0, A2);
			double y2 = mesh.Cell0DsCoordinates(1, A2);
			double distanza = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			if(distanza < tol) 
			{
				cerr << "ci sono lati degeneri" << endl;
				return false;
			}			
		}
		cout << "Tutti i lati hanno una lunghezza maggiore della tolleranza" << endl;
		return true;
}

// ***************************************************************************

/* bool TestMarkers(PolygonalMesh& mesh);
{
		
}
 */
}