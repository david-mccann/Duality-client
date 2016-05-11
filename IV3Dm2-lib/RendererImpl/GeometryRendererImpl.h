#pragma once

#include "Scene/ScreenInfo.h"
#include "Scene/G3D.h"
#include "RendererImpl/GLFrameBufferObject.h"
#include "RendererImpl/GLShader.h"

#include <memory>

class GeometryNode;

class GeometryRendererImpl {
public:
    GeometryRendererImpl(ScreenInfo screenInfo);
    void render(const GeometryNode& dataset);
    
private:
    static int primitiveTypeGL(const GeometryNode& dataset);
    static int enableAttributeArrays(const GeometryNode& geometry);
    GLShader& determineActiveShader(const GeometryNode& dataset) const;

private:
    ScreenInfo m_screenInfo;
    std::unique_ptr<GLFrameBufferObject> m_fbo;
    std::unique_ptr<GLShader> m_normShader;
    std::unique_ptr<GLShader> m_normAlphaShader;
    std::unique_ptr<GLShader> m_normTexAlphaShader;
    std::unique_ptr<GLShader> m_normColShader;
    std::unique_ptr<GLShader> m_colShader;
    std::unique_ptr<GLShader> m_normTexShader;
    std::unique_ptr<GLShader> m_texShader;
};