// ┌───────────────────────────────────────────────────────────────────────────┐
// │  Mesh Component                                                           │
// │                                                                           │
// └───────────────────────────────────────────────────────────────────────────┘
/* A mesh contains vertex information that has been loaded to the GPU. */

#pragma once

/* System includes */
#include <iostream>
#include <map>
#include <mutex>

/* External includes */
#include <glm/glm.hpp>

/* Project includes */
// #include "Foton/Tools/Options.hxx"
#include <visii/utilities/static_factory.h>
#include <visii/mesh_struct.h>

/* Forward declarations */
class Vertex;

/* Class declaration */
class Mesh : public StaticFactory
{
	friend class StaticFactory;
	friend class Entity;
	public:
		/** 
		 * Creates a rectangular box centered at the origin aligned along the x, y, and z axis. 
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param size Half of the side length in x (0), y (1) and z (2) direction. 
		 * @param segments The number of segments in x (0), y (1) and z (2)
		 * directions. All should be >= 1. If any one is zero, faces in that
		 * direction are not genereted. If more than one is zero the mesh is empty.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createBox(
			std::string name, 
			glm::vec3  size = glm::vec3(1.0), 
			glm::ivec3 segments = glm::ivec3(1));
		
        /** 
		 * Creates a cone with a cap centered at the origin and pointing towards the positive z-axis. 
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the flat (negative z) end along the xy-plane.
		 * @param size Half of the length of the cylinder along the z-axis.
		 * @param slices Number of subdivisions around the z-axis.
		 * @param segments Number of subdivisions along the z-axis.
		 * @param rings Number of subdivisions of the cap.
		 * @param start Counterclockwise angle around the z-axis relative to the positive x-axis.
		 * @param sweep Counterclockwise angle around the z-axis.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCappedCone(
			std::string name, 
			float radius   = 1.0, 
			float size     = 1.0, 
			int   slices   = 32, 
			int   segments = 8, 
			int   rings    = 4, 
			float start    = 0.0, 
			float sweep    = radians(360.0));
		
		/** 
		 * Creates a cylinder with a cap centered at the origin and aligned along the z-axis
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the cylinder along the xy-plane.
		 * @param size Half of the length cylinder along the z-axis.
		 * @param slices Number of subdivisions around the z-axis.
		 * @param segments Number of subdivisions along the z-axis.
		 * @param rings Number of subdivisions on the caps.
		 * @param start Counterclockwise angle around the z-axis relative to x-axis.
		 * @param sweep Counterclockwise angle around the z-axis.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCappedCylinder(
			std::string name, 
			float radius   = 1.0f, 
			float size     = 1.f, 
			int   slices   = 32, 
			int   segments = 1, 
			int   rings    = 1, 
			float start    = 0.0f, 
			float sweep    = radians(360.0));
		
		/** 
		 * Creates a tube (a cylinder with thickness) with caps on both ends, centered at the origin and aligned along the z-axis.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius The outer radius of the cylinder on the xy-plane.
		 * @param innerRadius The inner radius of the cylinder on the xy-plane.
		 * @param size Half of the length of the cylinder along the z-axis.
		 * @param slices Number nubdivisions around the z-axis.
		 * @param segments Number of subdivisions along the z-axis.
		 * @param rings Number radial subdivisions in the cap.
		 * @param start Counterclockwise angle around the z-axis relative to the x-axis.
		 * @param sweep Counterclockwise angle around the z-axis.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCappedTube(
			std::string name, 
			float radius      = 1.0, 
			float innerRadius = 0.75, 
			float size        = 1.0, 
			int   slices      = 32, 
			int   segments    = 8, 
			int   rings       = 1, 
			float start       = 0.0, 
			float sweep       = radians(360.0));
		
		/** 
		 * Creates a capsule (a cylinder with spherical caps) centered at the origin and aligned along the z-axis.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the capsule on the xy-plane.
		 * @param size Half of the length between centers of the caps along the z-axis.
		 * @param slices Number of subdivisions around the z-axis in the caps.
		 * @param segments Number radial subdivisions in the cylinder.
		 * @param rings Number of radial subdivisions in the caps.
		 * @param start Counterclockwise angle relative to the x-axis.
		 * @param sweep Counterclockwise angle.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCapsule(
			std::string name, 
			float radius   = 1.0, 
			float size     = 0.5, 
			int   slices   = 32, 
			int   segments = 4, 
			int   rings    = 8, 
			float start    = 0.0, 
			float sweep    = radians(360.0));
		
		/** 
		 * Creates a cone centered at the origin, and whose tip points towards the z-axis.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the negative z end on the xy-plane.
		 * @param size Half of the length of the cylinder along the z-axis.
		 * @param slices Number of subdivisions around the z-axis.
		 * @param segments Number subdivisions along the z-axis.
		 * @param start Counterclockwise angle around the z-axis relative to the x-axis.
		 * @param sweep Counterclockwise angle around the z-axis.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCone(
			std::string name, 
			float radius   = 1.0, 
			float size     = 1.0,
			int   slices   = 32,
			int   segments = 8,
			float start    = 0.0, 
			float sweep    = radians(360.0));
		
		/** 
		 * Creates a convex polygonal disk with an arbitrary number of corners. 
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius The radius the enclosing circle.
		 * @param sides The number of sides. Should be >= 3. If <3 an empty mesh is generated.
		 * @param segments The number of segments per side. Should be >= 1. If zero an empty mesh is generated.
		 * @param rings The number of radial segments. Should be >= 1. = yields an empty mesh.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createConvexPolygonFromCircle(
			std::string name,
			float radius   = 1.0,
			int   sides    = 5,
			int   segments = 4,
			int   rings    = 4);

		/** 
		 * Creates a convex polygon from a set of corner vertices.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param vertices The corner coplanar vertex coordinates. Should form a convex polygon.
		 * @param segments The number of segments per side. Should be >= 1. If zero an empty mesh is generated.
		 * @param rings The number of radial segments. Should be >= 1. = yields an empty mesh.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createConvexPolygon(
			std::string name,
			std::vector<glm::vec2> vertices,
			int segments = 1,
			int rings    = 1 
			);
		
		/** 
		 * Creates an uncapped cylinder centered at the origin and aligned along the z-axis
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the cylinder along the xy-plane.
		 * @param size Half of the length of the cylinder along the z-axis.
		 * @param slices Subdivisions around the z-axis.
		 * @param segments Subdivisions along the z-axis.
		 * @param start Counterclockwise angle around the z-axis relative to the x-axis.
		 * @param sweep Counterclockwise angle around the z-axis.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createCylinder(
			std::string name,
			float radius   = 1.0,
			float size     = 1.0,
			int   slices   = 32,
			int   segments = 8,
			float start    = 0.0,
			float sweep    = radians(360.0)
		);

		/** 
		 * Creates a circular disk centered at the origin and along the xy-plane
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Outer radius of the disk on the xy-plane.
		 * @param innerRadius radius of the inner circle on the xy-plane.
		 * @param slices Number of subdivisions around the z-axis.
		 * @param rings Number of subdivisions along the radius.
		 * @param start Counterclockwise angle relative to the x-axis
		 * @param sweep Counterclockwise angle.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createDisk(
			std::string name,
			float radius      = 1.0,
			float innerRadius = 0.0,
			int   slices      = 32,
			int   rings       = 4,
			float start       = 0.0,
			float sweep       = radians(360.0));

		/** 
		 * Creates a regular dodecahedron centered at the origin and with a given radius.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * Each face is optionally subdivided along the edges and/or radius.
		 * @param radius The radius of the enclosing sphere.
		 * @param segments The number segments along each edge. Should be >= 1. If <1 empty mesh is generated.
		 * @param rings The number of radial segments on each face. Should be >= 1. If <1 an empty mesh is generated.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createDodecahedron(
			std::string name,
			float radius   = 1.0, 
			int   segments = 1, 
			int   rings    = 1);

		/** 
		 * Creates a plane (a regular grid) on the xy-plane whose normal points towards the z-axis.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param size Half of the side length in x (0) and y (1) direction.
		 * @param segments Number of subdivisions in the x (0) and y (1) direction.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createPlane(
			std::string name,
			vec2 size     = vec2(1.0, 1.0),
			ivec2 segments = ivec2(8, 8));

		/** 
		 * Creates a regular icosahedron centered at the origin and with a given radius.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius The radius of the enclosing sphere.
		 * @param segments The number segments along each edge. Must be >= 1.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createIcosahedron(
			std::string name,
			float radius   = 1.0, 
			int   segments = 1);

		/** 
		 * Creates an icosphere, otherwise known as a spherical subdivided icosahedron.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius The radius of the containing sphere.
		 * @param segments The number of segments per icosahedron edge. Must be >= 1.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createIcosphere(
			std::string name,
			float radius  = 1.0, 
			int  segments = 4);

		// /** 
		//  * Creates a mesh component from a procedural parametric mesh. (TODO: accept a callback which given an x and y position 
		// 	returns a Z hightfield) */
		// static Mesh* createParametricMesh(std::string name);

