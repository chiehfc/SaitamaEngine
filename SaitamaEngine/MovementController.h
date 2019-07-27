#pragma once

#include "Saitama.h"
class SceneNode;

////////////////////////////////////////////////////
//
// MovementController Description
//
//    Implements a WASD style movement controller
//
//  class MovementController		- Chapter 10, page 281
//
////////////////////////////////////////////////////

class MovementController
{
protected:
    Matrix  m_matFromWorld;
    Matrix	m_matToWorld;
    Matrix  m_matPosition;

    Matrix m_matScale;

    //Point					m_lastMousePos;
    //bool					m_bKey[256];			// Which keys are up and down

    // Orientation Controls
    float		m_fTargetYaw;
    float		m_fTargetPitch;
    float		m_fYaw;
    float		m_fPitch;
    float		m_fPitchOnDown;
    float		m_fYawOnDown;
    float		m_maxSpeed;
    float		m_currentSpeed;

    // Added for Ch19/20 refactor
    //bool		m_mouseLButtonDown;
    //bool		m_bRotateWhenLButtonDown;

    shared_ptr<SceneNode> m_object;

    shared_ptr<DirectX::Keyboard> m_keyboard;
    shared_ptr<DirectX::Mouse> m_mouse;

public:
    MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, shared_ptr<DirectX::Keyboard> keyboard, shared_ptr<DirectX::Mouse> mouse);
    void SetObject(shared_ptr<SceneNode> newObject);

    void OnUpdate(const float elapsedMs);

public:
    /*bool VOnPointerMove(const Point &mousePos, const int radius);
    bool VOnPointerButtonDown(const Point &mousePos, const int radius, const std::string &buttonName);
    bool VOnPointerButtonUp(const Point &mousePos, const int radius, const std::string &buttonName);

    bool VOnKeyDown(const BYTE c) { m_bKey[c] = true; return true; }
    bool VOnKeyUp(const BYTE c) { m_bKey[c] = false; return true; }*/

    const Matrix *GetToWorld() { return &m_matToWorld; }
    const Matrix *GetFromWorld() { return &m_matFromWorld; }
};
