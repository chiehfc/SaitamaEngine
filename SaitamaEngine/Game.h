//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Timer.h"
#include "GameObjectFactory.h"
#include "D3DRenderer11.h"
#include "Scene.h"
#include "MovementController.h"

class EventManager;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

	DirectX::SimpleMath::Matrix CreateLookAtFunc(const DirectX::SimpleMath::Vector3 & eye, const DirectX::SimpleMath::Vector3 & target, const DirectX::SimpleMath::Vector3 & up);

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

   
    //Graphics gfx;
    std::shared_ptr<D3DRenderer11> renderer;

    Scene *scene;

    // Event manager
    EventManager *m_pEventManager;

    shared_ptr<CameraNode> m_pCamera;
    
    shared_ptr<MovementController> m_pObjectController;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    Timer m_fpsTimer;


  std::shared_ptr<DirectX::Keyboard> m_keyboard;
  std::shared_ptr<DirectX::Mouse> m_mouse;

  float m_pitch;
  float m_yaw;
};