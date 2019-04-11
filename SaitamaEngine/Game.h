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

    //GameObjectFactory factory;
    

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

    Timer m_fpsTimer;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
  Camera *camera;

  std::unique_ptr<DirectX::Keyboard> m_keyboard;
  std::unique_ptr<DirectX::Mouse> m_mouse;

  std::unique_ptr<DirectX::GeometricPrimitive> m_room;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_roomTex;
  float m_pitch;
  float m_yaw;
};