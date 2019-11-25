#include <iostream>
#include <variant>
#include <vector>

struct Coord {
    double x;
    double y;

    Coord &operator+=(const Coord &other) {
      x += other.x;
      y += other.y;
      return *this;
    }

};

std::ostream &operator<<(std::ostream &out, const Coord &coord) {
  out << "(" << coord.x << "," << coord.y << ")";
  return out;
}

class Line {
private:
    Coord _from;
    Coord _end;

public:
    Line(Coord from, Coord end) : _from{from}, _end{end} {}

    void draw() const {
      std::cout << "from at" << _from << " to " << _end << '\n';
    }
};

class Circle {
private:
    Coord center;
    int rad;

public:
    Circle(Coord c, int r) : center{c}, rad{r} {}

    void move(const Coord &coord) {
      center += coord;
    }

    void draw() const {
      std::cout << "circle at" << center << " with radius " << rad << '\n';
    }
};

using GeoObj = std::variant<Line, Circle>;

std::vector<GeoObj> createFigure() {
  std::vector<GeoObj> f;
  f.push_back(Line{Coord{1, 2}, Coord{3, 4}});
  f.push_back(Circle{Coord{5, 5}, 2});
  return f;
}

int main() {
  std::vector<GeoObj> figure = createFigure();
  for (const GeoObj &geoObj: figure) {
    std::visit([](const auto &obj) { obj.draw(); }, geoObj);
  }
}