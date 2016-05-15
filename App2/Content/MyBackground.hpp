
#pragma once

#include <exception>
#include <SpriteBatch.h>
#include <thread>
#include <wrl.h>
#include "Keyboard.h"
#include "Player.hpp"


using namespace DirectX;

class MyBackground
{
public:
	MyBackground() :
		mScreenHeight(0),
		mScreenWidth(0),
		mTextureWidth(0),
		mTextureHeight(0),
		mScreenPos(0, 0),
		mTextureSize(0, 0),
		mOrigin(0, 0)

	{
	}
	MyBackground(Player* mplayer) :
		mScreenHeight(0),
		mScreenWidth(0),
		mTextureWidth(0),
		mTextureHeight(0),
		mScreenPos(0, 0),
		mTextureSize(0, 0),
		mOrigin(0, 0)
		
		

	{
		player = mplayer;
		
	}


	void Load(ID3D11ShaderResourceView* texture)
	{
		mTexture = texture;

		if (texture)
		{
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			texture->GetResource(resource.GetAddressOf());

			D3D11_RESOURCE_DIMENSION dim;
			resource->GetType(&dim);

			if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
				throw std::exception("ScrollingBackground expects a Texture2D");

			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
			resource.As(&tex2D);

			D3D11_TEXTURE2D_DESC desc;
			tex2D->GetDesc(&desc);

			mTextureWidth = desc.Width;
			mTextureHeight = desc.Height;

			mTextureSize.x = float(desc.Width);
			mTextureSize.y = float( desc.Height );
			mTextureSize.y = 0.f; //Wrong - loss of usefull data
		//	mOrigin.y = desc.Width / 2.f;
		//	mOrigin.x = desc.Width / 2.f;
			mOrigin.x = 0.f;
			mOrigin.y = 0.f;
			
			
			TempPlayerPos.x = 0;
			TempPlayerPos.y = 0;
			
		}
	}

	void SetWindow(int screenWidth, int screenHeight)
	{
		mScreenHeight = screenHeight;
		mScreenWidth = screenWidth;

		//mScreenPos.x = float( screenWidth ) / 2.f;
		mScreenPos.x = -30.f;
		//mScreenPos.y = float( screenHeight ) / 2.f;
		mScreenPos.y = -30.f;

		scalingFactor.x = ((float)mScreenWidth / (float)mTextureWidth)*3;
		scalingFactor.y = ((float)mScreenHeight / (float)mTextureHeight)*3;

		player->setPosition((float)mScreenWidth/2, (float)mScreenHeight/2);
	}

	void Update()
	{	

		// Scrolling Background by Keyboard (moving player on the map )

		XMFLOAT2 PlayerPos = player->getPosition();

		std::unique_ptr<Keyboard::KeyboardStateTracker> tracker(new Keyboard::KeyboardStateTracker);
		auto keyboardState = Keyboard::Get().GetState();
		tracker->Update(keyboardState);
		if (tracker->pressed.S)
		{

			if (mScreenPos.y + mScreenHeight >= (-((float)mTextureHeight*scalingFactor.y + 10) / 3) && TempPlayerPos.y == 0)
				mScreenPos.y -= 10;
			else {
				PlayerPos.y += 10;
				TempPlayerPos.y += 10;
			}

		}

		if (tracker->pressed.W)
		{
			if (mScreenPos.y <= -10 && TempPlayerPos.y==0)
				mScreenPos.y += 10;
			else {
				PlayerPos.y -= 10;
				TempPlayerPos.y -= 10;
			}
		}

		if (tracker->pressed.A)
		{
			if (mScreenPos.x < -10 && TempPlayerPos.x == 0)
				mScreenPos.x += 10;
			else { 
				PlayerPos.x -= 10;
				TempPlayerPos.x -= 10;
			}
		}
		if (tracker->pressed.D)
		{
			if (mScreenPos.x+mScreenWidth >= (-((float)mTextureWidth*scalingFactor.x + 10)/3) && TempPlayerPos.x == 0)

				mScreenPos.x -= 10;
			else {
				PlayerPos.x += 10;
				TempPlayerPos.x += 10;
			}
		}
		player->setPosition(PlayerPos);



	}

	void Draw(DirectX::SpriteBatch* batch)
	{

		XMVECTOR screenPos = XMLoadFloat2(&mScreenPos);
		XMVECTOR origin = XMLoadFloat2(&mOrigin);
		XMVECTOR scale = XMLoadFloat2(&scalingFactor);


		batch->Draw(mTexture.Get(), screenPos, nullptr,
			Colors::White, 0.f, origin, scale, SpriteEffects_None, 0.f);


		XMVECTOR textureSize = XMLoadFloat2(&mTextureSize); //TODO:edit the vector to zero one dimmension, but not lose data

		batch->Draw(mTexture.Get(), XMLoadFloat2(&XMFLOAT2(((float)mTextureWidth*scalingFactor.x), 0)), nullptr,
			Colors::White, 0.f, origin, scale, SpriteEffects_None, 0.f);

	}

private:
	int                                                 mScreenHeight;
	int													mScreenWidth;
	int                                                 mTextureWidth;
	int                                                 mTextureHeight;
	DirectX::XMFLOAT2                                   mScreenPos;
	DirectX::XMFLOAT2                                   mTextureSize;
	DirectX::XMFLOAT2                                   mOrigin;
	DirectX::XMFLOAT2									scalingFactor;
	DirectX::XMFLOAT2									TempPlayerPos;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    mTexture;
	std::unique_ptr<Keyboard::KeyboardStateTracker>		tracker;
	Player*												player;
	//std::unique_ptr<Player>								player;
};