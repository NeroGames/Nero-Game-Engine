#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <list>
#include <unordered_set>
#include <vector>

#include <SFML/Graphics.hpp>

namespace ltbl
{

namespace priv
{

const float _pi = 3.14159265f;
const float _radToDeg = 180.0f / _pi;
const float _degToRad = _pi / 180.0f;

inline sf::Vector2f rectCenter(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f);
}

inline bool rectContains(const sf::FloatRect& rect, const sf::FloatRect& other)
{
	if (other.left < rect.left)
		return false;
	if (other.top < rect.top)
		return false;
	if (other.left + other.width > rect.left + rect.width)
		return false;
	if (other.top + other.height > rect.top + rect.height)
		return false;
	return true;
}

inline sf::Vector2f rectHalfDims(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.width * 0.5f, rect.height * 0.5f);
}

inline sf::Vector2f rectDims(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.width, rect.height);
}

inline sf::Vector2f rectLowerBound(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left, rect.top);
}

inline sf::Vector2f rectUpperBound(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
}

inline sf::FloatRect rectFromBounds(const sf::Vector2f& lowerBound, const sf::Vector2f& upperBound)
{
	return sf::FloatRect(lowerBound.x, lowerBound.y, upperBound.x - lowerBound.x, upperBound.y - lowerBound.y);
}

inline sf::FloatRect rectRecenter(const sf::FloatRect& rect, const sf::Vector2f& center)
{
	sf::Vector2f dims = rectDims(rect);
	return sf::FloatRect(center - dims * 0.5f, dims);
}

