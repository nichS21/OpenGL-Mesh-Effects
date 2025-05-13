#include "Mesh.h"
#include <fstream>
#include <cassert>

void Mesh::setupBuffers() 
{
	//get vertices of each triangle of the mesh (our data to pass in)
	vector<Vertex> vertices;

	for (const Triangle& t : triangles)
	{
		Vertex currVertices[3] = { t.v1, t.v2, t.v3 };
		for (const Vertex& v : currVertices)
		{
			//push vertices
			vertices.push_back(v);
		}
	}

	//-----send the data to OpenGL to load on GPU-----//
	glGenBuffers(1, &vertexBuffer);					// request buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);	// attach to buffer
	glBufferData(GL_ARRAY_BUFFER,					// type of buffer
		vertices.size() * sizeof(Vertex),			// #bytes of data
		vertices.data(),							// where the data is (and where it starts)
												// here starts in beginning, so no offset
		GL_STATIC_DRAW);						// data will not change


	//-----describe the data-----//
	// get current active program -- created in init() in 'main.cpp' -- needed to access shader variables
	//vertex data
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	glGenVertexArrays(1, &attribBuffer);    // request layout buffer
	glBindVertexArray(attribBuffer);        // attach to layout buffer

	// get the id of the attribute variable in (one of the) shaders
	GLuint posAttr = glGetAttribLocation(program, "vertexCoords");   // "vertexCoords" should be "in" variable

	// describe the meaning of the data that was made available to OpenGL (loaded on GPU)
	int ptOffset = offsetof(Vertex, point);
	GLsizei dist = sizeof(Vertex);				//skip one entire vertex length to get to next chunk of data

	glVertexAttribPointer(posAttr,				// which variable in vertex shader is described
		3,										// #components for variable: our vertex contains 3D position coords
		GL_FLOAT,								// type of components (all are float)
		GL_FALSE,								// do not normalize during transfer
		dist,									// distance to next pair (skip rgb color, and 3D normal)
		(GLvoid*)(ptOffset));					// where to start in data buffer

	// enable this attribute for sending to the shaders
	glEnableVertexAttribArray(posAttr);	


	//repeat for vertex color
	//attribute variable for color
	GLuint colAttr = glGetAttribLocation(program, "vertexColor");  

	// describe the meaning of the data that was made available to OpenGL (loaded on GPU)
	int colorOffset = offsetof(Vertex, vColor);

	glVertexAttribPointer(colAttr,				// which variable in vertex shader is described
		3,										// #components for variable: rgb
		GL_FLOAT,								// type of components (all are float)
		GL_FALSE,								// do not normalize during transfer
		dist,								// distance to next pair (skip normal, and next point)
		(GLvoid*)(colorOffset));				// where to start in databuffer

	// enable this attribute for sending to the shaders
	glEnableVertexAttribArray(colAttr);


	//repeat for vertex normal
	//attribute variable for normal
	GLuint normAttr = glGetAttribLocation(program, "vertexNorm");

	// describe the meaning of the data that was made available to OpenGL (loaded on GPU)
	int normOffset = offsetof(Vertex, vNormal);

	glVertexAttribPointer(normAttr,				// which variable in vertex shader is described
		3,										// #components for variable: x,y,z
		GL_FLOAT,								// type of components (all are float)
		GL_FALSE,								// do not normalize during transfer
		dist,								// distance to next pair (skip normal, and next point)
		(GLvoid*)(normOffset));				// where to start in databuffer

	// enable this attribute for sending to the shaders
	glEnableVertexAttribArray(normAttr);
}

void Mesh::draw() const
{
	//Note: all the data has already been sent by the time this method is called, so we just tell OpenGL to draw it
	

	// make active the layout buffers (automatically activates associated data buffer)
	glBindVertexArray(attribBuffer);

	// draw the vertex data that was loaded and described in the activated buffers
	glDrawArrays(GL_TRIANGLES,		 // type of primitives to draw
		0,							 // where to being in buffer: 0 offset, i.e. from beginning
		triangles.size() * 3		 // total number of vertices (depends on primitive type)
	);
}

Mesh::Mesh(string filename)
{
	//open file that has mesh's triangles
	ifstream ifs(filename);

	string token;
	Triangle tri;

	while (ifs >> token) {								//	loop through source file
		if (token == "smooth_triangle")
		{
			ifs >> tri;

			tri.setSmooth(true);							// mesh always starts as smooth for openGL project

			triangles.push_back(tri);
		}
	}

	//hardcode baseline material coefficients
	mat.ka = 0.4;
	mat.kd = 0.5;
	mat.ks = 0.7;
	mat.n = 70;
}

optional<Hit> Mesh::intersect(const Ray& ray) const
{
	//before checking for where intersection is in mesh, first check if it intersects the bounding sphere first
	optional<Hit> boundHit = bound.intersect(ray);
	if (!boundHit) return {};										// bound not hit -> mesh never hit


	//loop through all triangles to check for an intersection, finding CLOSEST intersection
	optional<Hit> minHit;
	float minT = FLT_MAX;

	
	for (const Triangle& tri : triangles)
	{
		optional<Hit> hit = tri.intersect(ray);

		if (hit) hit = updateHit(hit);								//check for case where hit is not visible, before checking if it is a new min
		else continue;												//no hit, continue looking
		
		//check for new minimum
		if (hit && hit->t < minT)
		{
			minT = hit->t;
			minHit = hit;
		}
	}

	if (minHit)	return minHit;
	
	return {};	//failed to find any intersection
}

