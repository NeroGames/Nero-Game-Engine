////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/CircleMesh.h>
////////////////////////////////////////////////////////////
namespace nero
{
    CircleMesh::CircleMesh()
        : PointMesh(PointMesh::Shape::Circle)
    {
        m_CircleShape.setOutlineThickness(1.f);
    }

    void CircleMesh::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        PointMesh::draw(target, states);

        states.transform = sf::Transform::Identity;
        states.transform.translate(m_CircleShape.getPosition());
        target.draw(m_CircleShape);
    }

    void CircleMesh::updateShape()
    {
        PointMesh::updateShape();

        const auto pointTable = getPointTable();
        const auto radius     = math::distance(pointTable.front(), pointTable.back());
        m_CircleShape.setRadius(radius);
        m_CircleShape.setOrigin(sf::Vector2f(radius, radius));
        m_CircleShape.setPosition(pointTable.front());
    }

    void CircleMesh::updateColor()
    {
        PointMesh::updateColor();
        m_CircleShape.setFillColor(graphics::getTransparentColor(getColor(), m_ColorAlpha));
        m_CircleShape.setOutlineColor(getColor());
    }

    sf::CircleShape& CircleMesh::getCircleShape()
    {
        return m_CircleShape;
    }

    CircleMesh::Ptr CircleMesh::Cast(PointMesh::Ptr pointMesh)
    {
        if(pointMesh->getMeshShape() != PointMesh::Circle)
            return nullptr;

        return std::static_pointer_cast<CircleMesh>(pointMesh);
    }

    sf::FloatRect CircleMesh::getRealGlobalBounds() const
    {
        sf::FloatRect globalBound;

        sf::Vector2f  pointOne = m_VertexTable.front().getPosition();
        sf::Vector2f  pointTwo = m_VertexTable.back().getPosition();
        float         radius   = math::distance(pointOne, pointTwo);

        globalBound.left       = pointOne.x - radius;
        globalBound.top        = pointOne.y - radius;
        globalBound.height     = radius * 2.f;
        globalBound.width      = radius * 2.f;

        return globalBound;
    }

    void CircleMesh::generateDefaultShape()
    {
        const auto massCenter = sf::Vector2f(0.f, 0.f);
        const auto radius     = 50.f;
        addVertex(massCenter);
        addVertex(sf::Vector2f(massCenter.x + radius, massCenter.y));

        m_MeshType = PointMesh::Dynamic;
        updateShape();
        updateColor();
    }

    PointMesh::Ptr CircleMesh::clone() const
    {
        CircleMesh::Ptr circleMesh = std::make_shared<CircleMesh>();

        for(const auto& point : getPointTable())
            circleMesh->addVertex(point);

        circleMesh->setMeshType(m_MeshType);
        circleMesh->m_Scale    = m_Scale;
        circleMesh->m_Rotation = m_Rotation;
        circleMesh->m_Position = m_Position;
        circleMesh->updateShape();
        circleMesh->updateColor();

        return circleMesh;
    }

    void CircleMesh::scaleMesh(const sf::Vector2f& scaleFactor)
    {
        sf::Vector2f massCenter = m_VertexTable.front().getPosition();
        sf::Vector2f pointTwo   = m_VertexTable.back().getPosition();
        sf::Vector2f newPointTwo =
            sf::Vector2f((pointTwo.x - massCenter.x) * scaleFactor.x + massCenter.x,
                         (pointTwo.y - massCenter.y) * scaleFactor.y + massCenter.y);
        m_VertexTable.back().setPosition(newPointTwo);

        updateShape();
    }

    void CircleMesh::rotateMesh(const float& angle)
    {
        const auto newPointTwo = math::rotateVertex(m_VertexTable.front().getPosition(),
                                                    angle,
                                                    m_VertexTable.back().getPosition());
        m_VertexTable.back().setPosition(newPointTwo);

        updateShape();
    }

} // namespace nero
