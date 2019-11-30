#include <Nero/core/cpp/utility/Utility.h>

#include <SFML/Window/Keyboard.hpp>

namespace nero
{
    void centerOrigin(sf::Sprite& sprite)
    {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

    void centerOrigin(sf::Text& text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

	bool CTRL()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_ALT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool SHIFT_ALT()
		{
			return  !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		bool CTRL_SHIFT_ALT()
		{
			return  sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);
		}

		sf::Color b2_to_sf(const b2Color& color, int transparency)
			{
				return sf::Color(color.r * 255, color.g * 255, color.b * 255, transparency);
			}

			sf::Vector2f b2_to_sf(b2Vec2 vect, float scale)
			{
				return sf::Vector2f(vect.x * scale, vect.y * scale);
			}

			sf::Vector2f b2_to_sf(float x, float y, float scale)
			{
				return sf::Vector2f(x * scale, y * scale);
			}

			b2Color sf_to_b2(const sf::Color& color)
			{
				return b2Color(color.r / 255, color.g / 255, color.b / 255);
			}

			b2Vec2 sf_to_b2(const sf::Vector2f& vect, float scale)
			{
				return b2Vec2(vect.x / scale, vect.y / scale);
			}

			b2Vec2 sf_to_b2(float x, float y, float scale)
			{
				return b2Vec2(x / scale, y / scale);
			}
			sf::Color transparent_color(sf::Color color, int transparency)
				{
					return sf::Color(color.r, color.g, color.b, transparency);
				}

				bool isConvex(std::vector<sf::RectangleShape> polygon)
				{
					std::vector<Point> points;
					for(auto p : polygon)
					{
						Point point;
						point.x = p.getPosition().x;
						point.y = p.getPosition().y;

						points.push_back(point);
					}

					return isConvexPolygon(points);
				}

				bool isConvexPolygon(std::vector<Point> polygon)
				{
					//Thanks to http://www.sunshine2k.de/coding/java/Polygon/Convex/polygon.htm
					if (polygon.size() < 3) return false;

					Point p;
					Point v;
					Point u;

					int res = 0;
					for (int i = 0; i < polygon.size(); i++)
					{
						p = polygon[i];
						Point tmp = polygon[(i+1) % polygon.size()];
						Point v;
						v.x = tmp.x - p.x;
						v.y = tmp.y - p.y;
						u = polygon[(i+2) % polygon.size()];

						if (i == 0) // in first loop direction is unknown, so save it in res
							res = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
						else
						{
							int newres = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;

							if ( (newres > 0 && res < 0) || (newres < 0 && res > 0) )
								return false;
						}
					}
					return true;
				}


				std::vector<PointTab> concave_to_convex(PointTab pVerticesVec)
					{
						PointTab vec;
						int i, n, j,minLen;
						float d, t, dx, dy;
						int i1, i2, i3;
						sf::Vector2f p1, p2, p3;
						int j1, j2;
						sf::Vector2f v1, v2;
						int k=0, h=0;
						PointTab *vec1, *vec2;
						sf::Vector2f *pV, hitV(0,0);
						bool isConvex;
						std::vector<PointTab> figsVec;
						std::queue<PointTab> queue_;

						queue_.push(pVerticesVec);

						while (!queue_.empty()) {
							vec = queue_.front();
							n = vec.size();
							isConvex=true;

							for (i=0; i<n; i++) {
								i1=i;
								i2=(i<n-1)?i+1:i+1-n;
								i3=(i<n-2)?i+2:i+2-n;

								p1 = vec[i1];
								p2 = vec[i2];
								p3 = vec[i3];

								d = det(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
								if ((d<0)) {
									isConvex=false;
									minLen = MAX_VALUE;

									for (j=0; j<n; j++) {
										if ((j!=i1)&&(j!=i2)) {
											j1=j;
											j2=(j<n-1)?j+1:0;

											v1=vec[j1];
											v2=vec[j2];

											pV = hitRay(p1.x,p1.y,p2.x,p2.y,v1.x,v1.y,v2.x,v2.y);

											if (pV) {
												sf::Vector2f v = *pV;
												dx=p2.x-v.x;
												dy=p2.y-v.y;
												t=dx*dx+dy*dy;

												if ((t<minLen)) {
													h=j1;
													k=j2;
													hitV=v;
													minLen=t;
												}
											}
										}
									}

									if (minLen==MAX_VALUE) {
										//TODO: Throw Error !!!
									}

									vec1 = new std::vector<sf::Vector2f>();
									vec2 = new std::vector<sf::Vector2f>();

									j1=h;
									j2=k;
									v1=vec[j1];
									v2=vec[j2];

									if (! pointsMatch(hitV.x,hitV.y,v2.x,v2.y)) {
										vec1->push_back(hitV);
									}
									if (! pointsMatch(hitV.x,hitV.y,v1.x,v1.y)) {
										vec2->push_back(hitV);
									}

									h=-1;
									k=i1;
									while (true) {
										if ((k!=j2)) {
											vec1->push_back(vec[k]);
										}
										else {
											if (((h<0)||h>=n)) {
												//TODO: Throw Error !!!
											}
											if (! isOnSegment(v2.x,v2.y,vec[h].x,vec[h].y,p1.x,p1.y)) {
												vec1->push_back(vec[k]);
											}
											break;
										}

										h=k;
										if (((k-1)<0)) {
											k=n-1;
										}
										else {
											k--;
										}
									}

									reverse(vec1->begin(), vec1->end());

									h=-1;
									k=i2;
									while (true) {
										if ((k!=j1)) {
											vec2->push_back(vec[k]);
										}
										else {
											if (((h<0)||h>=n)) {
												//TODO: Throw Error !!!
											}
											if (((k==j1)&&! isOnSegment(v1.x,v1.y,vec[h].x,vec[h].y,p2.x,p2.y))) {
												vec2->push_back(vec[k]);
											}
											break;
										}

										h=k;
										if (((k+1)>n-1)) {
											k=0;
										}
										else {
											k++;
										}
									}

									queue_.push(*vec1);
									queue_.push(*vec2);
									queue_.pop();

									break;
								}
							}

							if (isConvex) {
								figsVec.push_back(queue_.front());
								queue_.pop();
							}
						}

						return figsVec;
					}

					float det(float x1, float y1, float x2, float y2, float x3, float y3)
					{
						return x1*y2+x2*y3+x3*y1-y1*x2-y2*x3-y3*x1;
					}

					sf::Vector2f* hitRay(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
					{
						float t1 = x3-x1;
						float t2 = y3-y1;
						float t3 = x2-x1;
						float t4 = y2-y1;
						float t5 = x4-x3;
						float t6 = y4-y3;
						float t7 = t4*t5-t3*t6;

						//DBZ Error. Undefined hit segment.
						if(t7 == 0) return nullptr;

						float  a = (((t5*t2) - t6*t1) / t7);
						float px = x1+a*t3;
						float py = y1+a*t4;
						bool b1 = isOnSegment(x2,y2,x1,y1,px,py);
						bool b2 = isOnSegment(px,py,x3,y3,x4,y4);

						if(b1 && b2) {
							return new sf::Vector2f(px,py);
						}
						return nullptr;
					}

					bool isOnSegment(float px, float py, float x1, float y1, float x2, float y2)
					{
						bool b1 =((((x1+0.1)>=px)&&px>=x2-0.1)||(((x1-0.1)<=px)&&px<=x2+0.1));
						bool b2 =((((y1+0.1)>=py)&&py>=y2-0.1)||(((y1-0.1)<=py)&&py<=y2+0.1));
						return ((b1&&b2)&&isOnLine(px,py,x1,y1,x2,y2));
					}

					bool isOnLine(float px, float py, float x1, float y1, float x2, float y2)
					{
						if ((((x2-x1)>0.1)||x1-x2>0.1))
						{
							float a=(y2-y1)/(x2-x1), possibleY=a*(px-x1)+y1, diff=(possibleY>py)?possibleY-py:py-possibleY;
							return (diff<0.1);
						}

						return (((px-x1)<0.1)||x1-px<0.1);
					}

					void err()
					{
						throw std::runtime_error("A problem has occurred. Use the Validate() method to see where the problem is.");
					}

					bool pointsMatch(float x1, float y1, float x2, float y2)
					{
						float dx=(x2>=x1)?x2-x1:x1-x2, dy=(y2>=y1)?y2-y1:y1-y2;
						return ((dx<0.1)&&dy<0.1);
					}

					int validate_polygon(const PointTab &verticesVec)
					{
						int i, n=verticesVec.size(), ret=0;
						float j, j2, i2, i3, d;
						bool fl, fl2=false;

						for (i=0; i<n; i++)
						{
							i2=(i<n-1)?i+1:0;
							i3=(i>0)?i-1:n-1;

							fl=false;
							for (j=0; j<n; j++)
							{
								if ((j!=i)&&(j!=i2))
								{
									if (! fl)
									{
										d = det(verticesVec[i].x,verticesVec[i].y,verticesVec[i2].x,verticesVec[i2].y,verticesVec[j].x,verticesVec[j].y);
										if ((d>0))
										{
											fl=true;
										}
									}

									if ((j!=i3))
									{
										j2=(j<n-1)?j+1:0;
										if (hitSegment(verticesVec[i].x,verticesVec[i].y,verticesVec[i2].x,verticesVec[i2].y,verticesVec[j].x,verticesVec[j].y,verticesVec[j2].x,verticesVec[j2].y))
										{
											ret=1; // TODO: This may be wrong!!!
										}
									}
								 }
							 }

							if (! fl)
							{
								 fl2=true;
							}
						}

						if (fl2)
						{
							 if (ret==1)
							 {
								 ret=3;
							 }
							 else {
								 ret=2;
							 }

						 }

						 return ret;
					 }

					sf::Vector2f* hitSegment(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
					{
						float t1 = x3-x1;
						float t2 = y3-y1;
						float t3 = x2-x1;
						float t4 = y2-y1;
						float t5 = x4-x3;
						float t6 = y4-y3;
						float t7 = t4*t5 - t3*t6;

						//DBZ Error. Undefined hit segment.
						if(t7 == 0) return nullptr;

						float  a = (((t5*t2) - t6*t1) / t7);
						float px = x1+a*t3;
						float py = y1+a*t4;
						bool b1 = isOnSegment(px,py,x1,y1,x2,y2);
						bool b2 = isOnSegment(px,py,x3,y3,x4,y4);

						if(b1 && b2)
						{
							return new sf::Vector2f(px,py);
						}

						return nullptr;
					}

					sf::Vector2f getPolygonCenter(PointTab vertices)
					{
						sf::Vector2f center(0.f, 0.f);
						double signedArea = 0.0;
						double x0 = 0.0; // Current vertex X
						double y0 = 0.0; // Current vertex Y
						double x1 = 0.0; // Next vertex X
						double y1 = 0.0; // Next vertex Y
						double a = 0.0;  // Partial signed area

						// For all vertices except last
						int i=0;
						for (i=0; i<vertices.size()-1; ++i)
						{
							x0 = vertices[i].x;
							y0 = vertices[i].y;
							x1 = vertices[i+1].x;
							y1 = vertices[i+1].y;
							a = x0*y1 - x1*y0;
							signedArea += a;
							center.x += (x0 + x1)*a;
							center.y += (y0 + y1)*a;
						}

						// Do last vertex separately to avoid performing an expensive
						// modulus operation in each iteration.
						x0 = vertices[i].x;
						y0 = vertices[i].y;
						x1 = vertices[0].x;
						y1 = vertices[0].y;
						a = x0*y1 - x1*y0;
						signedArea += a;
						center.x += (x0 + x1)*a;
						center.y += (y0 + y1)*a;

						signedArea *= 0.5;
						center.x /= (6.0*signedArea);
						center.y /= (6.0*signedArea);

						return center;
					}

					sf::Vector2f rotateVertex(sf::Vector2f center, float angle, sf::Vector2f p)
					{
						float cx = center.x;
						float cy = center.y;

						float s = std::sin(toRadian(angle));
						float c = std::cos(toRadian(angle));

						// translate point back to origin:
						p.x -= cx;
						p.y -= cy;

						// rotate point
						float xnew = p.x * c - p.y * s;
						float ynew = p.x * s + p.y * c;

						// translate point back:
						p.x = xnew + cx;
						p.y = ynew + cy;
						return p;
					}

					sf::Vector2f getLineCenter(const sf::Vector2f& a, const sf::Vector2f& b)
					{
						 sf::Vector2f center;
						center.x = (a.x + b.x) / 2;
						center.y = (a.y + b.y) / 2;

						return center;
					}

					nlohmann::json toJson(sf::Color color)
						{
							return  {{"r", color.r}, {"g", color.g}, {"b", color.b}, {"a", color.a}};
						}

						sf::Color colorFromJson(nlohmann::json json)
						{
							return sf::Color(json["r"], json["g"], json["b"], json["a"]);
						}

						float toDegree (float radian)
							{
								return 180.f / 3.141592653589793238462643383f * radian;
							}

							float toRadian(float degree)
							{
								return 3.141592653589793238462643383f / 180.f * degree;
							}
							float vectLength(sf::Vector2f vect)
								{
									return std::sqrt(vect.x * vect.x + vect.y * vect.y);
								}

								float vectLength(b2Vec2 vect)
								{
									return std::sqrt(vect.x * vect.x + vect.y * vect.y);
								}

								float distance(sf::Vector2f vect1, sf::Vector2f vect2)
								{
									return vectLength(vect2 - vect1);
								}

								float distance(b2Vec2 vect1, b2Vec2 vect2)
								{
									return vectLength(vect2 - vect1);
								}


								float dot_product(const sf::Vector2f& vect1, const sf::Vector2f& vect2)
								{
									return vect1.x * vect2.x + vect1.y * vect2.y;
								}


								float distance(sf::Vector2f line_vect1, sf::Vector2f line_vect2, sf::Vector2f vect3)
								{
									float x = vect3.x;
									float y = vect3.y;
									float x1 = line_vect1.x;
									float y1 = line_vect1.y;
									float x2 = line_vect2.x;
									float y2 = line_vect2.y;

									float A = x - x1;
									float B = y - y1;
									float C = x2 - x1;
									float D = y2 - y1;

									float dot = A * C + B * D;
									float len_sq = C * C + D * D;
									float param = dot / len_sq;

									float xx,yy;

									if(param < 0)
									{
										xx = x1;
										yy = y1;
									}
									else if(param > 1)
									{
										xx = x2;
										yy = y2;
									}
									else
									{
										xx = x1 + param * C;
										yy = y1 + param * D;
									}

									return distance(sf::Vector2f(x,y) , sf::Vector2f(xx,yy));
								}

								sf::Vector2f unitVector(sf::Vector2f vect)
								{
									assert(vect != sf::Vector2f(0.f, 0.f));
									return vect / vectLength(vect);
								}
}
