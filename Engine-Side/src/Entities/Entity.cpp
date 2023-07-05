#include "Entities/Grid.h"
#include "Entities/Tile.h"
#include "Entities/Entity.h"
#include "Entities/Collision.h"
#include "Rendering/renderer.h"
#include "Rendering/renderList.h"
#include "Rendering/DebugRenderer.h"
#include "Resources/ResourceManager.h"

void Entity::Draw(divil::RenderList* aRenderList)
{
	if (mTexture)
	{
		aRenderList->DrawTexturedSprite(mTransform.mSize.x * mTransform.mScale.x, mTransform.mSize.x * mTransform.mScale.y,
			glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y),
			mTransform.mRotation,divil::ResourceManager::GetTexture(mTexture), divil::SpriteLayers::SpriteLayers_Game);
	}
	else
	{
		aRenderList->DrawSprite(mTransform.mSize.x * mTransform.mScale.x, mTransform.mSize.x * mTransform.mScale.y,
			glm::vec2(mTransform.mPosition.x, mTransform.mPosition.y),
			mTransform.mRotation, glm::vec3(0, 1, 0), divil::SpriteLayers::SpriteLayers_Game);
	}
}

// aDrawShape(default: 1): 0 => Square, 1 => Circle
void Entity::DebugDraw(int aDrawShape)
{
	glm::vec2 size = { mTransform.mSize.x * 0.5f * mTransform.mScale.x, mTransform.mSize.y * 0.5f * mTransform.mScale.y };
	switch (aDrawShape)
	{
	case 0:
		DebugRenderer::sInstance().drawSquare(mTransform.mPosition.x, mTransform.mPosition.y, mTransform.mSize.x, mTransform.mSize.y);
		break;
	case 1:
		DebugRenderer::sInstance().drawCircle(mTransform.mPosition.x + size.x, mTransform.mPosition.y + size.y, mCollider.mRadius, DebugColor::White, CircleResolution::medium);
		break;
	default:
		// Don't do anything if the number doesn't mean anything
		break;
	}
}

void Entity::OnCollision(Entity*)
{
	// Base collision
}