		/** 
		 * Creates a rectangular box with rounded edges, centered at the origin and aligned along the x, y, and z axis.
		 *
		 * @param name The name (used as a primary key) for this mesh component
		 * @param radius Radius of the rounded edges.
		 * @param size Half of the side length in x (0), y (1) and z (2) direction.
		 * @param slices Number subdivions around in the rounded edges.
		 * @param segments Number of subdivisions in x (0), y (1) and z (2) direction for the flat faces.
		 * @returns a reference to the mesh component
		 */
		static Mesh* createRoundedBox(
			std::string name, 
			float radius   = 0.25, 
			vec3  size     = vec3(0.75f, 0.75f, 0.75f), 
			int   slices   = 4, 
			ivec3 segments = ivec3(1,1,1));
	
		/** 
		 * Creates a sphere of the given radius, centered around the origin, subdivided around the z-axis in slices and along the z-axis in segments.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param radius The radius of the sphere
		 *  @param slices Subdivisions around the z-axis (longitudes).
		 *  @param segments Subdivisions along the z-axis (latitudes).
		 *  @param sliceStart Counterclockwise angle around the z-axis relative to x-axis.
		 *  @param sliceSweep Counterclockwise angle.
		 *  @param segmentStart Counterclockwise angle relative to the z-axis.
		 *  @param segmentSweep Counterclockwise angle.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createSphere(
			std::string name, 		
			float radius       = 1.0,
			int   slices       = 32,
			int   segments     = 16,
			float sliceStart   = 0.0,
			float sliceSweep   = radians(360.0),
			float segmentStart = 0.0,
			float segmentSweep = radians(180.0)
		);

		/** 
		 * Creates a cone with a spherical cap, centered at the origin and whose tip points towards the z-axis.
		 *  Each point on the cap has equal distance from the tip.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param radius Radius of the negative z end on the xy-plane.
		 *  @param size Half of the distance between cap and tip along the z-axis.
		 *  @param slices Number of subdivisions around the z-axis.
		 *  @param segments Number subdivisions along the z-axis.
		 *  @param rings Number subdivisions in the cap.
		 *  @param start Counterclockwise angle around the z-axis relative to the positive x-axis.
		 *  @param sweep Counterclockwise angle around the z-axis.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createSphericalCone(
			std::string name,
			float radius   = 1.0,
			float size     = 1.0,
			int   slices   = 32,
			int   segments = 8,
			int   rings    = 4,
			float start    = 0.0,
			float sweep    = radians(360.0));

		/** 
		 * Creates a triangular region on the surface of a sphere.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param radius Radius of the containing sphere.
		 *  @param segments Number of subdivisions along each edge.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createSphericalTriangleFromSphere(
			std::string name,
			float radius   = 1.0,
			int   segments = 4);

		/** 
		 * Creates a triangular region on the surface of a sphere.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param v0 First of the three counter-clockwise triangle vertices
		 *  @param v1 Second of the three counter-clockwise triangle vertices
		 *  @param v2 Third of the three counter-clockwise triangle vertices
		 *  @param segments Number of subdivisions along each edge.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createSphericalTriangleFromTriangle(
			std::string name,
			vec3 v0, vec3 v1, vec3 v2,
			int segments = 4);

		/** 
		 * Creates a spring aligned along the z-axis and with a counter-clockwise winding.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param minor Radius of the spring it self.
		 *  @param major Radius from the z-axis
		 *  @param size Half of the length along the z-axis.
		 *  @param slices Subdivisions around the spring.
		 *  @param segments Subdivisions along the path.
		 *  @param majorStart Counterclockwise angle around the z-axis relative to the x-axis.
		 *  @param majorSweep Counterclockwise angle around the z-axis.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createSpring(
			std::string name,
			float minor      = 0.25,
			float major      = 1.0,
			float size       = 1.0,
			int   slices     = 8,
			int   segments   = 32,
			float minorStart = 0.0,
			float minorSweep = radians(360.0),
			float majorStart = 0.0,
			float majorSweep = radians(720.0));

		/** 
		 * Creates the Utah Teapot using the original b-spline surface data. (https://en.wikipedia.org/wiki/Utah_teapot)
		 *  The lid is points towards the z axis and the spout points towards the x axis.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param segments The number segments along each patch. Should be >= 1. If zero empty mesh is generated.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTeapotahedron(
			std::string name, 
			int segments = 8);

		/** 
		 * Creates a torus centered at the origin and along the xy-plane.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param minor Radius of the minor (inner) ring
		 *  @param major Radius of the major (outer) ring
		 *  @param slices Subdivisions around the minor ring
		 *  @param segments Subdivisions around the major ring
		 *  @param minorStart Counterclockwise angle relative to the xy-plane.
		 *  @param minorSweep Counterclockwise angle around the circle.
		 *  @param majorStart Counterclockwise angle around the z-axis relative to the x-axis.
		 *  @param majorSweep Counterclockwise angle around the z-axis.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTorus(
			std::string name,
			float minor      = 0.25,
			float major      = 1.0,
			int   slices     = 16,
			int   segments   = 32,
			float minorStart = 0.0,
			float minorSweep = radians(360.0),
			float majorStart = 0.0,
			float majorSweep = radians(360.0));

		/** 
		 * Creates a circle extruded along the path of a knot. (https://en.wikipedia.org/wiki/Torus_knot)
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param p First coprime integer.
		 *  @param q Second coprime integer.
		 *  @param slices Number subdivisions around the circle.
		 *  @param segments Number of subdivisions around the path.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTorusKnot(
			std::string name,
			int p        = 2,
			int q        = 3,
			int slices   = 8,
			int segments = 96);


		/** 
		 * Creates a triangle centered at the origin and contained within the circumscribed circle.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param radius The radius of the containing circle.
		 *  @param segments The number of segments along each edge. Must be >= 1.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTriangleFromCircumscribedCircle(
			std::string name,
			float radius   = 1.0, 
			int   segments = 4);

		/** 
		 * Creates a triangle from the specified vertices
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param v0 First of the vertex positions of the triangle.
		 *  @param v1 Second of the vertex positions of the triangle.
		 *  @param v2 Third of the vertex positions of the triangle.
		 *  @param segments The number of segments along each edge. Must be >= 1.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTriangle(
			std::string name,
			vec3 v0, vec3 v1, vec3 v2,
			int segments = 4);

		/** 
		 * Creates an uncapped tube (a cylinder with thickness) centered at the origin and aligned along the z-axis.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param radius The outer radius of the cylinder on the xy-plane.
		 *  @param innerRadius The inner radius of the cylinder on the xy-plane.
		 *  @param size Half of the length of the cylinder along the z-axis.
		 *  @param slices Subdivisions around the z-axis.
		 *  @param segments Subdivisions along the z-axis.
		 *  @param start Counterclockwise angle around the z-axis relative to the x-axis.
		 *  @param sweep Counterclockwise angle around the z-axis.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTube(
			std::string name,
			float radius      = 1.0,
			float innerRadius = 0.75,
			float size        = 1.0,
			int   slices      = 32,
			int   segments    = 8,
			float start       = 0.0,
			float sweep       = radians(360.0));

		/** 
		 * Creates a tube from a circle extruded linearly along the specified path.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param path A set of vertices describing a linear path.
		 *  @param radius The radius of the extruded circle
		 *  @param segments Number of subdivisions around the circle.
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createTubeFromPolyline(
			std::string name, 
			std::vector<glm::vec3> path, 
			float     radius = 1.0, 
			int  segments = 16);

		/** 
		 * Creates a tube from a rounded rectangle extruded linearly along the specified path.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param path A set of vertices describing a linear path.
		 *  @param radius The radius of the rounded corners
		 *  @param size Half of the length of an edge
		 *  @param slices Number of subdivisions in each rounded corner
		 *  @param segments Number of subdivisions along each edge
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createRoundedRectangleTubeFromPolyline(
			std::string name, 
			std::vector<glm::vec3> path, 
			float radius = 1.0, 
			vec2 size = vec2(.75, .75),
			int slices = 4,
			ivec2 segments = ivec2(8, 8));

		/** 
		 * Creates a tube from a rounded rectangle extruded linearly along the specified path.
		 * 
		 *  @param name The name (used as a primary key) for this mesh component
		 *  @param path A set of vertices describing a linear path.
		 *  @param size Half of the length of an edge
		 *  @param segments Number of subdivisions along each edge
		 *  @returns a reference to the mesh component
		 */
		static Mesh* createRectangleTubeFromPolyline(
			std::string name, 
			std::vector<glm::vec3> path, 
			vec2 size = vec2(1., 1.),
			ivec2 segments = ivec2(8, 8));

