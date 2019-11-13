#include "LightDirectionEmission.hpp"

namespace ltbl
{

LightDirectionEmission::LightDirectionEmission()
	: BaseLight()
	, mShape()
	, mCastDirection(0.f, 1.f)
	, mCastAngle(90.f)
	, mSourceRadius(5.0f)
	, mSourceDistance(100.0f)
{
}

void LightDirectionEmission::setColor(const sf::Color& color)
{
	mShape.setFillColor(color);
}

const sf::Color& LightDirectionEmission::getColor() const
{
	return mShape.getFillColor();
}

void LightDirectionEmission::render(const sf::View& view, sf::RenderTexture& lightTempTexture, sf::RenderTexture& antumbraTempTexture, sf::Shader& unshadowShader, const std::vector<priv::QuadtreeOccupant*>& shapes, float shadowExtension)
{
    lightTempTexture.setView(view);
    lightTempTexture.clear(sf::Color::White);

    // Mask off light shape (over-masking - mask too much, reveal penumbra/antumbra afterwards)
	unsigned int shapesCount = shapes.size();
	for (unsigned int i = 0; i < shapesCount; ++i)
	{
        LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
		if (pLightShape != nullptr && pLightShape->isTurnedOn())
		{
			// Get boundaries
			std::vector<priv::Penumbra> penumbras;
			std::vector<int> innerBoundaryIndices;
			std::vector<int> outerBoundaryIndices;
			std::vector<sf::Vector2f> innerBoundaryVectors;
			std::vector<sf::Vector2f> outerBoundaryVectors;

			getPenumbrasDirection(penumbras, innerBoundaryIndices, innerBoundaryVectors, outerBoundaryIndices, outerBoundaryVectors, *pLightShape);

			if (innerBoundaryIndices.size() != 2 || outerBoundaryIndices.size() != 2)
			{
				continue;
			}

			antumbraTempTexture.clear(sf::Color::White);
			antumbraTempTexture.setView(view);

			float maxDist = 0.0f;
			for (unsigned j = 0; j < pLightShape->getPointCount(); j++)
			{
				maxDist = std::max(maxDist, priv::vectorMagnitude(view.getCenter() - pLightShape->getTransform().transformPoint(pLightShape->getPoint(j))));
			}
			float totalShadowExtension = shadowExtension + maxDist;

			sf::ConvexShape maskShape;
			maskShape.setPointCount(4);
			maskShape.setPoint(0, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0])));
			maskShape.setPoint(1, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1])));
			maskShape.setPoint(2, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1])) + priv::vectorNormalize(innerBoundaryVectors[1]) * totalShadowExtension);
			maskShape.setPoint(3, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0])) + priv::vectorNormalize(innerBoundaryVectors[0]) * totalShadowExtension);
			maskShape.setFillColor(sf::Color::Black);
			antumbraTempTexture.draw(maskShape);

			unmaskWithPenumbras(antumbraTempTexture, sf::BlendAdd, unshadowShader, penumbras, totalShadowExtension);

			antumbraTempTexture.display();

			lightTempTexture.setView(lightTempTexture.getDefaultView());
			lightTempTexture.draw(sf::Sprite(antumbraTempTexture.getTexture()), sf::BlendMultiply);
			lightTempTexture.setView(view);
		}
    }

    for (unsigned int i = 0; i < shapesCount; i++) 
	{
        LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
        if (pLightShape->renderLightOver() && pLightShape->isTurnedOn()) 
		{
            pLightShape->setColor(sf::Color::White);

            lightTempTexture.draw(*pLightShape);
        }
    }

    lightTempTexture.setView(lightTempTexture.getDefaultView());
	mShape.setSize(lightTempTexture.getView().getSize());
    lightTempTexture.draw(mShape, sf::BlendMultiply);

    lightTempTexture.display();
}

void LightDirectionEmission::setCastDirection(const sf::Vector2f& castDirection)
{
	mCastDirection = priv::vectorNormalize(castDirection);
	mCastAngle = priv::_radToDeg * std::atan2(mCastDirection.y, mCastDirection.x);
}

