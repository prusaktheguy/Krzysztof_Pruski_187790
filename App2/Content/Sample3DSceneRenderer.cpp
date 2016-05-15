#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "DDSTextureLoader.h"


#include "..\Common\DirectXHelper.h"

using namespace App2;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	//m_degreesPerSecond(45),
	//m_indexCount(0),
	//m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	//float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		//fovAngleY *= 2.0f;
	}


	// MUST BE DONE FOR EVERY SPRITEBATCH
	m_sprites->SetRotation(m_deviceResources->ComputeDisplayRotation()); 

	spriteBatchT1->SetRotation(m_deviceResources->ComputeDisplayRotation());
	spriteBatchT2->SetRotation(m_deviceResources->ComputeDisplayRotation());



}


void Sample3DSceneRenderer::CreateAudioResources()
{

}

// Called once per frame
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{

	auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution







#pragma region Keyboard
	std::unique_ptr<Keyboard::KeyboardStateTracker> tracker(new Keyboard::KeyboardStateTracker);



	auto keyboardState = Keyboard::Get().GetState();

	tracker->Update(keyboardState);
	//XMFLOAT2 tempPos = player->getPosition();
	//if (tracker->pressed.S)
	//{

	//	tempPos.y += 10;
	//}

	//if (tracker->pressed.W)
	//{

	//	tempPos.y -= 10;
	//}

	//if (tracker->pressed.A)
	//{
	//	tempPos.x -= 10;
	//}
	//if (tracker->pressed.D)
	//{
	//	tempPos.x += 10;
	//}


	//player->setPosition(tempPos);

	background->Update();



#pragma endregion Handling Keyboard input






	//auto test = timer.GetElapsedSeconds();


	//update the animation
	//animation->Update((float)timer.GetElapsedSeconds());

	player->Update((float)timer.GetElapsedSeconds());














}

void Sample3DSceneRenderer::NewAudioDevice()
{

}

void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	//if (!m_loadingComplete)
	//{
	//	return;
	//}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Set render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	D3D11_TEXTURE2D_DESC pDesc;
	Microsoft::WRL::ComPtr<ID3D11Resource> res;
	//1.) -----------------
	//m_texture->GetResource(&res);
	//((ID3D11Texture2D*)res.Get())->GetDesc(&pDesc); // Usually dangerous!

	//2.) -----------------
	m_texture->GetResource(res.GetAddressOf());
	Microsoft::WRL::ComPtr<ID3D11Texture2D> text2D;
	res.As(&text2D);
	text2D->GetDesc(&pDesc);

	auto height = pDesc.Height; //texture height
	auto width = pDesc.Width; //texture width

	auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution

															// Draw sprites
	m_sprites->Begin();

	background->Draw(m_sprites.get());

	//Drawing walls

	player->Draw(m_sprites.get());



//	m_font->DrawString(m_sprites.get(), collisionString.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
	m_sprites->End();


}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Create DirectXTK objects
	auto device = m_deviceResources->GetD3DDevice();

	auto context = m_deviceResources->GetD3DDeviceContext();


	auto windowSize = m_deviceResources->GetOutputSize(); // physical screen resolution
	auto logicalSize = m_deviceResources->GetLogicalSize(); //DPI dependent resolution


	m_sprites.reset(new SpriteBatch(context));
	spriteBatchT1.reset(new SpriteBatch(context));
	spriteBatchT2.reset(new SpriteBatch(context));

//	m_font.reset(new SpriteFont(device, L"Assets\\italic.spritefont"));


	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Image-Player-Sprite-Sheet.dds", nullptr, m_texture.ReleaseAndGetAddressOf())
	);
	player.reset(new Player(m_texture.Get()));

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\bg2.dds", nullptr, backgroundTexture.ReleaseAndGetAddressOf())
	);
	background.reset(new MyBackground(player.get()));
	background->Load(backgroundTexture.Get());

	//TODO: Instatiate enemies here
	//Enemy enemyTemp(enemyTexture.Get());
	//enemiesVector.push_back(enemyTemp);


	//set windows size for drawing the background
	background->SetWindow(logicalSize.Width, logicalSize.Height);

	//Gamepad
	gamePad.reset(new GamePad);


}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{


	//TODO:
	m_sprites.reset();
	m_font.reset();
	m_texture.Reset();
	backgroundTexture.Reset();



}