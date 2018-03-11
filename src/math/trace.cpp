#include <math/trace.hpp>

#include <vector>
#include <numeric>

namespace math {

std::optional<Vector<double>> rayTraceCircle(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Circle& circle)
{
    auto rayLine = Line::alongDirection(origin, direction);
    auto centerToLine = rayLine - circle.center;
    auto distanceToLine = centerToLine.length();
    if (distanceToLine > circle.radius) {
        return {};
    }

    auto projection = circle.center + centerToLine;
    auto offsetValue = std::sqrt(
        circle.radius * circle.radius - distanceToLine * distanceToLine);
    auto offsetDirection = (origin - projection).normalized();
    auto result = projection + offsetDirection * offsetValue;
    return result;
}

std::optional<Vector<double>> rayTraceLine(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Line& line)
{
    auto rayLine = Line::alongDirection(origin, direction);
    auto crossing = lineCross(rayLine, line);
    if (!crossing) {
        return {};
    }

    if (dot(*crossing - origin, direction) <= 0) {
        return {};
    }

    return crossing;
}

std::optional<Vector<double>> rayTraceInterval(
    const Vector<double>& origin,
    const Vector<double>& direction,
    const Interval& interval)
{
    auto intervalLine = Line::alongDirection(
        interval.start, interval.end - interval.start);
    auto lineTraceResult = rayTraceLine(origin, direction, intervalLine);
    if (!lineTraceResult) {
        return {};
    }

    if (dot(*lineTraceResult - interval.start,
            *lineTraceResult - interval.end) > 0) {
        return {};
    }

    return lineTraceResult;
}

std::optional<Vector<double>> circleTraceCircle(
    const Circle& tracer,
    const Vector<double>& direction,
    const Circle& target)
{
    auto blownTarget = Circle{target.center, target.radius + tracer.radius};
    return rayTraceCircle(tracer.center, direction, blownTarget);
}

std::optional<Vector<double>> circleTracePoint(
    const Circle& tracer,
    const Vector<double>& direction,
    const Vector<double>& target)
{
    auto blownTarget = Circle(target, tracer.radius);
    return rayTraceCircle(tracer.center, direction, blownTarget);
}

std::optional<Vector<double>> circleTraceLine(
    const Circle& circle, const Vector<double>& direction, const Line& line)
{
    auto offset = dot(line.normal, circle.center) - line.value;
    auto movedLine = line.move(offset);
    auto traceResult = rayTraceLine(circle.center, direction, movedLine);
    if (!traceResult) {
        return {};
    }

    *traceResult -= line.normal * offset;
    return traceResult;
}

std::optional<Vector<double>> circleTraceInterval(
    const Circle& circle,
    const Vector<double>& direction,
    const Interval& interval)
{
    auto intervalLine = Line::alongInterval(interval);
    auto lineTraceResult = circleTraceLine(circle, direction, intervalLine);
    if (!lineTraceResult) {
        return {};
    }

    auto startTraceResult = circleTracePoint(circle, direction, interval.start);
    if (startTraceResult) {
        return startTraceResult;
    }
    auto endTraceResult = circleTracePoint(circle, direction, interval.end);
    if (endTraceResult) {
        return endTraceResult;
    }

    return {};
}

std::optional<Vector<double>> circleTraceAxisBox(
    const Circle& circle,
    const Vector<double>& direction,
    const AxisBox& box)
{
    std::vector<Interval> intervals {
        { box.start, Vector<double>{box.end.x, box.start.y} },
        { box.start, Vector<double>{box.start.x, box.end.y} },
        { Vector<double>{box.start.x, box.end.y}, box.end },
        { Vector<double>{box.end.x, box.start.y}, box.end },
    };

    std::optional<Vector<double>> bestResult;
    double bestDistance = std::numeric_limits<double>::infinity();
    for (const auto& interval : intervals) {
        auto traceResult = circleTraceInterval(circle, direction, interval);
        if (traceResult) {
            double distance = (*traceResult - circle.center).length();
            if (distance < bestDistance) {
                bestDistance = distance;
                bestResult = traceResult;
            }
        }
    }

    return bestResult;
}

} // namespace math
