#include "pch.h"
#include "MovementController.h"
#include "SceneNode.h"

static const float ROTATION_GAIN = 0.01f;
static const float MOVEMENT_GAIN = 0.5f;

MovementController::MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, shared_ptr<DirectX::Keyboard> keyboard, shared_ptr<DirectX::Mouse> mouse)
    : m_object(object)
{
    m_object->VGet()->Transform(&m_matToWorld, &m_matFromWorld);

    m_fTargetYaw = m_fYaw = (-initialYaw);
    m_fTargetPitch = m_fPitch = (initialPitch);

    Vector3 pos = m_matToWorld.Translation();

    m_matPosition = m_matPosition.CreateTranslation(pos);

    m_keyboard = keyboard;
    m_mouse = mouse;
    //POINT ptCursor;
    //GetCursorPos(&ptCursor);
    //m_lastMousePos = ptCursor;

    //memset(m_bKey, 0x00, sizeof(m_bKey));

    //m_mouseLButtonDown = false;
    //m_bRotateWhenLButtonDown = rotateWhenLButtonDown;
}

//  class MovementController::OnUpdate			- Chapter 10, page 283

void MovementController::OnUpdate(const float deltaMilliseconds)
{
    //if (m_bKey['Q'])
    //{
    //	// This code is a cheat to position the camera exactly in a given
    //	// spot so I can take screen shots!

    //	Mat4x4 camTranslate;
    //	D3DXMatrixTranslation(&m_matPosition, 8.847f, 7.055f, 11.618f);

    //	m_fTargetYaw = m_fYaw += -64.35f;
    //	m_fTargetPitch = m_fPitch = 28.57f;

    //	// Calculate the new rotation matrix from the camera
    //	// yaw and pitch.
    //	Mat4x4 matRot;
    //	D3DXMatrixRotationYawPitchRoll(&matRot, DEGREES_TO_RADIANS(m_fYaw), DEGREES_TO_RADIANS(m_fPitch), 0);

    //	// Create the new object-to-world matrix, and the
    //	// new world-to-object matrix. 
    //	D3DXMatrixMultiply(&m_matToWorld, &matRot, &m_matPosition);
    //	D3DXMatrixInverse(&m_matFromWorld, NULL, &m_matToWorld);

    //	m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    //	return;
    //}
    auto kb = m_keyboard->GetState();
    auto mouse = m_mouse->GetState();
    bool bTranslating = false;
    Vector4 atWorld(0, 0, 0, 0);
    Vector4 rightWorld(0, 0, 0, 0);
    Vector4 upWorld(0, 0, 0, 0);

    if (kb.W || kb.S)
    {
        // In D3D, the "look at" default is always
        // the positive Z axis.
        Vector4 at = g_Forward4;
        if (kb.S)
            at *= -1;

        // This will give us the "look at" vector 
        // in world space - we'll use that to move
        // the camera.
        atWorld = DirectX::XMVector4Transform(at, m_matToWorld);
        //atWorld = m_matToWorld.Transform (at);
        bTranslating = true;
    }

    if (kb.A || kb.D)
    {
        // In D3D, the "right" default is always
        // the positive X axis.
        Vector4 right = g_Right4;
        if (kb.A)
            right *= -1;

        // This will give us the "right" vector 
        // in world space - we'll use that to move
        // the camera
        rightWorld = DirectX::XMVector4Transform(right, m_matToWorld);
        //rightWorld = m_matToWorld.Xform(right);
        bTranslating = true;
    }

    if (kb.C || kb.X)
    {
        // In D3D, the "up" default is always
        // the positive Y axis.
        Vector4 up = g_Up4;
        if (kb.C)
            up *= -1;

        //Unlike strafing, Up is always up no matter
        //which way you are looking
        upWorld = up;
        bTranslating = true;
    }

    //Handling rotation as a result of mouse position
    {
        if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
        {
            DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3(float(mouse.x), float(mouse.y), 0.f)
                ;

            m_fTargetYaw = m_fTargetYaw - (delta.x);
            m_fTargetPitch = m_fTargetPitch + (delta.y);
            m_fYaw += (m_fTargetYaw - m_fYaw) * (.35f);
            m_fTargetPitch = std::max(-90, std::min(90, (int)m_fTargetPitch));
            m_fPitch += (m_fTargetPitch - m_fPitch) * (.35f);

            Matrix matRot;
            matRot = matRot.CreateFromYawPitchRoll(DirectX::XMConvertToRadians(-m_fYaw), DirectX::XMConvertToRadians(m_fPitch), 0);

            // Create the new object-to-world matrix, and the
            // new world-to-object matrix. 
            m_matToWorld = matRot * m_matPosition;
            m_matFromWorld = m_matToWorld.Invert();
            m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
        }
        m_mouse->SetMode(mouse.rightButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE);
    }

    if (bTranslating)
    {

        Vector3 direction = atWorld + rightWorld + upWorld;
        direction.Normalize();

        direction *= MOVEMENT_GAIN;

        Vector3 pos = m_matPosition.Translation() + direction;
        m_matPosition = m_matPosition.CreateTranslation(pos);        
        m_matToWorld.Translation(pos);

        m_matFromWorld = m_matToWorld.Invert();
        m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    }
}

