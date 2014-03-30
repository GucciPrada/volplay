// This file is part of volplay, a library for interacting with
// volumetric data.
//
// Copyright (C) 2014 Christoph Heindl <christoph.heindl@gmail.com>
//
// This Source Code Form is subject to the terms of the BSD 3 license.
// If a copy of the BSD was not distributed with this file, You can obtain
// one at http://opensource.org/licenses/BSD-3-Clause.

#include <volplay/rendering/camera.h>
#include <volplay/rendering/renderer.h>
#include <volplay/rendering/image_generator.h>
#include <volplay/sdf_node.h>

namespace volplay {
    
    namespace rendering {
    
        Renderer::Renderer()
        : _imageWidth(0), _imageHeight(0)
        {}
        
        void
        Renderer::setCamera(const CameraPtr &cam)
        {
            _camera = cam;
        }
        
        void
        Renderer::setScene(const SDFNodePtr &root)
        {
            _root = root;
        }
        
        void
        Renderer::setImageResolution(int imageHeight, int imageWidth)
        {
            _imageHeight = imageHeight;
            _imageWidth = imageWidth;
        }
        
        int
        Renderer::imageHeight() const
        {
            return _imageHeight;
        }
        
        int
        Renderer::imageWidth() const
        {
            return _imageWidth;
        }
        
        void
        Renderer::setPrimaryTraceOptions(const SDFNode::TraceOptions &opts)
        {
            _primaryTraceOptions = opts;
        }
        
        const SDFNode::TraceOptions &
        Renderer::primaryTraceOptions() const
        {
            return _primaryTraceOptions;
        }
        
        void
        Renderer::addImageGenerator(const ImageGeneratorPtr &g)
        {
            _generators.push_back(g);
        }
        
        void
        Renderer::render()
        {
            if (!_camera || !_root || _imageWidth == 0|| _imageHeight == 0) {
                return;
            }
            
            // Prepare rays
            std::vector<Vector> rays;
            _camera->generateCameraRays(_imageHeight, _imageWidth, rays);
            AffineTransform::LinearPart t = _camera->cameraToWorldTransform().linear();
            
            // Prepare generators
            std::vector<ImageGeneratorPtr>::iterator gBegin = _generators.begin();
            std::vector<ImageGeneratorPtr>::iterator gEnd = _generators.end();
            std::vector<ImageGeneratorPtr>::iterator gi;
            
            for (gi = gBegin; gi != gEnd; ++gi) {
                (*gi)->onRenderingBegin(this);
            }
            
            ImageGenerator::PixelInfo pi;
            pi.origin = _camera->originInWorld();
            
            // For each row
            for (pi.row = 0; pi.row < _imageHeight; ++pi.row) {
                
                // Update generators
                for (gi = gBegin; gi != gEnd; ++gi) {
                    (*gi)->onRowBegin(pi.row);
                }
                
                const Vector *rayRow = &rays[pi.row * _imageWidth];
                
                for (pi.col = 0; pi.col < _imageWidth; ++pi.col) {
                    
                    // Trace primary ray
                    pi.direction = t * rayRow[pi.col];
                    _root->trace(pi.origin, pi.direction, _primaryTraceOptions, &pi.tr);
                    
                    // Update generators
                    for (gi = gBegin; gi != gEnd; ++gi) {
                        (*gi)->onUpdatePixel(pi);
                    }

                }
            }
            
            // Finish generators
            for (gi = gBegin; gi != gEnd; ++gi) {
                (*gi)->onRenderingComplete(this);
            }
        }
        
        
    }
}