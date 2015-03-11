// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/surface/dual_contouring.h>
#include <volplay/sdf_node.h>
#include <volplay/util/voxel_grid.h>

namespace volplay {
    
    namespace surface {

        DualContouring::DualContouring()
        {}

        IndexedSurface
        DualContouring::extractSurface(SDFNodePtr scene, const Vector &lower, const Vector &upper, const Vector &resolution)
        {
            return IndexedSurface();
        }

    }
    
    
}