optional<Hit> Mesh::viableT(float t, const Ray& ray) const
{
	return {};
}

pair<float, float> Mesh::texel_uv(const Point& pt) const
{
	return {};
}

Unit Mesh::bumpNormal(const Point& pt, const Unit& N, float u, float v) const
{
	if (bumpMap == nullptr) return N;			//no bump map, return regular normal

	//else was a bump map
	//compute U, V, then perturbed normal
	auto [du, dv] = bumpMap->gradient(u, v);


	Vector yAxis(0, 1, 0);
	Vector U = yAxis.cross(N);	

	if (U == Vector(0, 0, 0)) U = Vector(1, 0, 0).cross(N);			// if yAxis and N are parallel dot product is zero, so use x axis instead

	Vector V = N.cross(U);

	Vector norm = N + du * U + dv * V;
	return Unit(norm);

	
	return Unit();
}

optional<Hit> Mesh::updateHit(optional<Hit> hit) const
{
	if (mapMode == "direct")
	{
		//similar to image mapping for other shapes, except we already know t is viable, and know u,v

		//check if object isVisible (for masking purposes)
		if (isVisible(hit->u, hit->v))
		{
		
			//get either solid color of shape or color of its texture at intersection
			Color obColor = selectColor(hit->u, hit->v);

			Unit normal = bumpNormal(hit->inter, hit->normal, hit->u, hit->v);

			//Create hit object and return it
			return Hit{ hit->inter, normal, obColor, hit->t, mat };
		}

		return {};												//object not visible
	}
	else if (mapMode == "spherical")
	{
		Ray boundRay(hit->inter, hit->normal);					//from hit on triangle in direction of the normal at this intersection
		optional<Hit> sphereHit = bound.intersect(boundRay);
		sphereHit->t = hit->t;									//keep t from triangle's intersection, not the sphere so can still compare for closest intersected triangle in mesh
		//sphereHit->normal = hit->normal;

		return sphereHit;
	}

	//no mapping chosen, just return regular hit to see interpolated colors
	return Hit{ hit->inter, hit->normal, hit->color, hit->t, mat };
}

Color Mesh::selectColor(float u, float v) const
{
	//if no texture, just return solid color
	if (texture == nullptr) return color;

	//is texture, so find color at given coordinates in texure map
	return texture->rgb_uv(u, v);
}

bool Mesh::isVisible(float u, float v) const
{
	if (this->mask == nullptr) return true;		//if no mask, point is visible by default

	//if is mask, find color at given coordinates in the mask (value greater than 0 = visible)
	return mask->gray_uv(u, v) > 0;
}

ostream& operator<< (ostream& os, const Mesh& m)
{
	cout << "Mesh: " << m.triangles.size() << " triangles" << endl;

	for (const Triangle& tri : m.triangles)
	{
		cout << tri << endl;
	}


	return os;
}

istream& operator>>(istream& is, Mesh& m)
{
	/*
	//variables for reading input
	string token;

	string mode;
	string source;
	Triangle tri;


	is >> token >> source;								// get 'source,' then path to source file
	is >> mode;											// get 'flat' or 'smooth' keyword for mesh
	is >> m.mapMode;									// read in mapping mode: direct or spherical

	is >> token >> m.scaleMesh;								// read in 'scale' and scale factor
	is >> token >> m.transMesh;							// read in 'translate' and the translation 'vector'


	//varibles for computing bounding sphere
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;

	//read triangles from mesh source file
	ifstream ifs(source);

	while (ifs >> token) {								//	loop through source file
		if (token == "smooth_triangle")
		{
			ifs >> tri;

			if (mode == "smooth") tri.setSmooth(true);		// mesh is smooth, so triangles have averaged normals
			else tri.setSmooth(false);						// mesh is flat, so each vertex's normal is the triangle's flat normal

			//tri.resize(scale, translate);					// scale and translate each triangle

			//gather data for bounding sphere
			for (const Point& vert : tri.vertices)
			{
				if (vert.x() > maxX)	maxX = vert.x();
				if (vert.y() > maxY)	maxY = vert.y();
				if (vert.z() > maxZ)	maxZ = vert.z();
				if (vert.x() < minX)	minX = vert.x();
				if (vert.y() < minY)	minY = vert.y();
				if (vert.z() < minZ)	minZ = vert.z();
			}

			m.triangles.push_back(tri);
		}
	}

	//create bounding sphere
	Point boxMinPt(minX, minY, minZ, 1);					
	Point boxMaxPt(maxX, maxY, maxZ, 1);
	Vector diagVec(boxMinPt, boxMaxPt);									//diagonal between boxMinPt and boxMaxPt of the bounding box
	float radius = diagVec.length() / 2;

	Vector center = (Vector(boxMinPt) + Vector(boxMaxPt)) / 2;			//center of bounding sphere -> midpoint of the diagnoal of bounding box

	
	m.bound = Sphere(center.point(), radius);

	//read appearance of shape
	if (m.mapMode == "spherical") m.bound.readApperance(is);			//spherical mapping mode, sphere returns color, material, normal
	else m.readApperance(is);
	*/
	return is;

}




