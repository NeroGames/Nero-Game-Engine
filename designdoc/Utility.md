# Clang Format

find . -name "*.h" -path "*include/*" | xargs clang-format -i -style=file
find . -name "*.cpp" -path "*src/*" | xargs clang-format -i -style=file