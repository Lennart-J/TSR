/*
 * Copyright (C) 2018 by Author: Aroudj, Samir
 * TU Darmstadt - Graphics, Capture and Massively Parallel Computing
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _SURFACE_EXTRACTION_CONE_NODES_CHECKER_H_
#define _SURFACE_EXTRACTION_CONE_NODES_CHECKER_H_

#include "CollisionDetection/ObliqueCircularCone.h"
#include "SurfaceReconstruction/Scene/Tree/NodesChecker.h"

namespace SurfaceReconstruction
{
	/// Nodes checker to test tree nodes for overlap with a specific cone.
	class ConeNodesChecker : public CollisionDetection::ObliqueCircularCone, public NodesChecker
	{
	public:
		ConeNodesChecker(const uint32 sampleIdx, const Math::Vector3 &apexPosWS, const Real endRadius, const Real length, 
			const uint32 maxDepth, const uint32 *nodeStates, const uint32 requiredFlags);

		/** Defines the cone which is then used for overlap tests against scene tree node AABBs.
		@param apex This is where the cone originates / where its sharp end is.
		@param endOutsideNormal Set this to define the orientation of the circular discs centered at the main axis from the apex to the end center position.
			This is the normal of the circular disc at getEndCenter() and supposed to point outside at the cone end.
		@param endCenter Set this to where the main cone axis ends (= center of the last & biggest circular disc of the cone).
		@param endRadius Defines the radius of the circular disc at endCenterPosition (radius of last / biggest circular disc of the cone) and thus defines the "width of the cone". */
		ConeNodesChecker(const Math::Vector3 &apex,	const Math::Vector3 &endOutsideNormal, const Math::Vector3 &endCenter, const Real endRadius, const Real length,
			const uint32 maxDepth, const uint32 *nodeStates, const uint32 requiredFlags);

		/** Frees resources. */
		virtual ~ConeNodesChecker();

		/** Tests whether a node is overplaps with the 3D support volume of this oblique circular cone.
		@param scope Identifies and describes the extent of the node which is tested for intersection within this spherical nodes checker.
		@param isLeaf This should be set to true if scope is the scope of a leaf.
		@return Returns true when the cone of this nodes checker overlaps with the AABB of scope otherwise false is returned. */
		virtual bool isIgnored(const Scope &scope, const bool isLeaf, const uint32 depth) const;

	private:
		const uint32 *mNodeStates;
		const uint32 mRequiredFlags;
	};
}

#endif // _SURFACE_EXTRACTION_CONE_NODES_CHECEKR_H_