		/** 
		 * Creates a mesh component from an OBJ file (ignoring any .mtl files) 
		 * 
		 * @param name The name (used as a primary key) for this mesh component
		 * @param path A path to the OBJ file.
		*/
		static Mesh* createFromObj(std::string name, std::string path);

		// /* Creates a mesh component from an ASCII STL file */
		// static Mesh* createFromStl(std::string name, std::string stlPath);

		// /* Creates a mesh component from a GLB file (material properties are ignored) */
		// static Mesh* createFromGlb(std::string name, std::string glbPath);

		// /* Creates a mesh component from TetGen node/element files (Can be made using "Mesh::tetrahedrahedralize") */
		// static Mesh* createFromTetgen(std::string name, std::string path);
		
		/**
		 * Creates a mesh component from a set of positions, optional normals, optional colors, optional texture coordinates, 
		 * and optional indices. If anything other than positions is supplied (eg normals), that list must be the same length
		 * as the point list. If indicies are supplied, indices must be a multiple of 3 (triangles). Otherwise, all other
		 * supplied per vertex data must be a multiple of 3 in length. 
		 * 
		 * @param name The name (used as a primary key) for this mesh component
		 * @param positions A list of 3D vertex positions. If indices aren't supplied, this must be a multiple of 3.
		 * @param normals A list of 3D vertex normals. If indices aren't supplied, this must be a multiple of 3.
		 * @param colors A list of per-vertex colors. If indices aren't supplied, this must be a multiple of 3.
		 * @param texcoords A list of 2D per-vertex texture coordinates. If indices aren't supplied, this must be a multiple of 3.
		 * @param indices A list of integer indices connecting vertex positions in a counterclockwise ordering to form triangles. If supplied, indices must be a multiple of 3.
		 * @returns a reference to the mesh component
		*/
		static Mesh* createFromData(
			std::string name,
			std::vector<glm::vec4> positions, 
			std::vector<glm::vec4> normals = std::vector<glm::vec4>(), 
			std::vector<glm::vec4> colors = std::vector<glm::vec4>(), 
			std::vector<glm::vec2> texcoords = std::vector<glm::vec2>(), 
			std::vector<uint32_t> indices = std::vector<uint32_t>());

