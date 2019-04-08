#pragma once
#include "Saitama.h"
#include "GameObject.h"

class IRenderer
{
public:
    virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB) = 0;
    virtual HRESULT VOnRestore() = 0;
    virtual void VShutdown() = 0;
    virtual bool VPreRender() = 0;
    virtual bool VPostRender() = 0;
    //virtual void VCalcLighting(Lights *lights, int maximumLights) = 0;
    virtual void VSetWorldTransform(const Matrix *m) = 0;
    virtual void VSetViewTransform(const Matrix *m) = 0;
    virtual void VSetProjectionTransform(const Matrix *m) = 0;
    //virtual shared_ptr<IRenderState> VPrepareAlphaPass() = 0;
    //virtual shared_ptr<IRenderState> VPrepareSkyBoxPass() = 0;
    virtual void VDrawLine(const Vector3& from, const Vector3& to, const Color& color) = 0;
};