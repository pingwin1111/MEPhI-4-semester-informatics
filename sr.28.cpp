#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

using namespace std;

class GraphicElement {
protected:
    bool onScene;
    string name;

public:
    GraphicElement(const string& n) : name(n), onScene(false) {}
    virtual ~GraphicElement() {}


    virtual void placeOnScene() {
        onScene = true;
        cout << name << " помещен на сцену\n";
    }

    virtual void removeFromScene() {
        onScene = false;
        cout << name << " убран со сцены\n";
    }

    virtual void move(double dx, double dy) = 0;
    virtual double getLength() const { return 0.0; }
    virtual double getArea() const { return 0.0; }
    virtual bool containsPoint(double x, double y) const { return false; }

    bool isOnScene() const { return onScene; }
    string getName() const { return name; }

    virtual void displayInfo() const {
        cout << name << " (" << (onScene ? "на сцене" : "не на сцене") << ")";
    }
};


class Point : public GraphicElement {
private:
    double x, y;

public:
    Point(double x, double y, const string& name = "Точка")
        : GraphicElement(name), x(x), y(y) {}

    void move(double dx, double dy) override {
        x += dx;
        y += dy;
        cout << name << " перемещена в (" << x << ", " << y << ")\n";
    }

    double getX() const { return x; }
    double getY() const { return y; }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", координаты: (" << x << ", " << y << ")\n";
    }
};


class Circle : public GraphicElement {
private:
    double centerX, centerY;
    double radius;

public:
    Circle(double x, double y, double r, const string& name = "Окружность")
        : GraphicElement(name), centerX(x), centerY(y), radius(r) {}

    void move(double dx, double dy) override {
        centerX += dx;
        centerY += dy;
        cout << name << " перемещена в (" << centerX << ", " << centerY << ")\n";
    }

    double getLength() const override {
        return 2 * M_PI * radius;
    }

    double getArea() const override {
        return M_PI * radius * radius;
    }

    bool containsPoint(double x, double y) const override {
        double dx = x - centerX;
        double dy = y - centerY;
        return dx*dx + dy*dy <= radius*radius;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", центр: (" << centerX << ", " << centerY << "), радиус: " << radius
             << ", длина: " << getLength() << ", площадь: " << getArea() << "\n";
    }
};


class Ellipse : public GraphicElement {
private:
    double centerX, centerY;
    double a, b; // полуоси

public:
    Ellipse(double x, double y, double a, double b, const string& name = "Эллипс")
        : GraphicElement(name), centerX(x), centerY(y), a(a), b(b) {}

    void move(double dx, double dy) override {
        centerX += dx;
        centerY += dy;
        cout << name << " перемещен в (" << centerX << ", " << centerY << ")\n";
    }

    double getLength() const override {

        return M_PI * (3*(a + b) - sqrt((3*a + b) * (a + 3*b)));
    }

    double getArea() const override {
        return M_PI * a * b;
    }

    bool containsPoint(double x, double y) const override {
        double dx = (x - centerX) / a;
        double dy = (y - centerY) / b;
        return dx*dx + dy*dy <= 1.0;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", центр: (" << centerX << ", " << centerY << "), полуоси: " << a << ", " << b
             << ", длина: " << getLength() << ", площадь: " << getArea() << "\n";
    }
};


class LineSegment : public GraphicElement {
private:
    double x1, y1, x2, y2;

public:
    LineSegment(double x1, double y1, double x2, double y2, const string& name = "Отрезок")
        : GraphicElement(name), x1(x1), y1(y1), x2(x2), y2(y2) {}

    void move(double dx, double dy) override {
        x1 += dx; y1 += dy;
        x2 += dx; y2 += dy;
        cout << name << " перемещен в (" << x1 << ", " << y1 << ")-(" << x2 << ", " << y2 << ")\n";
    }

    double getLength() const override {
        double dx = x2 - x1;
        double dy = y2 - y1;
        return sqrt(dx*dx + dy*dy);
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", точки: (" << x1 << ", " << y1 << ")-(" << x2 << ", " << y2
             << "), длина: " << getLength() << "\n";
    }
};


class Polyline : public GraphicElement {
private:
    vector<pair<double, double>> points;

public:
    Polyline(const vector<pair<double, double>>& pts, const string& name = "Ломаная")
        : GraphicElement(name), points(pts) {}

    void move(double dx, double dy) override {
        for (auto& p : points) {
            p.first += dx;
            p.second += dy;
        }
        cout << name << " перемещена\n";
    }

    double getLength() const override {
        if (points.size() < 2) return 0.0;

        double length = 0.0;
        for (size_t i = 1; i < points.size(); ++i) {
            double dx = points[i].first - points[i-1].first;
            double dy = points[i].second - points[i-1].second;
            length += sqrt(dx*dx + dy*dy);
        }
        return length;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", количество точек: " << points.size()
             << ", длина: " << getLength() << "\n";
    }
};


class Triangle : public GraphicElement {
private:
    double x1, y1, x2, y2, x3, y3;

public:
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3,
             const string& name = "Треугольник")
        : GraphicElement(name), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}

    void move(double dx, double dy) override {
        x1 += dx; y1 += dy;
        x2 += dx; y2 += dy;
        x3 += dx; y3 += dy;
        cout << name << " перемещен\n";
    }

    double getLength() const override {
        double a = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        double b = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
        double c = sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));
        return a + b + c;
    }

    double getArea() const override {
        return abs((x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)) / 2.0;
    }

    bool containsPoint(double x, double y) const override {

        double d = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);
        double a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / d;
        double b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / d;
        double c = 1 - a - b;

        return a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", вершины: (" << x1 << "," << y1 << "), (" << x2 << "," << y2
             << "), (" << x3 << "," << y3 << "), периметр: " << getLength()
             << ", площадь: " << getArea() << "\n";
    }
};