		/**
		 * @param name The name of the Mesh to get
		 * @returns a Mesh who's name matches the given name 
		 */
        static Mesh* get(std::string name);

        /** @returns a pointer to the table of MeshStructs required for rendering */
        static MeshStruct* getFrontStruct();

        /** @returns a pointer to the table of mesh components */
        static Mesh* getFront();

        /** @returns the number of allocated meshes */
        static uint32_t getCount();

		/** @returns the name of this component */
		std::string getName();
		
		/** @returns A map whose key is a mesh name and whose value is the ID for that mesh */
		static std::map<std::string, uint32_t> getNameToIdMap();

		/** @param name The name of the Mesh to remove */
        static void remove(std::string name);

        /** Allocates the tables used to store all mesh components */
        static void initializeFactory();

        /** @returns True if the tables used to store all mesh components have been allocated, and False otherwise */
        static bool isFactoryInitialized();

		/** @returns True the current mesh is a valid, initialized mesh, and False if the mesh was cleared or removed. */
		bool isInitialized();

        /** Iterates through all mesh components, computing mesh metadata for rendering purposes. */
        static void updateComponents();

        /** Clears any existing Mesh components. */
        static void clearAll();

		/** Indicates whether or not any meshes are "out of date" and need to be updated through the "update components" function*/
		static bool areAnyDirty();

