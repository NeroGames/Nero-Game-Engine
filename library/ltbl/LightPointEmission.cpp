#include "LightPointEmission.hpp"

namespace ltbl
{

LightPointEmission::LightPointEmission()
	: BaseLight()
	, mSprite()
	, mLocalCastCenter()
	, mSourceRadius(8.0f)
	, mShadowOverExtendMultiplier(1.4f)
{
}

void LightPointEmission::setTexture(sf::Texture& texture)
{
	mSprite.setTexture(texture);
	quadtreeAABBChanged();
}

const sf::Texture* LightPointEmission::getTexture() const
{
	return mSprite.getTexture();
}

void LightPointEmission::setTextureRect(const sf::IntRect& rect)
{
	mSprite.setTextureRect(rect);
	quadtreeAABBChanged();
}

const sf::IntRect& LightPointEmission::getTextureRect() const
{
	return mSprite.getTextureRect();
}

void LightPointEmission::setColor(const sf::Color& color)
{
	mSprite.setColor(color);
}

const sf::Color& LightPointEmission::getColor() const
{
	return mSprite.getColor();
}

const sf::Transform& LightPointEmission::getTransform() const
{
	return mSprite.getTransform();
}

void LightPointEmission::setPosition(const sf::Vector2f& position)
{
	mSprite.setPosition(position);
	quadtreeAABBChanged();
}

void LightPointEmission::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
	quadtreeAABBChanged();
}

void LightPointEmission::move(const sf::Vector2f& movement)
{
	mSprite.move(movement);
	quadtreeAABBChanged();
}

