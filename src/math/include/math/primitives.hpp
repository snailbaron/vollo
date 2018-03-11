#pragma once

#include <math/vector.hpp>

#include <optional>

namespace math {

struct Interval {
    Interval(const Vector<double>& start, const Vector<double>& end)
        : start(start), end(end)
    { }

    Vector<double> start;
    Vector<double> end;
};

struct Line {
    Line() {}
    Line(const Vector<double>& normal, double value);

    static Line alongDirection(
        const Vector<double>& position, const Vector<double>& direction);
    static Line alongInterval(const Interval& interval);
    static Line betweenPoints(
        const Vector<double>& firstPoint, const Vector<double>& secondPoint);

    Line move(double offset) const;

    Vector<double> normal;
    double value;
};

struct Circle {
    Circle() {}
    Circle(const Vector<double>& center, double radius);

    Vector<double> center;
    double radius;
};

struct AxisBox {
    Vector<double> start;
    Vector<double> end;
};

Vector<double> operator-(const Line& line, Vector<double> point);
Vector<double> operator-(Vector<double> point, const Line& line);

double distance(
    const Vector<double>& firstPoint, const Vector<double>& secondPoint);
double distance(const Line& line, const Vector<double>& point);
double distance(const Vector<double>& point, const Line& line);
double distance(const Circle& firstCircle, const Circle& secondCircle);
double distance(const Circle& circle, const Line& line);
double distance(const Line& line, const Circle& circle);

std::optional<Vector<double>> lineCross(const Line& left, const Line& right);

} // namespace math
