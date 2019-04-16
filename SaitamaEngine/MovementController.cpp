#include "pch.h"
#include "MovementController.h"
#include "SceneNode.h"

static const float ROTATION_GAIN = 0.01f;
static const float MOVEMENT_GAIN = 20.0f;

MovementController::MovementController(shared_ptr<SceneNode> object, float initialYaw, float initialPitch, bool rotateWhenLButtonDown, shared_ptr<DirectX::Keyboard> keyboard, shared_ptr<DirectX::Mouse> mouse)
    : m_object(object)
{
    m_object->VGet()->Transform(&m_matToWorld, &m_matFromWorld);

    //m_fTargetYaw = m_fYaw = RADIANS_TO_DEGREES(-initialYaw);
    //m_fTargetPitch = m_fPitch = RADIANS_TO_DEGREES(initialPitch);

    m_maxSpeed = 30.0f;			// 30 meters per second
    m_currentSpeed = 0.0f;

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

void MovementController::OnUpdate(DWORD const deltaMilliseconds)
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

    //if (m_bKey['W'] || m_bKey['S'])
    //{
    //    // In D3D, the "look at" default is always
    //    // the positive Z axis.
    //    Vec4 at = g_Forward4;
    //    if (m_bKey['S'])
    //        at *= -1;

    //    // This will give us the "look at" vector 
    //    // in world space - we'll use that to move
    //    // the camera.
    //    atWorld = m_matToWorld.Xform(at);
    //    bTranslating = true;
    //}

    //if (m_bKey['A'] || m_bKey['D'])
    //{
    //    // In D3D, the "right" default is always
    //    // the positive X axis.
    //    Vec4 right = g_Right4;
    //    if (m_bKey['A'])
    //        right *= -1;

    //    // This will give us the "right" vector 
    //    // in world space - we'll use that to move
    //    // the camera
    //    rightWorld = m_matToWorld.Xform(right);
    //    bTranslating = true;
    //}

    //if (m_bKey[' '] || m_bKey['C'] || m_bKey['X'])
    //{
    //    // In D3D, the "up" default is always
    //    // the positive Y axis.
    //    Vec4 up = g_Right4;
    //    if (!m_bKey[' '])
    //        up *= -1;

    //    //Unlike strafing, Up is always up no matter
    //    //which way you are looking
    //    upWorld = up;
    //    bTranslating = true;
    //}

    //Handling rotation as a result of mouse position
    {
        if (mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
        {
            DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3(float(mouse.x), float(mouse.y), 0.f)
                * ROTATION_GAIN;


            Matrix matRot;
            matRot = matRot.CreateFromYawPitchRoll(delta.y, delta.x, 0);

            // Create the new object-to-world matrix, and the
            // new world-to-object matrix. 

            m_matToWorld = matRot * m_matToWorld;
            m_matFromWorld = m_matToWorld.Invert();
            m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
        }
        m_mouse->SetMode(mouse.rightButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE);


        //// The secret formula!!! Don't give it away!
        ////If you are seeing this now, then you must be some kind of elite hacker!
        //m_fYaw += (m_fTargetYaw - m_fYaw) * (.35f);
        //m_fTargetPitch = MAX(-90, MIN(90, m_fTargetPitch));
        //m_fPitch += (m_fTargetPitch - m_fPitch) * (.35f);

        //// Calculate the new rotation matrix from the camera
        //// yaw and pitch.
        //Mat4x4 matRot;
        //matRot.BuildYawPitchRoll(DEGREES_TO_RADIANS(-m_fYaw), DEGREES_TO_RADIANS(m_fPitch), 0);

        //// Create the new object-to-world matrix, and the
        //// new world-to-object matrix. 

        //m_matToWorld = matRot * m_matPosition;
        //m_matFromWorld = m_matToWorld.Inverse();
        //m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    }

    //if (bTranslating)
    //{
    //    float elapsedTime = (float)deltaMilliseconds / 1000.0f;

    //    Vec3 direction = atWorld + rightWorld + upWorld;
    //    direction.Normalize();

    //    // Ramp the acceleration by the elapsed time.
    //    float numberOfSeconds = 5.f;
    //    m_currentSpeed += m_maxSpeed * ((elapsedTime*elapsedTime) / numberOfSeconds);
    //    if (m_currentSpeed > m_maxSpeed)
    //        m_currentSpeed = m_maxSpeed;

    //    direction *= m_currentSpeed;

    //    Vec3 pos = m_matPosition.GetPosition() + direction;
    //    m_matPosition.SetPosition(pos);
    //    m_matToWorld.SetPosition(pos);

    //    m_matFromWorld = m_matToWorld.Inverse();
    //    m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
    //} else
    //{
    //    m_currentSpeed = 0.0f;
    //}
}