        /** @returns True if the mesh has been modified since the previous frame, and False otherwise */
        bool isDirty() { return dirty; }

        /** @returns True if the mesh has not been modified since the previous frame, and False otherwise */
        bool isClean() { return !dirty; }

        /** Tags the current component as being modified since the previous frame. */
        void markDirty();

        /** Tags the current component as being unmodified since the previous frame. */
        void markClean() { dirty = false; }
		
		/** @returns a json string representation of the current component */
		std::string toString();
		
		/** @returns a list of per vertex positions */
		std::vector<glm::vec4> getVertices();

		/** @returns a list of per vertex colors */
		std::vector<glm::vec4> getColors();

		/** @returns a list of per vertex normals */
		std::vector<glm::vec4> getNormals();

		/** @returns a list of per vertex texture coordinates */
		std::vector<glm::vec2> getTexCoords();

		// /* Returns a list of edge indices */
		// std::vector<uint32_t> get_edge_indices();

		/** @returns a list of triangle indices */
		std::vector<uint32_t> getTriangleIndices();

		// /* Returns a list of tetrahedra indices */
		// std::vector<uint32_t> get_tetrahedra_indices();		

		// /* Returns the handle to the position buffer */
		// vk::Buffer get_point_buffer();

		// /* Returns the handle to the per vertex colors buffer */
		// vk::Buffer get_color_buffer();