void LightPointEmission::move(float x, float y)
{
	mSprite.move(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getPosition() const
{
	return mSprite.getPosition();
}

void LightPointEmission::setRotation(float angle)
{
	mSprite.setRotation(angle);
	quadtreeAABBChanged();
}

void LightPointEmission::rotate(float angle)
{
	mSprite.rotate(angle);
	quadtreeAABBChanged();
}

float LightPointEmission::getRotation() const
{
	return mSprite.getRotation();
}

void LightPointEmission::setScale(const sf::Vector2f& scale)
{
	mSprite.setScale(scale);
	quadtreeAABBChanged();
}

void LightPointEmission::setScale(float x, float y)
{
	mSprite.setScale(x, y);
	quadtreeAABBChanged();
}

void LightPointEmission::scale(const sf::Vector2f& scale)
{
	mSprite.scale(scale);
	quadtreeAABBChanged();
}

void LightPointEmission::scale(float x, float y)
{
	mSprite.scale(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getScale() const
{
	return mSprite.getScale();
}

void LightPointEmission::setOrigin(const sf::Vector2f& origin)
{
	mSprite.setOrigin(origin);
	quadtreeAABBChanged();
}

void LightPointEmission::setOrigin(float x, float y)
{
	mSprite.setOrigin(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getOrigin() const
{
	return mSprite.getOrigin();
}

void LightPointEmission::render(const sf::View& view, sf::RenderTexture& lightTempTexture, sf::RenderTexture& antumbraTempTexture, sf::Shader& unshadowShader, sf::Shader& lightOverShapeShader, const std::vector<priv::QuadtreeOccupant*>& shapes, bool normalsEnabled, sf::Shader& normalsShader)
{
    float shadowExtension = mShadowOverExtendMultiplier * (getAABB().width + getAABB().height);

    struct OuterEdges 
	{
        std::vector<int> _outerBoundaryIndices;
        std::vector<sf::Vector2f> _outerBoundaryVectors;
    };

    std::vector<OuterEdges> outerEdges(shapes.size());

    std::vector<int> innerBoundaryIndices;
    std::vector<sf::Vector2f> innerBoundaryVectors;
    std::vector<priv::Penumbra> penumbras;

    //----- Emission

    lightTempTexture.clear();
    lightTempTexture.setView(view);

	if (normalsEnabled) 
	{
		auto oglLightPosition = lightTempTexture.mapCoordsToPixel(mSprite.getPosition(), view);
		normalsShader.setUniform("lightPosition", sf::Glsl::Vec3(static_cast<float>(oglLightPosition.x), static_cast<float>(lightTempTexture.getSize().y - oglLightPosition.y), 0.15f));

		const auto& lightColor = mSprite.getColor();
		normalsShader.setUniform("lightColor", sf::Glsl::Vec3(lightColor.r / 255.f, lightColor.g / 255.f, lightColor.b / 255.f));
		
		auto oglOrigin = lightTempTexture.mapCoordsToPixel({ 0.f, 0.f });
		auto oglLightWidthPos = lightTempTexture.mapCoordsToPixel({ getAABB().width, 0.f }) - oglOrigin;
		auto oglLightHeightPos = lightTempTexture.mapCoordsToPixel({ 0.f, getAABB().height }) - oglOrigin;
		float oglLightWidth = static_cast<float>(std::sqrt(oglLightWidthPos.x * oglLightWidthPos.x + oglLightWidthPos.y * oglLightWidthPos.y));
		float oglLightHeight = static_cast<float>(std::sqrt(oglLightHeightPos.x * oglLightHeightPos.x + oglLightHeightPos.y * oglLightHeightPos.y));
		normalsShader.setUniform("lightSize", sf::Glsl::Vec2(oglLightWidth, oglLightHeight));

		lightTempTexture.draw(mSprite, &normalsShader);
	}
	else 
	{
		lightTempTexture.draw(mSprite);
	}

    //----- Shapes

    // Mask off light shape (over-masking - mask too much, reveal penumbra/antumbra afterwards)
    unsigned int shapesCount = shapes.size();
    for (unsigned int i = 0; i < shapesCount; ++i) 
	{
        LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
		if (pLightShape->isAwake() && pLightShape->isTurnedOn())
		{
			// Get boundaries
			innerBoundaryIndices.clear();
			innerBoundaryVectors.clear();
			penumbras.clear();
			getPenumbrasPoint(penumbras, innerBoundaryIndices, innerBoundaryVectors, outerEdges[i]._outerBoundaryIndices, outerEdges[i]._outerBoundaryVectors, *pLightShape);

			if (innerBoundaryIndices.size() != 2 || outerEdges[i]._outerBoundaryIndices.size() != 2)
			{
				continue;
			}

			// Render shape
			if (!pLightShape->renderLightOver())
			{
				pLightShape->setColor(sf::Color::Black);
				lightTempTexture.draw(*pLightShape);
			}

			sf::Vector2f as = pLightShape->getTransform().transformPoint(pLightShape->getPoint(outerEdges[i]._outerBoundaryIndices[0]));
			sf::Vector2f bs = pLightShape->getTransform().transformPoint(pLightShape->getPoint(outerEdges[i]._outerBoundaryIndices[1]));
			sf::Vector2f ad = outerEdges[i]._outerBoundaryVectors[0];
			sf::Vector2f bd = outerEdges[i]._outerBoundaryVectors[1];

			sf::Vector2f intersectionOuter;

			// Handle antumbras as a seperate case
			if (priv::rayIntersect(as, ad, bs, bd, intersectionOuter))
			{
				sf::Vector2f asi = pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0]));
				sf::Vector2f bsi = pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1]));
				sf::Vector2f adi = innerBoundaryVectors[0];
				sf::Vector2f bdi = innerBoundaryVectors[1];

				antumbraTempTexture.clear(sf::Color::White);
				antumbraTempTexture.setView(view);

				sf::Vector2f intersectionInner;

				if (priv::rayIntersect(asi, adi, bsi, bdi, intersectionInner))
				{
					sf::ConvexShape maskShape;
					maskShape.setPointCount(3);
					maskShape.setPoint(0, asi);
					maskShape.setPoint(1, bsi);
					maskShape.setPoint(2, intersectionInner);
					maskShape.setFillColor(sf::Color::Black);
					antumbraTempTexture.draw(maskShape);
				}
				else
				{
					sf::ConvexShape maskShape;
					maskShape.setPointCount(4);
					maskShape.setPoint(0, asi);
					maskShape.setPoint(1, bsi);
					maskShape.setPoint(2, bsi + priv::vectorNormalize(bdi) * shadowExtension);
					maskShape.setPoint(3, asi + priv::vectorNormalize(adi) * shadowExtension);
					maskShape.setFillColor(sf::Color::Black);
					antumbraTempTexture.draw(maskShape);
				}

				unmaskWithPenumbras(antumbraTempTexture, sf::BlendAdd, unshadowShader, penumbras, shadowExtension);

				antumbraTempTexture.display();

				lightTempTexture.setView(lightTempTexture.getDefaultView());
				lightTempTexture.draw(sf::Sprite(antumbraTempTexture.getTexture()), sf::BlendMultiply);
				lightTempTexture.setView(view);
			}
			else
			{
				sf::ConvexShape maskShape;
				maskShape.setPointCount(4);
				maskShape.setPoint(0, as);
				maskShape.setPoint(1, bs);
				maskShape.setPoint(2, bs + priv::vectorNormalize(bd) * shadowExtension);
				maskShape.setPoint(3, as + priv::vectorNormalize(ad) * shadowExtension);
				maskShape.setFillColor(sf::Color::Black);
				lightTempTexture.draw(maskShape);

				unmaskWithPenumbras(lightTempTexture, sf::BlendMultiply, unshadowShader, penumbras, shadowExtension);
			}
		}
    }

    for (unsigned i = 0; i < shapesCount; i++) 
	{
        LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);

        if (pLightShape->renderLightOver()) 
		{
            pLightShape->setColor(sf::Color::White);
            lightTempTexture.draw(*pLightShape, &lightOverShapeShader);
        }
        else 
		{
            pLightShape->setColor(sf::Color::Black);
            lightTempTexture.draw(*pLightShape);
        }
    }

    lightTempTexture.display();
}