inline float vectorMagnitude(const sf::Vector2f& vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline float vectorMagnitudeSquared(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

inline sf::Vector2f vectorNormalize(const sf::Vector2f& vector)
{
	float magnitude = vectorMagnitude(vector);
	if (magnitude == 0.0f)
		return sf::Vector2f(1.0f, 0.0f);
	float distInv = 1.0f / magnitude;
	return sf::Vector2f(vector.x * distInv, vector.y * distInv);
}

inline float vectorDot(const sf::Vector2f& left, const sf::Vector2f& right)
{
	return left.x * right.x + left.y * right.y;
}

inline float vectorProject(const sf::Vector2f& left, const sf::Vector2f& right)
{
	assert(vectorMagnitudeSquared(right) != 0.0f);
	return vectorDot(left, right) / vectorMagnitudeSquared(right);
}

inline bool shapeIntersection(const sf::ConvexShape& left, const sf::ConvexShape& right)
{
	std::vector<sf::Vector2f> transformedLeft(left.getPointCount());
	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		transformedLeft[i] = left.getTransform().transformPoint(left.getPoint(i));
	}

	std::vector<sf::Vector2f> transformedRight(right.getPointCount());
	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		transformedRight[i] = right.getTransform().transformPoint(right.getPoint(i));
	}

	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		sf::Vector2f point = transformedLeft[i];
		sf::Vector2f nextPoint = (i == left.getPointCount() - 1u) ? transformedLeft[0] : transformedLeft[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedRight[0].x * edgePerpendicular.x + transformedRight[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < right.getPointCount(); j++)
		{
			float proj = (transformedRight[j].x * edgePerpendicular.x + transformedRight[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}

	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		sf::Vector2f point = transformedRight[i];
		sf::Vector2f nextPoint = (i == right.getPointCount() - 1u) ? transformedRight[0] : transformedRight[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedLeft[0].x * edgePerpendicular.x + transformedLeft[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < left.getPointCount(); j++)
		{
			float proj = (transformedLeft[j].x * edgePerpendicular.x + transformedLeft[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}

	return true;
}

inline sf::ConvexShape shapeFromRect(const sf::FloatRect& rect)
{
	sf::ConvexShape shape(4);
	sf::Vector2f halfDims = sf::Vector2f(rect.width * 0.5f, rect.height * 0.5f);
	shape.setPoint(0, sf::Vector2f(-halfDims.x, -halfDims.y));
	shape.setPoint(1, sf::Vector2f(halfDims.x, -halfDims.y));
	shape.setPoint(2, sf::Vector2f(halfDims.x, halfDims.y));
	shape.setPoint(3, sf::Vector2f(-halfDims.x, halfDims.y));
	shape.setPosition(sf::Vector2f(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f));
	return shape;
}

inline bool rayIntersect(const sf::Vector2f& as, const sf::Vector2f& ad, const sf::Vector2f& bs, const sf::Vector2f& bd, sf::Vector2f& intersection)
{
	float dx = bs.x - as.x;
	float dy = bs.y - as.y;
	float det = bd.x * ad.y - bd.y * ad.x;
	if (det == 0.0f)
		return false;
	float u = (dy * bd.x - dx * bd.y) / det;
	if (u < 0.0f)
		return false;
	float v = (dy * ad.x - dx * ad.y) / det;
	if (v < 0.0f)
		return false;
	intersection = as + ad * u;
	return true;
}

//////////////////////////////////////////////////////////////////////////
/// \brief An occupant of a quadtree
//////////////////////////////////////////////////////////////////////////
class QuadtreeOccupant
{
	public:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		//////////////////////////////////////////////////////////////////////////
		QuadtreeOccupant()
			: mAwake(true)
			, mAABBChanged(false)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Set the occupant awake for the quadtree
		/// \param awake True to set awake, false otherwise
		//////////////////////////////////////////////////////////////////////////
		void setAwake(bool awake)
		{
			mAwake = awake;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Is the occupant awake for the quadtree ?
		/// \return True if the occupant is awake, false otherwise
		//////////////////////////////////////////////////////////////////////////
		bool isAwake() const
		{
			return mAwake;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Toggle the awake statut of the occupant
		//////////////////////////////////////////////////////////////////////////
		void toggleAwake()
		{
			mAwake = !mAwake;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the AABB box of the occupant
		/// \return The AABB box
		//////////////////////////////////////////////////////////////////////////
		virtual sf::FloatRect getAABB() const = 0;

		//////////////////////////////////////////////////////////////////////////
		/// \brief Notify the quadtree that the AABB box changed
		//////////////////////////////////////////////////////////////////////////
		void quadtreeAABBChanged()
		{
			mAABBChanged = true;
		}

	private:
		bool mAwake; ///< Is the occupant awake ? (ie queryable / updatable by the quadtree)
		bool mAABBChanged; ///< Do the AABB box changed ?

	private:
		friend class Quadtree;
};

//////////////////////////////////////////////////////////////////////////
/// \brief A quadtree node
/// The only node you have to create is the root node
/// The difference with other nodes is that the root has no parent
/// To build the root node only the 3 first parameters are required (and the second and third are optionnal)
//////////////////////////////////////////////////////////////////////////
class Quadtree : sf::NonCopyable, public sf::Drawable
{
	public:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Constructor
		/// \param region The region of the quadtree
		/// \param maxOccupants The number of occupants per region, having more occupants will make the quadtree split
		/// \param maxLevels The number of depth level of the quadtree, if this level is reached, the quadtree will never split again
		/// \param parent The parent of this quadtree
		/// \param level The level of this quadtree
		/// \param type The type of this quadtree, used to render the quadtree
		//////////////////////////////////////////////////////////////////////////
		Quadtree(const sf::FloatRect& region, unsigned int maxOccupants = 5, unsigned int maxLevels = 5, Quadtree* parent = nullptr, unsigned int level = 0, unsigned int type = 0)
			: mRegion(region)
			, mMaxOccupants(maxOccupants)
			, mMaxLevels(maxLevels)
			, mParent(parent)
			, mLevel(level)
			, mType(type)
			, mOccupants()
			, mOutsideOccupants()
			, mChildren()
		{
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create the quadtree (or recreate)
		/// \param region The region of the quadtree
		/// \param maxOccupants The number of occupants per region, having more occupants will make the quadtree split
		/// \param maxLevels The number of depth level of the quadtree, if this level is reached, the quadtree will never split again
		/// \param parent The parent of this quadtree
		/// \param level The level of this quadtree
		/// \param type The type of this quadtree, used to render the quadtree
		//////////////////////////////////////////////////////////////////////////
		void create(const sf::FloatRect& region, unsigned int maxOccupants = 5, unsigned int maxLevels = 5, Quadtree* parent = nullptr, unsigned int level = 0, unsigned int type = 0)
		{
			clear();

			mRegion = region;
			mMaxOccupants = maxOccupants;
			mMaxLevels = maxLevels;
			mParent = parent;
			mLevel = level;
			mType = type;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Add an occupant
		/// \param oc The occupant
		//////////////////////////////////////////////////////////////////////////
		void addOccupant(QuadtreeOccupant* oc)
		{
			if (oc != nullptr)
			{
				if (mRegion.intersects(oc->getAABB()))
				{
					if (hasChildren())
					{
						bool handled = false;
						for (std::size_t i = 0; i < mChildren.size(); i++)
						{
							if (!handled && mChildren[i]->mRegion.intersects(oc->getAABB()))
							{
								mChildren[i]->addOccupant(oc);
								handled = true;
							}
						}
						if (handled)
						{
							return;
						}
					}
					else
					{
						mOccupants.insert(oc);

						if (mOccupants.size() >= mMaxOccupants && mLevel < mMaxLevels)
						{
							split();
						}

						return;
					}
				}

				if (mParent != nullptr)
				{
					mParent->addOccupant(oc);
				}
				else
				{
					mOutsideOccupants.insert(oc);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Remove an occupant
		/// \param oc The occupant to remove
		/// \return True if it has been removed, false otherwise
		//////////////////////////////////////////////////////////////////////////
		bool removeOccupant(QuadtreeOccupant* oc)
		{
			if (hasChildren())
			{
				bool removed = false;
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (mChildren[i]->mRegion.intersects(oc->getAABB()))
					{
						if (mChildren[i]->removeOccupant(oc))
						{
							removed = true;
						}
					}
				}
				if (removed)
				{
					if (getNumOccupantsBelow() < mMaxOccupants)
					{
						unsplit();
					}
					return true;
				}
			}
			else
			{
				auto itr = mOccupants.find(oc);
				if (itr != mOccupants.end())
				{
					mOccupants.erase(itr);
					return true;
				}
			}

			if (mOutsideOccupants.size() > 0)
			{
				auto itr = mOutsideOccupants.find(oc);
				if (itr != mOutsideOccupants.end())
				{
					mOutsideOccupants.erase(itr);
					return true;
				}
			}

			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Update the quadtree (occupants which have moved)
		//////////////////////////////////////////////////////////////////////////
		bool update()
		{
			bool moved = false;
			if (hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (mChildren[i]->update())
					{
						moved = true;
					}
				}

				if (mOutsideOccupants.size() > 0)
				{
					for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); )
					{
						bool erase = false;
						if ((*itr) != nullptr)
						{
							if ((*itr)->mAABBChanged && (*itr)->isAwake())
							{
								(*itr)->mAABBChanged = false;
								if (mRegion.intersects((*itr)->getAABB()))
								{
									addOccupant(*itr);
									erase = true;
								}
							}
						}
						else
						{
							erase = true;
						}
						if (erase)
						{
							itr = mOutsideOccupants.erase(itr);
						}
						else
						{
							++itr;
						}
					}
				}

				if (moved)
				{
					if (getNumOccupantsBelow() < mMaxOccupants)
					{
						unsplit();
					}
				}
			}
			else
			{
				for (auto itr = mOccupants.begin(); itr != mOccupants.end(); )
				{
					bool erase = false;
					if ((*itr) != nullptr)
					{
						if ((*itr)->mAABBChanged && (*itr)->isAwake())
						{
							(*itr)->mAABBChanged = false;
							if (!mRegion.intersects((*itr)->getAABB()))
							{
								if (mParent != nullptr)
								{
									mParent->addOccupant(*itr);
								}
								else
								{
									mOutsideOccupants.insert(*itr);
								}
								erase = true;
							}
						}
					}
					else
					{
						erase = true;
					}
					if (erase)
					{
						itr = mOccupants.erase(itr);
						moved = true;
					}
					else
					{
						++itr;
					}
				}
			}
			return moved;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Clear the quadtree
		//////////////////////////////////////////////////////////////////////////
		void clear()
		{
			if (hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					mChildren[i]->clear();
					delete mChildren[i];
					mChildren[i] = nullptr;
				}
				mChildren.clear();
			}

			mOccupants.clear();
			mOutsideOccupants.clear();

			mParent = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Query occupants from an area
		/// \param area The query area
		/// \param occupants The returned occupants
		//////////////////////////////////////////////////////////////////////////
		void query(const sf::FloatRect& area, std::vector<QuadtreeOccupant*>& occupants)
		{
			for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
			{
				if ((*itr) != nullptr && (*itr)->isAwake() && area.intersects((*itr)->getAABB()))
				{
					occupants.push_back(*itr);
				}
			}

			std::list<Quadtree*> open;
			open.push_back(this);
			while (!open.empty())
			{
				Quadtree* current = open.back();
				open.pop_back();
				if (area.intersects(current->mRegion))
				{
					if (current->hasChildren())
					{
						for (std::size_t i = 0; i < mChildren.size(); i++)
						{
							if (current->mChildren[i]->getNumOccupantsBelow() > 0)
							{
								open.push_back(current->mChildren[i]);
							}
						}
					}
					else
					{
						for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
						{
							if ((*itr) != nullptr && (*itr)->isAwake() && area.intersects((*itr)->getAABB()))
							{
								occupants.push_back(*itr);
							}
						}
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Query occupants from a point
		/// \param point The query point
		/// \param occupants The returned occupants
		//////////////////////////////////////////////////////////////////////////
		void query(const sf::Vector2f& point, std::vector<QuadtreeOccupant*>& occupants)
		{
			for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
			{
				if ((*itr) != nullptr && (*itr)->isAwake() && (*itr)->getAABB().contains(point))
				{
					occupants.push_back(*itr);
				}
			}

			std::list<Quadtree*> open;
			open.push_back(this);
			while (!open.empty())
			{
				Quadtree* current = open.back();
				open.pop_back();
				if (current->mRegion.contains(point))
				{
					if (current->hasChildren())
					{
						for (std::size_t i = 0; i < mChildren.size(); i++)
						{
							if (current->mChildren[i]->getNumOccupantsBelow() > 0)
							{
								open.push_back(current->mChildren[i]);
							}
						}
					}
					else
					{
						for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
						{
							if ((*itr) != nullptr && (*itr)->isAwake() && (*itr)->getAABB().contains(point))
							{
								occupants.push_back(*itr);
							}
						}
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Query occupants from a shape
		/// \param shape The query shape
		/// \param occupants The returned occupants
		//////////////////////////////////////////////////////////////////////////
		void query(const sf::ConvexShape& shape, std::vector<QuadtreeOccupant*>& occupants)
		{
			for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
			{
				if ((*itr) != nullptr && (*itr)->isAwake() && shapeIntersection(shape, shapeFromRect((*itr)->getAABB())))
				{
					occupants.push_back(*itr);
				}
			}

			std::list<Quadtree*> open;
			open.push_back(this);
			while (!open.empty())
			{
				Quadtree* current = open.back();
				open.pop_back();
				if (shapeIntersection(shape, shapeFromRect(current->mRegion)))
				{
					if (current->hasChildren())
					{
						for (std::size_t i = 0; i < mChildren.size(); i++)
						{
							if (current->mChildren[i]->getNumOccupantsBelow() > 0)
							{
								open.push_back(current->mChildren[i]);
							}
						}
					}
					else
					{
						for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
						{
							if ((*itr) != nullptr && (*itr)->isAwake() && shapeIntersection(shape, shapeFromRect((*itr)->getAABB())))
							{
								occupants.push_back(*itr);
							}
						}
					}
				}
			}
		}

	private:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Split the quadtree
		//////////////////////////////////////////////////////////////////////////
		void split()
		{
			mChildren.clear();

			sf::Vector2f lower = { mRegion.left, mRegion.top };
			sf::Vector2f size = { mRegion.width * 0.5f, mRegion.height * 0.5f };

			for (std::size_t i = 0; i < 4; i++)
			{
				sf::FloatRect rect(lower.x, lower.y, size.x, size.y);
				switch (i)
				{
				case 1: rect.left += size.x; break;
				case 3: rect.left += size.x;
				case 2: rect.top += size.y; break;
				default: break;
				}
				mChildren.push_back(new Quadtree(rect, mMaxOccupants, mMaxLevels, this, mLevel + 1, i + 1));
			}

			for (auto itr = mOccupants.begin(); itr != mOccupants.end(); itr++)
			{
				if ((*itr) != nullptr)
				{
					addOccupant(*itr);
				}
			}

			mOccupants.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Unsplit the quadtree
		//////////////////////////////////////////////////////////////////////////
		void unsplit()
		{
			for (std::size_t i = 0; i < mChildren.size(); i++)
			{
				if (mChildren[i] != nullptr)
				{
					for (auto itr = mChildren[i]->mOccupants.begin(); itr != mChildren[i]->mOccupants.end(); itr++)
					{
						if ((*itr) != nullptr)
						{
							mOccupants.insert(*itr);
						}
					}
					mChildren[i]->clear();

					delete mChildren[i];
					mChildren[i] = nullptr;
				}
			}
			mChildren.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Get the number of occupants below
		/// \return The number of occupants below
		//////////////////////////////////////////////////////////////////////////
		unsigned int getNumOccupantsBelow() const
		{
			if (hasChildren())
			{
				unsigned int sum = 0;
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					sum += mChildren[i]->getNumOccupantsBelow();
				}
				return sum;
			}
			else
			{
				return mOccupants.size();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Do the quadtree has children (ie sub-quadtree) ?
		/// \return True if the quadtree has children, false otherwise
		//////////////////////////////////////////////////////////////////////////
		bool hasChildren() const
		{
			return mChildren.size() > 0;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Draw the quadtree
		/// \param target The render target to draw the quadtree on
		/// \param states The render states to apply to the quadtree on render
		//////////////////////////////////////////////////////////////////////////
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			if (hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					target.draw(*mChildren[i], states);
				}
			}
			else
			{
				sf::Color color;
				switch (mType)
				{
				case 1: color = sf::Color::Red; break;
				case 2: color = sf::Color::Green; break;
				case 3: color = sf::Color::Blue; break;
				case 4: color = sf::Color::Yellow; break;
				default: color = sf::Color::Magenta; break;
				}
				for (auto itr = mOccupants.begin(); itr != mOccupants.end(); itr++)
				{
					if ((*itr) != nullptr)
					{
						sf::FloatRect box = (*itr)->getAABB();
						sf::RectangleShape oc({ box.width, box.height });
						oc.setPosition({ box.left, box.top });
						oc.setFillColor(color);
						target.draw(oc, states);
					}
				}

				sf::RectangleShape shape({ mRegion.width, mRegion.height });
				shape.setPosition({ mRegion.left, mRegion.top });
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(sf::Color::Black);
				shape.setOutlineThickness(1.f);
				target.draw(shape, states);
			}

			for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
			{
				if ((*itr) != nullptr)
				{
					sf::FloatRect box = (*itr)->getAABB();
					sf::RectangleShape oc({ box.width, box.height });
					oc.setPosition({ box.left, box.top });
					oc.setFillColor(sf::Color::Cyan);
					target.draw(oc, states);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Create a convex shape from a rectangle
		/// \param rect The rectangle defining the shape
		/// \return The created shape
		//////////////////////////////////////////////////////////////////////////
		static sf::ConvexShape shapeFromRect(const sf::FloatRect& rect)
		{
			sf::ConvexShape shape(4);
			shape.setPoint(0, { 0.f , 0.f });
			shape.setPoint(1, { rect.width, 0.f });
			shape.setPoint(2, { rect.width, rect.height });
			shape.setPoint(3, { 0.f, rect.height });
			shape.setPosition(rect.left, rect.top);
			return shape;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Do two convex shapes intersect ?
		/// \param left The first shape
		/// \param right The second shape
		/// \return True if they intersect, false otherwise
		//////////////////////////////////////////////////////////////////////////
		static bool shapeIntersection(const sf::ConvexShape& left, const sf::ConvexShape& right)
		{
			std::vector<sf::Vector2f> transformedLeft(left.getPointCount());
			for (unsigned i = 0; i < left.getPointCount(); i++)
			{
				transformedLeft[i] = left.getTransform().transformPoint(left.getPoint(i));
			}

			std::vector<sf::Vector2f> transformedRight(right.getPointCount());
			for (unsigned i = 0; i < right.getPointCount(); i++)
			{
				transformedRight[i] = right.getTransform().transformPoint(right.getPoint(i));
			}

			for (unsigned i = 0; i < left.getPointCount(); i++)
			{
				sf::Vector2f point = transformedLeft[i];
				sf::Vector2f nextPoint = (i == left.getPointCount() - 1u) ? transformedLeft[0] : transformedLeft[i + 1];
				sf::Vector2f edge = nextPoint - point;
				sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
				float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
				float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
				float minRightProj = (transformedRight[0].x * edgePerpendicular.x + transformedRight[0].y * edgePerpendicular.y) / magnitude;

				for (unsigned j = 1; j < right.getPointCount(); j++)
				{
					float proj = (transformedRight[j].x * edgePerpendicular.x + transformedRight[j].y * edgePerpendicular.y) / magnitude;
					minRightProj = std::min(minRightProj, proj);
				}

				if (minRightProj > pointProj)
				{
					return false;
				}
			}

			for (unsigned i = 0; i < right.getPointCount(); i++)
			{
				sf::Vector2f point = transformedRight[i];
				sf::Vector2f nextPoint = (i == right.getPointCount() - 1u) ? transformedRight[0] : transformedRight[i + 1];
				sf::Vector2f edge = nextPoint - point;
				sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
				float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
				float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
				float minRightProj = (transformedLeft[0].x * edgePerpendicular.x + transformedLeft[0].y * edgePerpendicular.y) / magnitude;

				for (unsigned j = 1; j < left.getPointCount(); j++)
				{
					float proj = (transformedLeft[j].x * edgePerpendicular.x + transformedLeft[j].y * edgePerpendicular.y) / magnitude;
					minRightProj = std::min(minRightProj, proj);
				}

				if (minRightProj > pointProj)
				{
					return false;
				}
			}
			return true;
		}

	private:
		sf::FloatRect mRegion; ///< The region of the quadree
		unsigned int mMaxOccupants; ///< The number of max occupants
		unsigned int mMaxLevels; ///< The number of level max
		Quadtree* mParent; ///< The parent of the quadtree
		unsigned int mLevel; ///< The actual level of the quadtree
		unsigned int mType; ///< The type of the quadtree

		std::unordered_set<QuadtreeOccupant*> mOccupants; ///< The occupants of the quadtree
		std::unordered_set<QuadtreeOccupant*> mOutsideOccupants; ///< The occupants outside the region, belonging to the quadtree (used only by the main quadtree)

		std::vector<Quadtree*> mChildren; ///< The children (sub-quadtree)
};

//////////////////////////////////////////////////////////////////////////
/// \brief Data of a penumbra
//////////////////////////////////////////////////////////////////////////
struct Penumbra
{
	sf::Vector2f _source; ///< The source
	sf::Vector2f _lightEdge; ///< The light edge
	sf::Vector2f _darkEdge; ///< The dark edge
	float _lightBrightness; ///< The light brightness
	float _darkBrightness; ///< The dark brightness
	float _distance; ///< The distance
};

//////////////////////////////////////////////////////////////////////////
/// \brief Base class for lights and light shape
//////////////////////////////////////////////////////////////////////////
class BaseLight
{
	public:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Default constructor
		//////////////////////////////////////////////////////////////////////////
		BaseLight()
			: mTurnedOn(true)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Enable or disable the light
		/// \param turnedOn True to enable the light, false to disable it
		//////////////////////////////////////////////////////////////////////////
		void setTurnedOn(bool turnedOn)
		{
			mTurnedOn = turnedOn;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Tell whether or not the light is enabled
		/// \return True if the light is enabled, false otherwise
		//////////////////////////////////////////////////////////////////////////
		bool isTurnedOn() const
		{
			return mTurnedOn;
		}

		//////////////////////////////////////////////////////////////////////////
		/// \brief Toggle the light on/off
		//////////////////////////////////////////////////////////////////////////
		void toggleTurnedOn()
		{
			mTurnedOn = !mTurnedOn;
		}

	protected:
		//////////////////////////////////////////////////////////////////////////
		/// \brief Unmask with penumbras
		/// \param renderTexture The render texture to apply penumbras on
		/// \param blendMode The blend mode
		/// \param unshadowShader The unshadow shader
		/// \param penumbras The penumbras
		/// \param shadowExtension The shadow extension
		//////////////////////////////////////////////////////////////////////////
		void unmaskWithPenumbras(sf::RenderTexture& renderTexture, sf::BlendMode blendMode, sf::Shader& unshadowShader, const std::vector<Penumbra>& penumbras, float shadowExtension)
		{
			sf::VertexArray vertexArray;
			vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
			vertexArray.resize(3);

			sf::RenderStates states;
			states.blendMode = blendMode;
			states.shader = &unshadowShader;

			unsigned int penumbrasCount = penumbras.size();
			for (unsigned int i = 0; i < penumbrasCount; i++)
			{
				unshadowShader.setUniform("lightBrightness", penumbras[i]._lightBrightness);
				unshadowShader.setUniform("darkBrightness", penumbras[i]._darkBrightness);
				vertexArray[0].position = penumbras[i]._source;
				vertexArray[1].position = penumbras[i]._source + priv::vectorNormalize(penumbras[i]._lightEdge) * shadowExtension;
				vertexArray[2].position = penumbras[i]._source + priv::vectorNormalize(penumbras[i]._darkEdge) * shadowExtension;
				vertexArray[0].texCoords = sf::Vector2f(0.0f, 1.0f);
				vertexArray[1].texCoords = sf::Vector2f(1.0f, 0.0f);
				vertexArray[2].texCoords = sf::Vector2f(0.0f, 0.0f);
				renderTexture.draw(vertexArray, states);
			}
		}

	private:
		bool mTurnedOn; ///< Is the light turned on ?
};

} // namespace priv

} // namespace ltbl