		// /* Returns the handle to the triangle indices buffer */
		// vk::Buffer get_triangle_index_buffer();

		// /* Returns the handle to the per vertex normals buffer */
		// vk::Buffer get_normal_buffer();

		// /* Returns the handle to the per vertex texcoords buffer */
		// vk::Buffer get_texcoord_buffer();

		// /* Returns the total number of edge indices used by this mesh. 
		// 	Divide by 2 to get the number of edges.  */
		// uint32_t get_total_edge_indices();

		// /* Returns the total number of triangle indices used by this mesh. 
		// 	Divide by 3 to get the number of triangles.  */
		// uint32_t get_total_triangle_indices();

		// /* Returns the total number of tetrahedral indices used by this mesh. 
		// 	Divide by 4 to get the number of tetrahedra.  */
		// uint32_t get_total_tetrahedra_indices();

		// /* Returns the total number of bytes per index */
		// uint32_t get_index_bytes();

		/** 
		 * Computes the average of all vertex positions. (centroid) 
		 * as well as min/max bounds and bounding sphere data. 
		*/
		void computeMetadata();

		// /* TODO: Explain this */
		// void save_tetrahedralization(float quality_bound, float maximum_volume);

		/** @returns the last computed mesh centroid. */
		glm::vec3 getCentroid();

		/** @returns the minimum axis aligned bounding box position */
		glm::vec3 getMinAabbCorner();

		/** @returns the maximum axis aligned bounding box position */
		glm::vec3 getMaxAabbCorner();

		/** @returns the center of the aligned bounding box */
		glm::vec3 getAabbCenter();

