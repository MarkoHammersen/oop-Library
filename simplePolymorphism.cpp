#include <iostream>

class Shape {
  public:
    virtual ~Shape() {};
    virtual void draw() const = 0;
};

class Circle: public Shape{
  public:
    ~Circle(){
      std::cout << "Circle destroyed" << std::endl;
    }

    void draw() const override {
        std::cout << "Drawing a Circle" << std::endl;
    }
};

class Rectangle: public Shape{
  public:
    ~Rectangle(){
      std::cout << "Rectangle destroyed" << std::endl;
    }

    void draw() const override {
        std::cout << "Drawing a Rectangle" << std::endl;
    }
};

int main() {

  std::unique_ptr<Shape> shapes[2] = {
      std::make_unique<Circle>(),
      std::make_unique<Rectangle>()
    };

    for(const auto& shape: shapes) {
        shape->draw();
    }

    Circle *ptr = dynamic_cast<Circle*>(shapes[0].get());
    if(ptr != nullptr) {
      ptr->draw();
    } else {
        std::cout << "First shape is NOT a Circle" << std::endl;
    }

    return 0;
}