const sf::Vector2f& LightDirectionEmission::getCastDirection() const
{
	return mCastDirection;
}

void LightDirectionEmission::setCastAngle(float angle)
{
	mCastAngle = angle;
	float radAngle = angle * priv::_degToRad;
	mCastDirection.x = std::cos(radAngle);
	mCastDirection.y = std::sin(radAngle);
}

float LightDirectionEmission::getCastAngle() const
{
	return mCastAngle;
}

void LightDirectionEmission::setSourceRadius(float radius)
{
	mSourceRadius = radius;
}

float LightDirectionEmission::getSourceRadius() const
{
	return mSourceRadius;
}

void LightDirectionEmission::setSourceDistance(float distance)
{
	mSourceDistance = distance;
}

float LightDirectionEmission::getSourceDistance() const
{
	return mSourceDistance;
}

void LightDirectionEmission::getPenumbrasDirection(std::vector<priv::Penumbra>& penumbras, std::vector<int>& innerBoundaryIndices, std::vector<sf::Vector2f>& innerBoundaryVectors, std::vector<int>& outerBoundaryIndices, std::vector<sf::Vector2f>& outerBoundaryVectors, const LightShape& shape)
{
	const int numPoints = shape.getPointCount();

	innerBoundaryIndices.reserve(2);
	innerBoundaryVectors.reserve(2);
	penumbras.reserve(2);

	std::vector<bool> bothEdgesBoundaryWindings;
	bothEdgesBoundaryWindings.reserve(2);

	// Calculate front and back facing sides
	std::vector<bool> facingFrontBothEdges;
	facingFrontBothEdges.reserve(numPoints);

	std::vector<bool> facingFrontOneEdge;
	facingFrontOneEdge.reserve(numPoints);

	for (int i = 0; i < numPoints; i++) 
	{
		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));
		sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint((i < numPoints - 1) ? i + 1 : 0));
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
		sf::Vector2f firstNextEdgeRay = nextPoint - (point - mCastDirection * mSourceDistance - perpendicularOffset);
		sf::Vector2f secondNextEdgeRay = nextPoint - (point - mCastDirection * mSourceDistance + perpendicularOffset);
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
		}
	}

	// Check looping indices separately
	if (facingFrontOneEdge[0] != facingFrontOneEdge[numPoints - 1])
	{
		outerBoundaryIndices.push_back(0);
	}

	for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++) 
	{
		int penumbraIndex = innerBoundaryIndices[bi];
		bool winding = bothEdgesBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);

		// Add boundary vector
		innerBoundaryVectors.push_back(winding ? secondEdgeRay : firstEdgeRay);
		sf::Vector2f outerBoundaryVector = winding ? firstEdgeRay : secondEdgeRay;
		outerBoundaryVectors.push_back(outerBoundaryVector);

		// Add penumbras
		bool hasPrevPenumbra = false;

		sf::Vector2f prevPenumbraLightEdgeVector;

		float prevBrightness = 1.0f;

		while (penumbraIndex != -1) 
		{
			sf::Vector2f nextPoint;
			int nextPointIndex;

			if (penumbraIndex < numPoints - 1) 
			{
				nextPointIndex = penumbraIndex + 1;
				nextPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex + 1));
			}
			else 
			{
				nextPointIndex = 0;
				nextPoint = shape.getTransform().transformPoint(shape.getPoint(0));
			}

			sf::Vector2f pointToNextPoint = nextPoint - point;

			sf::Vector2f prevPoint;
			int prevPointIndex;

			if (penumbraIndex > 0) 
			{
				prevPointIndex = penumbraIndex - 1;
				prevPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex - 1));
			}
			else 
			{
				prevPointIndex = numPoints - 1;
				prevPoint = shape.getTransform().transformPoint(shape.getPoint(numPoints - 1));
			}

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
					perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
					firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
					secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
					outerBoundaryVector = secondEdgeRay;
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
					perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
					firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
					secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
					outerBoundaryVector = firstEdgeRay;
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
					penumbraIndex = -1;
				}
			}

			penumbras.push_back(penumbra);
		}
	}
}

} // namespace ltbl