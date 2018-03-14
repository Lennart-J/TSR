/*
* Copyright (C) 2018 by Author: Aroudj, Samir
* TU Darmstadt - Graphics, Capture and Massively Parallel Computing
* All rights reserved.
*
* This software may be modified and distributed under the terms
* of the BSD 3-Clause license. See the License.txt file for details.
*/

#ifndef _FILE_NAMING_H_
#define _FILE_NAMING_H_

#include "IReconstructorObserver.h"

namespace SurfaceReconstruction
{
	class FileNaming
	{
	public:
		// file beginnings
		static const char *BEGINNING_RESULTS;
		static const char *BEGINNING_VIEW_FOLDER;

		// file endings
		static const char *ENDING_CAMERAS;
		static const char *ENDING_COLOR_IMAGE;
		static const char *ENDING_DUAL_CELLS;
		static const char *ENDING_LEAVES;
		static const char *ENDING_MESH;
		static const char *ENDING_MVE_IMAGE;
		static const char *ENDING_NODES;
		static const char *ENDING_OCCUPANCY;
		static const char *ENDING_OCTREE;
		static const char *ENDING_PLY;
		static const char *ENDING_SAMPLES;
		static const char *ENDING_VIEW_FOLDER;
		
		// file name parts
		static const char *FILTERED_SAMPLES;
		static const char *RECONSTRUCTED_MESHES[IReconstructorObserver::RECONSTRUCTION_TYPE_COUNT];
		static const char *REORDERED_SAMPLES;
		static const char *RESULTS_FOLDER;

		// MVE naming
		static const char *CAMERA_META_FILE_NAME;

		// image tags for file naming
		static const char *IMAGE_TAG_COLOR;
		static const char *IMAGE_TAG_COLOR_S0;
		static const char *IMAGE_TAG_DEPTH;
		static const char *IMAGE_TAG_VIEWS;
	};
}

#endif // _FILE_NAMING_H_