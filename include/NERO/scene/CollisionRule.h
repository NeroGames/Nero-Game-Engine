#ifndef COLLISIONRULE_H
#define COLLISIONRULE_H

#include <map>
#include <unordered_set>
#include <vector>
#include <NERO/object/Object.h>

namespace nero
{
    class CollisionRule
    {
        public:
            CollisionRule();
            virtual ~CollisionRule();

            void canCollide(std::string categoryA, std::string categoryB);
            void print();
            void printBit();
            void printCategoryTab();
            void apply(Object::Ptr root);

            void fullCollision(Object::Ptr root);
            void noCollision(Object::Ptr root);

        private:
            void updateCategoryBit();
            void updateCategoryMaskBit();

            //suppress all collision : set all mask bit to zero (0)
        private:
            std::map<std::string, std::unordered_set<std::string>> m_CollisionMap;
            std::map<std::string, std::pair<int, int>> m_CollisionRuleMap;
            std::vector<int> m_CatecotryBitTab;


    };
}


#endif // COLLISIONRULE_H
