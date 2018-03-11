#include <math/primitives.hpp>

#include <cmath>

namespace math {

Line::Line(const Vector<double>& normal, double value)
    : normal(normal)
    , value(value)
{ }

Line Line::alongDirection(
    const Vector<double>& position, const Vector<double>& direction)
{
    auto normal = direction.rotateCcw90().normalized();
    auto value = dot(position, normal);
    return {normal, value};
}

Line Line::alongInterval(const Interval& interval)
{
    return Line::alongDirection(interval.start, interval.end - interval.start);
}

Line Line::betweenPoints(
    const Vector<double>& firstPoint, const Vector<double>& secondPoint)
{
    return Line::alongDirection(firstPoint, secondPoint - firstPoint);
}

Line Line::move(double offset) const
{
    return {normal, value + offset};
}

Circle::Circle(const Vector<double>& center, double radius)
    : center(center)
    , radius(radius)
{ }

Vector<double> operator-(const Line& line, Vector<double> point)
{
    return -(point - line);
}

Vector<double> operator-(Vector<double> point, const Line& line)
{
    auto offset = dot(point, line.normal) - line.value;
    return line.normal * offset;
}

double distance(
    const Vector<double>& firstPoint, const Vector<double>& secondPoint)
{
    return (secondPoint - firstPoint).length();
}

double distance(const Line& line, const Vector<double>& point)
{
    return std::abs(dot(line.normal, point) - line.value);
}

double distance(const Vector<double>& point, const Line& line)
{
    return distance(line, point);
}

double distance(const Circle& firstCircle, const Circle& secondCircle)
{
    return distance(firstCircle.center, secondCircle.center) -
        firstCircle.radius - secondCircle.radius;
}

double distance(const Circle& circle, const Line& line)
{
    return distance(circle.center, line) - circle.radius;
}

double distance(const Line& line, const Circle& circle)
{
    return distance(circle, line);
}

std::optional<Vector<double>> lineCross(const Line& left, const Line& right)
{
    auto delta = cross(left.normal, right.normal);
    if (delta == 0) {
        return {};
    }

    auto c1 = cross<double>(
        {left.value, left.normal.y}, {right.value, right.normal.y});
    auto c2 = cross<double>(
        {left.normal.x, left.value}, {right.normal.x, right.value});

    return {{c1 / delta, c2 / delta}};
}

} // namespace math