class Square : public GraphicElement {
private:
    double x, y;
    double side;

public:
    Square(double x, double y, double s, const string& name = "Квадрат")
        : GraphicElement(name), x(x), y(y), side(s) {}

    void move(double dx, double dy) override {
        x += dx;
        y += dy;
        cout << name << " перемещен в (" << x << ", " << y << ")\n";
    }

    double getLength() const override {
        return 4 * side;
    }

    double getArea() const override {
        return side * side;
    }

    bool containsPoint(double px, double py) const override {
        return px >= x && px <= x + side && py >= y && py <= y + side;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", левый нижний угол: (" << x << ", " << y << "), сторона: " << side
             << ", периметр: " << getLength() << ", площадь: " << getArea() << "\n";
    }
};

class Rectangle : public GraphicElement {
private:
    double x, y;
    double width, height;

public:
    Rectangle(double x, double y, double w, double h, const string& name = "Прямоугольник")
        : GraphicElement(name), x(x), y(y), width(w), height(h) {}

    void move(double dx, double dy) override {
        x += dx;
        y += dy;
        cout << name << " перемещен в (" << x << ", " << y << ")\n";
    }

    double getLength() const override {
        return 2 * (width + height);
    }

    double getArea() const override {
        return width * height;
    }

    bool containsPoint(double px, double py) const override {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", левый нижний угол: (" << x << ", " << y << "), размеры: " << width << "x" << height
             << ", периметр: " << getLength() << ", площадь: " << getArea() << "\n";
    }
};


class Rhombus : public GraphicElement {
private:
    double centerX, centerY;
    double diag1, diag2;

public:
    Rhombus(double x, double y, double d1, double d2, const string& name = "Ромб")
        : GraphicElement(name), centerX(x), centerY(y), diag1(d1), diag2(d2) {}

    void move(double dx, double dy) override {
        centerX += dx;
        centerY += dy;
        cout << name << " перемещен в (" << centerX << ", " << centerY << ")\n";
    }

    double getLength() const override {

        double side = sqrt((diag1*diag1 + diag2*diag2) / 2.0;
        return 4 * side;
    }

    double getArea() const override {
        return diag1 * diag2 / 2.0;
    }

    bool containsPoint(double px, double py) const override {
        double dx = abs(px - centerX) * 2 / diag1;
        double dy = abs(py - centerY) * 2 / diag2;
        return dx + dy <= 1.0;
    }

    void displayInfo() const override {
        GraphicElement::displayInfo();
        cout << ", центр: (" << centerX << ", " << centerY << "), диагонали: " << diag1 << ", " << diag2
             << ", периметр: " << getLength() << ", площадь: " << getArea() << "\n";
    }
};


class Scene {
private:
    vector<unique_ptr<GraphicElement>> elements;

public:
    void addElement(unique_ptr<GraphicElement> elem) {
        elements.push_back(move(elem));
    }

    void displayAll() const {
        cout << "=== Элементы на сцене ===\n";
        for (const auto& elem : elements) {
            if (elem->isOnScene()) {
                elem->displayInfo();
            }
        }
    }

    vector<string> findElementsContainingPoint(double x, double y) const {
        vector<string> result;
        for (const auto& elem : elements) {
            if (elem->isOnScene() && elem->containsPoint(x, y)) {
                result.push_back(elem->getName());
            }
        }
        return result;
    }
};

int main() {
    Scene scene;


    auto point = make_unique<Point>(1, 1, "Точка A");
    auto circle = make_unique<Circle>(3, 3, 2, "Окружность C");
    auto ellipse = make_unique<Ellipse>(7, 5, 3, 1.5, "Эллипс E");
    auto line = make_unique<LineSegment>(0, 0, 4, 4, "Отрезок L");

    vector<pair<double, double>> polylinePoints = {{1,1}, {2,3}, {4,4}, {5,2}};
    auto polyline = make_unique<Polyline>(polylinePoints, "Ломаная P");

    auto triangle = make_unique<Triangle>(2,2, 5,2, 3.5,4, "Треугольник T");
    auto square = make_unique<Square>(6, 1, 2, "Квадрат S");
    auto rectangle = make_unique<Rectangle>(1, 5, 3, 2, "Прямоугольник R");
    auto rhombus = make_unique<Rhombus>(5, 5, 4, 2, "Ромб Rh");


    scene.addElement(move(point));
    scene.addElement(move(circle));
    scene.addElement(move(ellipse));
    scene.addElement(move(line));
    scene.addElement(move(polyline));
    scene.addElement(move(triangle));
    scene.addElement(move(square));
    scene.addElement(move(rectangle));
    scene.addElement(move(rhombus));

    auto& lastElement = scene.getElements().back();
    lastElement->placeOnScene();
    lastElement->move(1, -1);

    scene.getElements()[1]->placeOnScene();
    scene.getElements()[3]->placeOnScene();
    scene.getElements()[5]->placeOnScene();
    scene.getElements()[7]->placeOnScene();


    scene.displayAll();

    double testX = 3.0, testY = 3.0;
    auto containingElements = scene.findElementsContainingPoint(testX, testY);

    cout << "\nТочка (" << testX << ", " << testY << ") содержится в:\n";
    for (const auto& name : containingElements) {
        cout << "- " << name << "\n";
    }

    cout << "\nДлины и площади:\n";
    for (const auto& elem : scene.getElements()) {
        if (elem->isOnScene()) {
            cout << elem->getName() << ": длина = " << elem->getLength();
            if (elem->getArea() > 0) {
                cout << ", площадь = " << elem->getArea();
            }
            cout << "\n";
        }
    }

    return 0;
}
