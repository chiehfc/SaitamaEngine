//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <GeometricPrimitive.h>
#include <SimpleMath.h>
#include "Math.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Saitama;

using Microsoft::WRL::ComPtr;

static const XMVECTORF32 START_POSITION = { 0.f, -1.5f, 0.f, 0.f };
static const XMVECTORF32 ROOM_BOUNDS = { 8.f, 6.f, 12.f, 0.f };
static const float ROTATION_GAIN = 0.01f;
static const float MOVEMENT_GAIN = 20.0f;

Game::Game() :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);
    
    m_fpsTimer.Start();

    if (!gfx.Initialize(window, width, height))
    {
        return;
    }


    factory.CreateActor(nullptr, nullptr, nullptr, 0);

    //camera = new Camera();
    //camera->SetAngle(XM_PI / 4.f);
      //InitProjMatrix(camera->GetAngle(), m_outputWidth,m_outputHeight,1.0f,10.0f);

    //CreateDevice();

    //CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    Saitama::Vector3 v1(1, 2, 3);
    v1.Print();
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
    float dt = m_fpsTimer.GetMillisecondsElapsed();
    m_fpsTimer.Restart();

    // TODO: Add your game logic here.
    elapsedTime;

	float time = static_cast<float>(timer.GetTotalSeconds());

    gfx.GetGameObject()->AdjustRotation(DirectX::SimpleMath::Vector3(0.0f, 1.0f * ROTATION_GAIN, 0.0f));

  // Keyboard input.
  auto kb = m_keyboard->GetState();
  if (kb.Escape)
      PostQuitMessage(0);
  if (kb.W)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetForwardVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(gfx.GetCamera()->GetForwardVector() * MOVEMENT_GAIN * elapsedTime);
  if (kb.S)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetBackwardVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(gfx.GetCamera()->GetBackwardVector() * MOVEMENT_GAIN * elapsedTime);
  if (kb.A)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetLeftVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(gfx.GetCamera()->GetLeftVector() * MOVEMENT_GAIN * elapsedTime);
  if (kb.D)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetRightVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(gfx.GetCamera()->GetRightVector() * MOVEMENT_GAIN * elapsedTime);
  if (kb.R)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetForwardVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(DirectX::SimpleMath::Vector3(0.0f, MOVEMENT_GAIN * elapsedTime, 0.0f));
  if (kb.F)
      //gfx.GetGameModel()->AdjustPosition(gfx.GetGameModel()->GetForwardVector() * elapsedTime * MOVEMENT_GAIN);
      gfx.GetCamera()->AdjustPosition(DirectX::SimpleMath::Vector3(0.0f, -MOVEMENT_GAIN * elapsedTime, 0.0f));

  if (kb.C)
  {
      DirectX::SimpleMath::Vector3 lightPosition = gfx.GetCamera()->GetPositionVector();
      lightPosition += gfx.GetCamera()->GetForwardVector();
      gfx.GetLight()->SetPosition(lightPosition);
      gfx.GetLight()->SetRotation(gfx.GetCamera()->GetRotationVector());
  }

  // Mouse input.
  auto mouse = m_mouse->GetState();
  if (mouse.positionMode == Mouse::MODE_RELATIVE)
  {
      DirectX::SimpleMath::Vector3 delta = DirectX::SimpleMath::Vector3(float(mouse.x), float(mouse.y), 0.f)
            * ROTATION_GAIN;
      gfx.GetCamera()->AdjustRotation(DirectX::SimpleMath::Vector3(delta.y,delta.x, 0));
  }
  m_mouse->SetMode(mouse.rightButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    gfx.RenderFrame();

    //Clear();
   // camera->UpdateViewMatrix();
   // 
   // m_view = camera->GetView();
   // m_proj = camera->Proj();
   // // TODO: Add your rendering code here.
   // 
   // float y = sinf(m_pitch);
   // float r = cosf(m_pitch);
   // float z = r*cosf(m_yaw);
   // float x = r*sinf(m_yaw);
   // m_room->Draw(m_world, m_view, m_proj, Colors::White, m_roomTex.Get());

	  //m_shape->Draw(m_world, m_view, m_proj);

    //Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    

    //m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    //CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    //m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    //HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    /*if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }*/
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
//    
//
//#ifndef NDEBUG
//    ComPtr<ID3D11Debug> d3dDebug;
//    if (SUCCEEDED(device.As(&d3dDebug)))
//    {
//        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
//        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
//        {
//#ifdef _DEBUG
//            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
//            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
//#endif
//            D3D11_MESSAGE_ID hide [] =
//            {
//                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
//                // TODO: Add more message IDs here as needed.
//            };
//            D3D11_INFO_QUEUE_FILTER filter = {};
//            filter.DenyList.NumIDs = _countof(hide);
//            filter.DenyList.pIDList = hide;
//            d3dInfoQueue->AddStorageFilterEntries(&filter);
//        }
//    }
//#endif
//	
//    DX::ThrowIfFailed(device.As(&m_d3dDevice));
//    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).

  //m_shape = GeometricPrimitive::CreateCube(m_d3dContext.Get());
  //m_room = GeometricPrimitive::CreateBox(m_d3dContext.Get(), XMFLOAT3(ROOM_BOUNDS[0], ROOM_BOUNDS[1], ROOM_BOUNDS[2]), false, true);

  // Create room texture
  /*DX::ThrowIfFailed(
    CreateDDSTextureFromFile(m_d3dDevice.Get(), L"roomtexture.dds",
      nullptr, m_roomTex.ReleaseAndGetAddressOf()));

	m_world = Matrix::Identity;*/
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    /*m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();*/

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    //if (m_swapChain)
    //{
    //    HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

    //    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    //    {
    //        // If the device was removed for any reason, a new device and swap chain will need to be created.
    //        OnDeviceLost();

    //        // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
    //        // and correctly set up the new device.
    //        return;
    //    }
    //    else
    //    {
    //        DX::ThrowIfFailed(hr);
    //    }
    //}
    //else
    //{

    //    // Create a descriptor for the swap chain.
    //    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    //    swapChainDesc.Width = backBufferWidth;
    //    swapChainDesc.Height = backBufferHeight;
    //    swapChainDesc.Format = backBufferFormat;
    //    swapChainDesc.SampleDesc.Count = 1;
    //    swapChainDesc.SampleDesc.Quality = 0;
    //    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //    swapChainDesc.BufferCount = backBufferCount;

    //    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
    //    fsSwapChainDesc.Windowed = TRUE;

    //    // Create a SwapChain from a Win32 window.
    //    DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
    //        m_d3dDevice.Get(),
    //        m_window,
    //        &swapChainDesc,
    //        &fsSwapChainDesc,
    //        nullptr,
    //        m_swapChain.ReleaseAndGetAddressOf()
    //        ));

    //    // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
    //    DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    //}

    //// Obtain the backbuffer for this window which will be the final 3D rendertarget.
    //ComPtr<ID3D11Texture2D> backBuffer;
    //DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    //// Create a view interface on the rendertarget to use on bind.
    //DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    //DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    //DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
    
}


// TODO: My experimental stuffs
DirectX::SimpleMath::Matrix Game::CreateLookAtFunc(const SimpleMath::Vector3& eye, const SimpleMath::Vector3& target, const SimpleMath::Vector3& up)
{
  SimpleMath::Vector3 lookAtForward;
	(target - eye).Normalize(lookAtForward);

	return Matrix::Identity;
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_shape.reset();
  m_room.reset();
  m_roomTex.Reset();

   /* m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();*/

    CreateDevice();

    CreateResources();
}