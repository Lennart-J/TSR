/*
 * Copyright (C) 2018 by Author: Aroudj, Samir
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <vector>
#include "Graphics/Color.h"
#include "Graphics/Camera3D.h"
#include "Math/Polybezier.h"
#include "Math/Vector3.h"
#include "SurfaceReconstruction/Geometry/Edge.h"
#include "SurfaceReconstruction/Geometry/Triangle.h"
#include "SurfaceReconstruction/Scene/IReconstructorObserver.h"
#include "SurfaceReconstruction/Scene/Scene.h"

namespace SurfaceReconstruction
{
	class Cameras;
	class FlexibleMesh;
	class Mesh;
	class MeshRefiner;
	class Scope;
	class StaticMesh;

	struct TreeIntersectionTriangle
	{
	public:
		TreeIntersectionTriangle();

	public:
		uint32 mTriangleIdx;
		std::vector<uint32> mLeaves;
	};

	/// Visualizes 2D reconstruction related data such as surfaces and cameras.
	class Renderer : public IReconstructorObserver
	{
	public:
		/** Enables switching between different ways to render cameras. */
		enum CameraRendering
		{
			CAMERA_RENDERING_INVISIBLE,		/// Don't render any camera.
			CAMERA_RENDERING_POINTS,		/// Render only points to represent the cameras.
			CAMERA_RENDERING_ARROWS,		/// Render the cameras by means of simple arrows indicating their orientation.
			CAMERA_RENDERING_RAY_BUNDLES,	/// Render the cameras as arrows and also add a line for each sample between it and its center of projection.
			CAMERA_RENDERING_COUNT			/// Defines the number of different ways to render cameras.
		};

		enum LeafResultsRendering
		{
			LEAF_RESULTS_RENDERING_EMPTY		= 0x1 << 0,
			LEAF_RESULTS_RENDERING_NEAR_SURFACE	= 0x1 << 1
		};

		// todo
		enum MeshRefinerRendering
		{
			MESH_REFINER_PCS_MOVEMENTS = 0x1 << 0
		};

		enum OccupancyRenderingFlags
		{
			OCCUPANCY_RENDERING_FLAGS_EMPTINESS		= 0x1 << 0,
			OCCUPANCY_RENDERING_FLAGS_OCCUPANCY		= 0x1 << 1,
			OCCUPANCY_RENDERING_FLAGS_SAMPLENESS	= 0x1 << 2,
			OCCUPANCY_RENDERING_FLAGS_LOG_SCALE		= 0x1 << 31
		};

		/** Enables switching between different ways to render surface samples. */
		enum SampleRendering
		{
			SAMPLE_RENDERING_INVISIBLE,				/// Don't render any sample.
			SAMPLE_RENDERING_POINTS_DATA_COLOR,		/// Render samples by means of simple points with colors attached to them, e.g. colors loaded from file.
			SAMPLE_RENDERING_POINTS_NORMAL_COLOR,	/// Render samples by means of simple points with colors from normals.
			//SAMPLE_RENDERING_PLANES,				/// Render samples by means of a tangential plane and a small line representing their normal.
			SAMPLE_RENDERING_INVALID,				/// Identifies an invalid way to render a sample. If this is chosen the default rendering is used.
			SAMPLE_RENDERING_COUNT					/// Defines the number of different ways to render samples.
		};

		/** Flags that define what data of a Tree object is rendered. */
		enum SceneTreeRendering
		{
			SCENE_TREE_RENDERING_SHOW_LEAVES				= 0x1 << 0,
			SCENE_TREE_RENDERING_SHOW_LEAF_NEIGHBORHOODS	= 0x1 << 1
		};

	public:
		/** Creates the renderer and configures rendering colors. */
		Renderer();

		/** Releases resources. */
		virtual ~Renderer();

		/** Sets which camera is rendered with highlight color.
		@param cameraIdx Identifies which camera is rendered with highlight color. E.g., set this to a selected camera.
			Make sure that cameraIdx < cameras.getCount().
		@see cameras to be rendered are entered in render(cameras). */
		inline void highlightCamera(uint32 cameraIdx);

		/** Increases the ID of the camera to be highlighted to emphasize the "next" camera.
		@param cameraCount Set this to the number of cameras in order to highlight
			the first camera (ID 0) if the last camera (count - 1) is the highlighted camera upon this call. */
		inline void highlightNextCamera(uint32 cameraCount);
		
		virtual bool onNewReconstruction(FlexibleMesh *mesh,
			const uint32 iteration, const std::string &text, const Scene::ReconstructionType type, const bool responsible);

		/** Renders a scene. (surfaces, cameras, samples etc.)
		@param Set this to the camera from which you want to see the scene. This defines the camera and projection matrix to be used.
		@param scaleFactor Applies a uniform scaling matrix to all vertices in camera space in order to achieve a zooming effect. */
		void render(const Real scaleFactor);

		/** todo */
		void render(const Mesh &mesh, bool perFaceNormal = false);

		void renderCursor(const Math::Vector2 &cursorPosNDC, const Graphics::Color &color, const Real cursorSize);
		

		/** Changes the way cameras are rendered. It simply switches to the "next mode".
		@see See CameraRendering enumeration for possible modes. */
		inline void shiftCameraRendering();

		/** Changes the way samples are rendered. It simply switches to the "next mode".
		@see See SampleRendering enumeration for possible modes.*/
		inline void shiftSampleRendering();

		/** todo */	
		inline void showEdgeNeighbors(const uint32 edgeIdx);

		/** todo */
		inline void showDualCell(const uint32 index);

		inline void showNextEdgeNeighbors();

		inline void showNextTreeIntersectionTriangle();

		/** todo */
		inline void showNextReconstruction();

		/** todo */
		inline void showNextTriangleNeighbors();

		/** todo */
		inline void showNodes(const uint32 nodeIdx);

		/** todo */
		inline void showPreviousEdgeNeighbors();

		inline void showPreviousTreeIntersectionTriangle();

		/** todo */
		inline void showPreviousTriangleNeighbors();
		
		void showTreeIntersectionTriangle(const uint32 triangleIdx);

		/** todo */
		inline void showTriangleNeighbors(const uint32 centerTriangleIdx);

		/** todo 
		@param localNeighborIdx Should be in {0, 1, 2} since each triangle of a mannifold mesh can have up to 3 neighbors. */
		void showTriangleNeighborsOfNeighbor(const FlexibleMesh &mesh, uint32 localNeighborIdx);

		/** todo */
		inline void toggleBackfaceCulling();
		
		/** Toggles rendering of SurfaceCrust data.
		@param flags Set this to elements of the enumeration Renderer::SURFACE_CRUST_RENDERING 
			to specify what proxy data visibilities are toggled*/
		inline void toggleCrustRendering(const uint32 flags);

		/** Toggles objects' ground truth surface visibility. */
		inline void toggleGroundTruthVisibility();

		inline void toggleLeafResultsRendering(const uint32 flags);

		/** Toggles whether normals of meshes are rendered or not. */
		inline void toggleMeshNormalsVisibility();

		/** Toggles what MeshRefiner data is rendered. 
		@flags See MESH_REFINER_RENDERING for possibly rendered data. */
		inline void toggleMeshRefinerRendering(const uint32 flags);

		/** Changes the way the Occupancy is rendered. It simply switches to the "next mode".
		@see See OCCUPANCY_RENDERING enumeration for possible modes. */
		inline void toggleOccupancyRenderingFlags(OccupancyRenderingFlags flags);

		/** Toggles rendering of Tree elements.
		@param flags Set this to elements of the enumeration Renderer::SCENE_TREE_RENDERING
			to specify what tree element visibilities are toggled. */
		inline void toggleTreeRendering(const uint32 sceneTreeRenderingFlags);

		/** */
		void update();

	private:
		/** Enter all cameras that shall be rendered.
		@param cameras All contained cameras are rendered according to the current camera rendering mode.
		@see shiftCameraRendering() changes the way cameras are rendered and CameraRendering enumeration defines possible ways to render them. */
		void render(const Cameras &cameras);

		/** Renders all samples identified by the entered vector with a specific color. Is used to render a single camera.
		@param samples Set this to the set of samples to be rendered.
		@param highlighted Set this to true to use a lighter sample color. */
		void render(const std::vector<uint32> &sampleSet, bool highlighted = false);

		/** Renders all cameras or a specific one.
		@param cameras Set this to the objecect representing all registered cameras
		@param cameraIdx Set this to -1 to render all cameras and to some index to only render this particular camera.
		@param color Set this to the 4D color with which cameras are rendered. (OpenGL format) */
		void render(const Cameras &cameras, uint32 cameraIdx, const float *color);

		/** Enter the ground truth surfaces here which shall be shown.
		@param curves Defines the ground truth surfaces that shall be shown.
		@param partitions If this is not NULL then partitions are rendered in a way that makes them distinguishable.
		@param curveCount set this to the number of curves / polybeziers in the array curves.*/
		void render(const Math::Polybezier<Math::Vector3> *curves, const std::vector<uint32> *partitions, uint32 curveCount);

		/** todo */
		void renderArrowVertices(const Math::Vector3 &arrowHeadWS, const Math::Vector3 &middleVector, const Math::Vector3 &orthogonal0, const Math::Vector3 &orthogonal1);

		/** todo */
		void renderDualCubeVertices(const Math::Vector3 *vertices);

		// mesh rendering functions
		/** todo */
		void renderGroundTruthMesh();

		/** Displays an implicit function.*/
		void renderLeafResults();

		/** Displays important data regarding variational mesh refinement.
		@see MeshRefiner */
		void renderMeshRefinementData();

		/** todo */
		void renderOrientedPoint(const Math::Vector3 &positionWS, const Math::Vector3 &normalWS) const;
		
		/** Renders a representation of the point of origin (v0 = [0/0]) of the world space coordinate system. */
		void renderOrigin();

		/** todo */
		void renderReconstructedMesh();

		/** todo */
		void renderSamples();

		/** todo */
		void renderSamplesOfChosenNode() const;

		/** todo */
		void renderNode(const Scope &node, bool filled) const;

		/** todo */
		void renderEdgeNeighbors(const FlexibleMesh &mesh) const;

		/** todo */
		void renderRangedVertexSets() const;
		
		void renderTreeIntersectionTriangle(const FlexibleMesh &mesh) const;

		/** todo */
		void renderTriangleNeighbors(const FlexibleMesh &mesh) const;

		/** Renders a single sample. Should be used within glBegin() and glEnd().
		@param sampleIdx Set this to the index of the sample to be rendered by some lines.
		@param highlighted Set this to true if the sample should be emphasized and rendered in a special way.
		@param sampleRendering Set this to overwrite the default sample rendering behaviour stored in mSampleRendering. */
		void renderSample(const uint32 sampleIdx,
			bool highlighted = false, SampleRendering sampleRendering = SAMPLE_RENDERING_INVALID) const;

		/** todo */
		void renderTree();

	public:
		static const Graphics::Color COLOR_CAMERA;		/// Defines the color for rendering of capturing cameras which create samples.
		static const Graphics::Color COLOR_HIGHLIGHTED;	/// Defines how the color of emphasized elements.
		static const Graphics::Color COLOR_SAMPLES[3];	/// Defines the colors for rendering of surface samples created by cameras. [0] -> evaluated ones, [1] -> unprocessed, [2] -> Mean Shift
		static const Graphics::Color COLOR_SURFACES[3];	/// Defines the colors for rendering of object surfaces. Each surface might be partitioned into subsets. 3 colors are required to render the subsets.
		static const Real CAMERA_SIZE;					/// Defines the camera length of each camera visualization. This is a scaling factor to control the size of the camera representations. 
		static const Real NORMAL_SIZE;					/// Defines the length of each normal visualization.
		
	private:
		TreeIntersectionTriangle mInterTriangle;	/// For test rendering of triangle and nodes/tree intersection data

		Real mLightAzimuth;				/// Stores the angle which is used to animate the lights. (They are rotating in a horizontal plane.)		
		float mElementSizes[2];			/// Default size/width of points and lines in pixels.
		
		uint32 mHighlightedCamera;		/// Defines the camera and thus its samples that are highlighted by a special color.
		uint32 mHighlightedSampleCount;	/// Defines how many samples are emphasized.
		uint32 mHighlightedSampleStart;	/// Defines the index of the first sample to be emphasized.
		
		uint32 mCrustFlags;				/// Defines how a SurfaceCrust object is rendered.
		uint32 mLeafResultsFlags;		/// Contains flags that define what implicit function data is rendered.
		uint32 mMeshRefinerFlags;		/// Defines what data of the MeshRefiner object is shown.
		uint32 mNeighborsSize;		/// Contains the number of elements of a neighbors to be rendered.
		uint32 mOccupancyFlags;			/// Defines how the free space is rendered.
		uint32 mTreeFlags;				/// Defines how a Tree object is rendered.
		
		uint32 mShownDualCell;			/// Defines what cell of the Dual Scene Octree is rendered.
		uint32 mShownEdge;				/// Defines what edge of the currently shown reconstructed mesh is highlighted.
		uint32 mShownNodes;				/// Identifies a scene tree node data of which is rendered.
		uint32 mShownReconstruction;	/// Defines what reconstructed surface mesh is visualized (possibly none).
		uint32 mShownSample;			/// Identifies the sample for which additional information is rendered.
		uint32 mShownTriangle;			/// Identifies the reconstruction mesh triangle and its neighbors which are highlighted.
		
		CameraRendering mCameraRendering;	/// Defines how cameras are rendered.
		SampleRendering mSampleRendering;	/// Defines how samples are rendered.

		bool mBackfaceCulling;		/// Defines whether backface culling for faces and oriented points is done or not.
		bool mShowGroundTruth;		/// Defines whether the ground truth mesh is visualized or not.
		bool mShowMeshNormals;		/// Defines whether mesh normals are rendered or not.
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///   inline function definitions   ////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	inline void Renderer::highlightNextCamera(uint32 cameraCount)
	{
		++mHighlightedCamera;
		if (mHighlightedCamera >= cameraCount)
			mHighlightedCamera = 0;
	}

	inline void Renderer::highlightCamera(uint32 cameraID)
	{
		mHighlightedCamera = cameraID;
	}

	inline void Renderer::shiftCameraRendering()
	{
		mCameraRendering = (CameraRendering) (mCameraRendering + 1);
		if (mCameraRendering >= CAMERA_RENDERING_COUNT)
			mCameraRendering = CAMERA_RENDERING_INVISIBLE;
	}

	inline void Renderer::shiftSampleRendering()
	{
		mSampleRendering = (SampleRendering) (mSampleRendering + 1);
		if (mSampleRendering >= SAMPLE_RENDERING_INVALID)
			mSampleRendering = SAMPLE_RENDERING_INVISIBLE;
	}

	inline void Renderer::showDualCell(const uint32 index)
	{
		mShownDualCell = index;
	}

	inline void Renderer::showEdgeNeighbors(const uint32 edgeIdx)
	{
		mShownEdge = edgeIdx;
	}
	
	inline void Renderer::showNextEdgeNeighbors()
	{
		++mShownEdge;
		showEdgeNeighbors(mShownEdge);
	}

	inline void Renderer::showNextReconstruction()
	{
		++mShownReconstruction;
		if (mShownReconstruction > IReconstructorObserver::RECONSTRUCTION_TYPE_COUNT)
			mShownReconstruction = 0;
	}

	inline void Renderer::showNextTreeIntersectionTriangle()
	{
		++mInterTriangle.mTriangleIdx;
		showTreeIntersectionTriangle(mInterTriangle.mTriangleIdx);
	}

	inline void Renderer::showNextTriangleNeighbors()
	{
		++mShownTriangle;
		showTriangleNeighbors(mShownTriangle);
	}

	inline void Renderer::showPreviousEdgeNeighbors()
	{
		if (Edge::INVALID_INDEX == mShownEdge)
			return;

		--mShownEdge;
		showEdgeNeighbors(mShownEdge);
	}

	inline void Renderer::showPreviousTreeIntersectionTriangle()
	{
		if (-1 == mInterTriangle.mTriangleIdx)
			return;

		--mInterTriangle.mTriangleIdx;
		showTreeIntersectionTriangle(mInterTriangle.mTriangleIdx);
	}

	inline void Renderer::showPreviousTriangleNeighbors()
	{
		if (Triangle::INVALID_INDEX == mShownTriangle)
			return;

		--mShownTriangle;
		showTriangleNeighbors(mShownTriangle);
	}

	inline void Renderer::showNodes(const uint32 nodeIdx)
	{
		mShownNodes = nodeIdx;
	}
	
	inline void Renderer::showTriangleNeighbors(const uint32 centerTriangleIdx)
	{
		mShownTriangle = centerTriangleIdx;
	}

	inline void Renderer::toggleBackfaceCulling()
	{
		mBackfaceCulling = !mBackfaceCulling;
	}

	inline void Renderer::toggleCrustRendering(const uint32 flags)
	{
		mCrustFlags ^= flags;
	}

	inline void Renderer::toggleLeafResultsRendering(const uint32 flags)
	{
		mLeafResultsFlags ^= flags;
	}

	inline void Renderer::toggleGroundTruthVisibility()
	{
		mShowGroundTruth = !mShowGroundTruth;
	}
	
	inline void Renderer::toggleMeshNormalsVisibility()
	{
		mShowMeshNormals = !mShowMeshNormals;
	}

	inline void Renderer::toggleMeshRefinerRendering(const uint32 flags)
	{
		mMeshRefinerFlags ^= flags;
	}

	inline void Renderer::toggleOccupancyRenderingFlags(OccupancyRenderingFlags flags)
	{
		mOccupancyFlags ^= flags;
	}

	inline void Renderer::toggleTreeRendering(const uint32 sceneTreeRenderingFlags)
	{
		mTreeFlags ^= sceneTreeRenderingFlags;
	}
}

#endif // _RENDERER_H_