void LightPointEmission::setLocalCastCenter(sf::Vector2f const & localCenter)
{
	mLocalCastCenter = localCenter;
}

sf::Vector2f LightPointEmission::getLocalCastCenter() const
{
	return mLocalCastCenter;
}

void LightPointEmission::setSourceRadius(float radius)
{
	mSourceRadius = radius;
}

float LightPointEmission::getSourceRadius() const
{
	return mSourceRadius;
}

void LightPointEmission::setShadowOverExtendMultiplier(float multiplier)
{
	mShadowOverExtendMultiplier = multiplier;
}

float LightPointEmission::getShadowOverExtendMultiplier() const
{
	return mShadowOverExtendMultiplier;
}

sf::FloatRect LightPointEmission::getAABB() const
{
	return mSprite.getGlobalBounds();
}

sf::Vector2f LightPointEmission::getCastCenter() const
{
	sf::Transform t = mSprite.getTransform();
	t.translate(mSprite.getOrigin());
	return t.transformPoint(mLocalCastCenter);
}

void LightPointEmission::getPenumbrasPoint(std::vector<priv::Penumbra>& penumbras, std::vector<int>& innerBoundaryIndices, std::vector<sf::Vector2f>& innerBoundaryVectors, std::vector<int>& outerBoundaryIndices, std::vector<sf::Vector2f>& outerBoundaryVectors, const LightShape& shape)
{
	sf::Vector2f sourceCenter = getCastCenter();

	const int numPoints = shape.getPointCount();

	std::vector<bool> bothEdgesBoundaryWindings;
	bothEdgesBoundaryWindings.reserve(2);

	std::vector<bool> oneEdgeBoundaryWindings;
	oneEdgeBoundaryWindings.reserve(2);

	// Calculate front and back facing sides
	std::vector<bool> facingFrontBothEdges;
	facingFrontBothEdges.reserve(numPoints);

	std::vector<bool> facingFrontOneEdge;
	facingFrontOneEdge.reserve(numPoints);

	for (int i = 0; i < numPoints; i++) 
	{
		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));
		sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint((i < numPoints - 1) ? i + 1 : 0));

		sf::Vector2f firstEdgeRay;
		sf::Vector2f secondEdgeRay;
		sf::Vector2f firstNextEdgeRay;
		sf::Vector2f secondNextEdgeRay;

		{
			sf::Vector2f sourceToPoint = point - sourceCenter;
			sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
			firstEdgeRay = point - (sourceCenter - perpendicularOffset);
			secondEdgeRay = point - (sourceCenter + perpendicularOffset);
		}
		{
			sf::Vector2f sourceToPoint = nextPoint - sourceCenter;
			sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
			firstNextEdgeRay = nextPoint - (sourceCenter - perpendicularOffset);
			secondNextEdgeRay = nextPoint - (sourceCenter + perpendicularOffset);
		}

		sf::Vector2f pointToNextPoint = nextPoint - point;
		sf::Vector2f normal = priv::vectorNormalize(sf::Vector2f(-pointToNextPoint.y, pointToNextPoint.x));

		// Front facing, mark it
		facingFrontBothEdges.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f && priv::vectorDot(secondEdgeRay, normal) > 0.0f) || (priv::vectorDot(firstNextEdgeRay, normal) > 0.0f && priv::vectorDot(secondNextEdgeRay, normal) > 0.0f));
		facingFrontOneEdge.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f || priv::vectorDot(secondEdgeRay, normal) > 0.0f) || priv::vectorDot(firstNextEdgeRay, normal) > 0.0f || priv::vectorDot(secondNextEdgeRay, normal) > 0.0f);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontBothEdges[i] != facingFrontBothEdges[i - 1])
		{
			innerBoundaryIndices.push_back(i);
			bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[i]);
		}
	}

	// Check looping indices separately
	if (facingFrontBothEdges[0] != facingFrontBothEdges[numPoints - 1]) 
	{
		innerBoundaryIndices.push_back(0);
		bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[0]);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontOneEdge[i] != facingFrontOneEdge[i - 1]) 
		{
			outerBoundaryIndices.push_back(i);
			oneEdgeBoundaryWindings.push_back(facingFrontOneEdge[i]);
		}
	}

	// Check looping indices separately
	if (facingFrontOneEdge[0] != facingFrontOneEdge[numPoints - 1]) 
	{
		outerBoundaryIndices.push_back(0);
		oneEdgeBoundaryWindings.push_back(facingFrontOneEdge[0]);
	}

	// Compute outer boundary vectors
	for (unsigned bi = 0; bi < outerBoundaryIndices.size(); bi++) 
	{
		int penumbraIndex = outerBoundaryIndices[bi];
		bool winding = oneEdgeBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f sourceToPoint = point - sourceCenter;
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (sourceCenter + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (sourceCenter - perpendicularOffset);

		// Add boundary vector
		outerBoundaryVectors.push_back(winding ? firstEdgeRay : secondEdgeRay);
	}

	for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++) 
	{
		int penumbraIndex = innerBoundaryIndices[bi];
		bool winding = bothEdgesBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f sourceToPoint = point - sourceCenter;
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (sourceCenter + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (sourceCenter - perpendicularOffset);

		// Add boundary vector
		innerBoundaryVectors.push_back(winding ? secondEdgeRay : firstEdgeRay);
		sf::Vector2f outerBoundaryVector = winding ? firstEdgeRay : secondEdgeRay;

		if (innerBoundaryIndices.size() == 1)
		{
			innerBoundaryVectors.push_back(outerBoundaryVector);
		}

		// Add penumbras
		bool hasPrevPenumbra = false;

		sf::Vector2f prevPenumbraLightEdgeVector;

		float prevBrightness = 1.0f;

		while (penumbraIndex != -1) 
		{
			int nextPointIndex = (penumbraIndex < numPoints - 1) ? penumbraIndex + 1 : 0;
			sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint(nextPointIndex));
			sf::Vector2f pointToNextPoint = nextPoint - point;

			int prevPointIndex = (penumbraIndex > 0) ? penumbraIndex - 1 : numPoints - 1;
			sf::Vector2f prevPoint = shape.getTransform().transformPoint(shape.getPoint(prevPointIndex));
			sf::Vector2f pointToPrevPoint = prevPoint - point;

			priv::Penumbra penumbra;
			penumbra._source = point;

			if (!winding) 
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToNextPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle) 
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToNextPoint;
					penumbraIndex = nextPointIndex;

					if (hasPrevPenumbra) 
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					sourceToPoint = point - sourceCenter;
					perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
					firstEdgeRay = point - (sourceCenter + perpendicularOffset);
					secondEdgeRay = point - (sourceCenter - perpendicularOffset);
					outerBoundaryVector = secondEdgeRay;

					if (!outerBoundaryVectors.empty()) 
					{
						outerBoundaryVectors[0] = penumbra._darkEdge;
						outerBoundaryIndices[0] = penumbraIndex;
					}
				}
				else 
				{
					penumbra._darkBrightness = 0.0f;

					if (hasPrevPenumbra) 
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = false;

					if (!outerBoundaryVectors.empty()) 
					{
						outerBoundaryVectors[0] = penumbra._darkEdge;
						outerBoundaryIndices[0] = penumbraIndex;
					}

					penumbraIndex = -1;
				}
			}
			else // Winding = true
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToPrevPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle) 
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToPrevPoint;
					penumbraIndex = prevPointIndex;

					if (hasPrevPenumbra) 
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					sourceToPoint = point - sourceCenter;
					perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
					firstEdgeRay = point - (sourceCenter + perpendicularOffset);
					secondEdgeRay = point - (sourceCenter - perpendicularOffset);
					outerBoundaryVector = firstEdgeRay;

					if (!outerBoundaryVectors.empty()) 
					{
						outerBoundaryVectors[1] = penumbra._darkEdge;
						outerBoundaryIndices[1] = penumbraIndex;
					}
				}
				else 
				{
					penumbra._darkBrightness = 0.0f;

					if (hasPrevPenumbra) 
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = false;

					if (!outerBoundaryVectors.empty()) 
					{
						outerBoundaryVectors[1] = penumbra._darkEdge;
						outerBoundaryIndices[1] = penumbraIndex;
					}

					penumbraIndex = -1;
				}
			}

			penumbras.push_back(penumbra);
		}
	}
}

void LightPointEmission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

} // namespace ltbl