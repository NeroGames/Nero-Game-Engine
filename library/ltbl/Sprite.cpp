#include "Sprite.hpp"

namespace ltbl
{

Sprite::Sprite()
	: BaseLight()
	, sf::Sprite()
	, mTexture(nullptr)
	, mNormalsTexture(nullptr)
	, mNeedRenderNormals(false)
{
}

void Sprite::setTexture(sf::Texture& texture, bool resetRect)
{
	sf::Sprite::setTexture(texture, resetRect);
	mTexture = &texture;
}

void Sprite::setNormalsTexture(sf::Texture& normalsTexture)
{
	mNormalsTexture = &normalsTexture;
}

const sf::Texture* Sprite::getNormalsTexture() const
{
	return mNormalsTexture;
}

void Sprite::render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(*this, states);
	mNeedRenderNormals = true;
}

void Sprite::renderNormals(sf::RenderTarget& target, sf::RenderStates states)
{
	if (mNormalsTexture != nullptr && mNeedRenderNormals)
	{
		sf::Sprite::setTexture(*mNormalsTexture);
		target.draw(*this, states);
		sf::Sprite::setTexture(*mTexture);
	}
	mNeedRenderNormals = false;
}

} // namespace ltbl