		/** @returns the radius of a sphere centered at the centroid which completely contains the mesh */
		float getBoundingSphereRadius();

		// /* If mesh editing is enabled, replaces the position at the given index with a new position */
		// void edit_position(uint32_t index, glm::vec4 new_position);

		// /* If mesh editing is enabled, replaces the set of positions starting at the given index with a new set of positions */
		// void edit_positions(uint32_t index, std::vector<glm::vec4> new_positions);

		// /* If mesh editing is enabled, replaces the normal at the given index with a new normal */
		// void edit_normal(uint32_t index, glm::vec4 new_normal);

		// /* If mesh editing is enabled, replaces the set of normals starting at the given index with a new set of normals */
		// void edit_normals(uint32_t index, std::vector<glm::vec4> new_normals);

		/**
		 * Replaces any existing normals with per-vertex smooth normals computed by 
		 * averaging neighboring geometric face normals together. 
		 * Note that this does not take into account the surface area of each triangular face.
		*/
		void generateSmoothNormals();

		// /* If mesh editing is enabled, replaces the vertex color at the given index with a new vertex color */
		// void edit_vertex_color(uint32_t index, glm::vec4 new_color);

		// /* If mesh editing is enabled, replaces the set of vertex colors starting at the given index with a new set of vertex colors */
		// void edit_vertex_colors(uint32_t index, std::vector<glm::vec4> new_colors);

		// /* If mesh editing is enabled, replaces the texture coordinate at the given index with a new texture coordinate */
		// void edit_texture_coordinate(uint32_t index, glm::vec2 new_texcoord);

		// /* If mesh editing is enabled, replaces the set of texture coordinates starting at the given index with a new set of texture coordinates */
		// void edit_texture_coordinates(uint32_t index, std::vector<glm::vec2> new_texcoords);
		
		// /* If RTX Raytracing is enabled, builds a low level BVH for this mesh. */
		// void build_low_level_bvh(bool submit_immediately = false);

		// /* TODO */
		// vk::AccelerationStructureNV get_low_level_bvh();
		
		// uint64_t get_low_level_bvh_handle();

		// vk::GeometryNV get_nv_geometry();

		// /* TODO */
		// void show_bounding_box(bool should_show);

		// /* TODO */
		// bool should_show_bounding_box();

		/** For internal use. Returns the mutex used to lock entities for processing by the renderer. */
		static std::shared_ptr<std::mutex> getEditMutex();

	private:
		/** Creates an uninitialized mesh. Useful for preallocation. */
		Mesh();

		/** Creates a mesh with the given name and id. */
		Mesh(std::string name, uint32_t id);

		/* TODO */
		static std::shared_ptr<std::mutex> editMutex;
		
		/* TODO */
		static bool factoryInitialized;
		
		/** A list of the mesh components, allocated statically */
		static Mesh meshes[MAX_MESHES];
		static MeshStruct meshStructs[MAX_MESHES];

		/** A lookup table of name to mesh id */
		static std::map<std::string, uint32_t> lookupTable;

		// /* Lists of per vertex data. These might not match GPU memory if editing is disabled. */
		std::vector<glm::vec4> positions;
		std::vector<glm::vec4> normals;
		std::vector<glm::vec4> colors;
		std::vector<glm::vec2> texCoords;
		// std::vector<uint32_t> tetrahedra_indices;
		std::vector<uint32_t> triangleIndices;
		// std::vector<uint32_t> edge_indices;

		// /* A handle to the buffer containing per vertex positions */
		// vk::Buffer pointBuffer;
		// vk::DeviceMemory pointBufferMemory;
		// uint32_t pointBufferSize;

		// /* A handle to the buffer containing per vertex colors */
		// vk::Buffer colorBuffer;
		// vk::DeviceMemory colorBufferMemory;
		// uint32_t colorBufferSize;

		// /* A handle to the buffer containing per vertex normals */
		// vk::Buffer normalBuffer;
		// vk::DeviceMemory normalBufferMemory;
		// uint32_t normalBufferSize;

