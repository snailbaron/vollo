#pragma once

#include <math/vector.hpp>
#include <math/primitives.hpp>

#include <optional>

namespace math {

std::optional<Vector<double>> rayTraceCircle(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Circle& circle);
std::optional<Vector<double>> rayTraceLine(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Line& line);
std::optional<Vector<double>> rayTraceInterval(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Interval& interval);

std::optional<Vector<double>> circleTraceCircle(
    const Circle& tracer,
    const Vector<double>& direction,
    const Circle& target);
std::optional<Vector<double>> circleTracePoint(
    const Circle& tracer,
    const Vector<double>& direction,
    const Vector<double>& target);
std::optional<Vector<double>> circleTraceLine(
    const Circle& circle, const Vector<double>& direction, const Line& line);
std::optional<Vector<double>> circleTraceInterval(
    const Circle& circle,
    const Vector<double>& direction,
    const Interval& interval);
std::optional<Vector<double>> circleTraceAxisBox(
    const Circle& circle,
    const Vector<double>& direction,
    const AxisBox& box);

} // namespace math
