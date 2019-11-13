#pragma once

#include "LightDirectionEmission.hpp"
#include "LightPointEmission.hpp"
#include "LightResources.hpp"
#include "Sprite.hpp"

namespace ltbl
{

//////////////////////////////////////////////////////////////////////////
/// \brief System which handle lights
//////////////////////////////////////////////////////////////////////////
class LightSystem : sf::NonCopyable
{
    public:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Constructor
		/// \param useNormals Will the light system use normals ? Default to false
		//////////////////////////////////////////////////////////////////////////
		LightSystem(bool useNormals = false);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create quadtrees, resources and render textures
		/// \param rootRegion The root region for quadtrees
		/// \param imageSize The size of the image, used to create render texture
		//////////////////////////////////////////////////////////////////////////
        void create(const sf::FloatRect& rootRegion, const sf::Vector2u& imageSize);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Render the lights
		/// \param target The render target to render the lights on
		//////////////////////////////////////////////////////////////////////////
        void render(sf::RenderTarget& target);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \param shape The light shape will copy this shape
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape(const sf::RectangleShape& shape);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \param shape The light shape will copy this shape
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape(const sf::ConvexShape& shape);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \param shape The light shape will copy this shape
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape(const sf::CircleShape& shape);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \param rect The light shape will copy this rectangle
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape(const sf::FloatRect& rect);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light shape
		/// \param sprite The light shape will copy this sprite
		/// \return The new light shape
		//////////////////////////////////////////////////////////////////////////
		LightShape* createLightShape(const sf::Sprite& sprite);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Remove a light shape
		/// \param shape The light shape to remove
		//////////////////////////////////////////////////////////////////////////
		void removeShape(LightShape* shape);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light point emission
		/// \return The new light point emission
		//////////////////////////////////////////////////////////////////////////
		LightPointEmission* createLightPointEmission();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Remove a light point emission
		/// \param light The light point emission to remove
		//////////////////////////////////////////////////////////////////////////
		void removeLight(LightPointEmission* light);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a light direction emission
		/// \return The new light direction emission
		//////////////////////////////////////////////////////////////////////////
		LightDirectionEmission* createLightDirectionEmission();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Remove a light direction emssion
		/// \param light The light direction emission to remove
		//////////////////////////////////////////////////////////////////////////
		void removeLight(LightDirectionEmission* light);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Add a sprite
		/// \param sprite The new sprite
		//////////////////////////////////////////////////////////////////////////
		void addSprite(Sprite& sprite);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Remove a sprite
		/// \param sprite The sprite to remove
		//////////////////////////////////////////////////////////////////////////
		void removeSprite(Sprite& sprite);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Set the direction emission range
		/// \param range The new range
		//////////////////////////////////////////////////////////////////////////
		void setDirectionEmissionRange(float range);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the direction emission range
		/// \return The current range
		//////////////////////////////////////////////////////////////////////////
		float getDirectionEmissionRange() const;

		//////////////////////////////////////////////////////////////////////////
		/// \brief Set the direction emission radius multiplier
		/// \param multiplier The new multiplier
		//////////////////////////////////////////////////////////////////////////
		void setDirectionEmissionRadiusMultiplier(float multiplier);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the direction emission radius multiplier
		/// \return The current multiplier
		//////////////////////////////////////////////////////////////////////////
		float getDirectionEmissionRadiusMultiplier() const;

		//////////////////////////////////////////////////////////////////////////
		/// \brief Set ambient color
		/// \param color The new ambient color
		//////////////////////////////////////////////////////////////////////////
		void setAmbientColor(const sf::Color& color);

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get ambient color
		/// \return The current ambient color
		//////////////////////////////////////////////////////////////////////////
		const sf::Color& getAmbientColor() const;

		//////////////////////////////////////////////////////////////////////////
		/// \brief Tell whether or not the light system use normals
		/// \return True if the system uses it, false otherwise
		//////////////////////////////////////////////////////////////////////////
		bool useNormals() const;

		//////////////////////////////////////////////////////////////////////////
		/// \brief Update shader texture and the size of render texture
		/// Call it only if you change the penumbra texture or shaders, render texture size are automatically updated
		/// \param size The new size for render texture, don't call it with this parameter yourself !
		//////////////////////////////////////////////////////////////////////////
		void update(const sf::Vector2u& size = sf::Vector2u());

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the penumbra texture
		/// As penumbra texture if automatically loaded, use it only to change the texture
		/// You have to call update(), right after you changed it
		/// Don't forget to smooth the texture after changing it
		/// \return The penumbra texture
		//////////////////////////////////////////////////////////////////////////
		sf::Texture& getPenumbraTexture();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the unshadow shader
		/// As unshadow shader if automatically loaded, use it only to change the shader
		/// You have to call update(), right after you changed it
		/// \return The unshadow shader
		//////////////////////////////////////////////////////////////////////////
		sf::Shader& getUnshadowShader();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the light over shape shader
		/// As light over shape shader if automatically loaded, use it only to change the shader
		/// You have to call update(), right after you changed it
		/// \return The light over shape shader
		//////////////////////////////////////////////////////////////////////////
		sf::Shader& getLightOverShapeShader();

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the normal shader
		/// As normal shader if automatically loaded, use it only to change the shader
		/// You have to call update(), right after you changed it
		/// \return The normal shader
		//////////////////////////////////////////////////////////////////////////
		sf::Shader& getNormalsShader();

	private:
		sf::Texture mPenumbraTexture; ///< The penumbra texture, loaded from memory when the system is created
		sf::Shader mUnshadowShader; ///< The unshadow shader, loaded from memory when the system is created
		sf::Shader mLightOverShapeShader; ///< The light over shape shader, loaded from memory when the system is created
		sf::Shader mNormalsShader; ///< The normal shader

		priv::Quadtree mLightShapeQuadtree; ///< The quadtree which handles LightShape
		priv::Quadtree mLightPointEmissionQuadtree; ///< The quadtree which handles LightPointEmission

		std::unordered_set<LightPointEmission*> mPointEmissionLights; ///< The LightPointEmissions of the system
		std::unordered_set<LightDirectionEmission*> mDirectionEmissionLights; ///< The LightDirectionEmissions of the system
		std::unordered_set<LightShape*> mLightShapes; ///< The LightShapes of the system
		std::unordered_set<Sprite*> mNormalSprites; ///< The NormalSprites of the system

		sf::RenderTexture mLightTempTexture; ///< The light render texture
		sf::RenderTexture mEmissionTempTexture; ///< The emission render texture
		sf::RenderTexture mAntumbraTempTexture; ///< The antumbra render texture
		sf::RenderTexture mCompositionTexture; ///< The composition render texture
		sf::RenderTexture mNormalsTexture; ///< The normal render texture

		float mDirectionEmissionRange; ///< The direction emission range
		float mDirectionEmissionRadiusMultiplier; ///< The dreiction emission radius multiplier
		sf::Color mAmbientColor; ///< The ambient color

		const bool mUseNormals; ///< Do the system use normals ?
};

} // namespace ltbl