		// /* A handle to the buffer containing per vertex texture coordinates */
		// vk::Buffer texCoordBuffer;
		// vk::DeviceMemory texCoordBufferMemory;
		// uint32_t texCoordBufferSize;

		// /* A handle to the buffer containing triangle indices */		
		// vk::Buffer triangleIndexBuffer;
		// vk::DeviceMemory triangleIndexBufferMemory;
		// uint32_t triangleIndexBufferSize;

		// /* An RTX geometry handle */
		// vk::GeometryNV geometry;

		// /* An RTX handle to the low level acceleration structure */
		// vk::AccelerationStructureNV lowAS;
		// vk::DeviceMemory lowASMemory;
		// uint64_t ASHandle;

		// /* True if the low level BVH was built. (TODO: make false if mesh edits were submitted) */
		// bool lowBVHBuilt = false;
		
		// /* True if this mesh component supports editing. If false, indices are automatically generated. */
		// bool allowEdits = false;

		/* Frees any resources this mesh component may have allocated */
		// void cleanup();

		// /** Creates a generic vertex buffer object */
		// uint64_t createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &bufferMemory);

		// /** Creates a position buffer, and uploads position data stored in the positions list */
		// void createPointBuffer(bool allow_edits, bool submit_immediately);

		// /** Creates a per vertex color buffer, and uploads per vertex color data stored in the colors list */
		// void createColorBuffer(bool allow_edits, bool submit_immediately);

		// /** Creates a normal buffer, and uploads normal data stored in the normals list */
		// void createNormalBuffer(bool allow_edits, bool submit_immediately);

		// /** Creates a texture coordinate buffer, and uploads texture coordinate data stored in the texture coordinates list */
		// void createTexCoordBuffer(bool allow_edits, bool submit_immediately);

		// /** Creates an index buffer, and uploads index data stored in the indices list */
		// void createTriangleIndexBuffer(bool allow_edits, bool submit_immediately);

		// /* Loads in an OBJ mesh and copies per vertex data to the GPU */
		void loadObj(std::string objPath);

		// /* Loads in an STL mesh and copies per vertex data to the GPU */
		// void load_stl(std::string stlPath);

		// /* Loads in a GLB mesh and copies per vertex data to the GPU */
		// void load_glb(std::string glbPath);

		// /* TODO: Explain this */
		// void load_tetgen(std::string path);

		/* Copies per vertex data to the GPU */
		void loadData (
			std::vector<glm::vec4> &positions, 
			std::vector<glm::vec4> &normals, 
			std::vector<glm::vec4> &colors, 
			std::vector<glm::vec2> &texcoords,
			std::vector<uint32_t> indices
		);
		
		/** Creates a procedural mesh from the given mesh generator, and copies per vertex to the GPU */
		template <class Generator>
		void generateProcedural(Generator &mesh, bool flip_z)
		{
			std::lock_guard<std::mutex>lock(*editMutex.get());

			std::vector<Vertex> vertices;

			auto genVerts = mesh.vertices();
			while (!genVerts.done()) {
				auto vertex = genVerts.generate();
				positions.push_back(glm::vec4(vertex.position.x, vertex.position.y, vertex.position.z, 1.0f));
				if (flip_z)
					normals.push_back(glm::vec4(-vertex.normal.x, -vertex.normal.y, -vertex.normal.z, 0.0f));
				else
					normals.push_back(glm::vec4(vertex.normal.x, vertex.normal.y, vertex.normal.z, 0.0f));
				texCoords.push_back(vertex.texCoord);
				colors.push_back(glm::vec4(0.0, 0.0, 0.0, 0.0));
				genVerts.next();
			}

			auto genTriangles = mesh.triangles();
			while (!genTriangles.done()) {
				auto triangle = genTriangles.generate();
				triangleIndices.push_back(triangle.vertices[0]);
				triangleIndices.push_back(triangle.vertices[1]);
				triangleIndices.push_back(triangle.vertices[2]);
				genTriangles.next();
			}

			computeMetadata();
		}

		/* Indicates that one of the components has been edited */
		static bool anyDirty;

		/* Indicates this component has been edited */
		bool dirty = true;
};
