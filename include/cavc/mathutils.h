#ifndef CAVC_MATHUTILS_H
#define CAVC_MATHUTILS_H

#include <cassert>
#include <cmath>
#include <iterator>
namespace cavc {
namespace utils {
// absolute and relative threshold to be used for comparing reals
template <typename Real> constexpr Real realThreshold = Real(1e-8);

// absolute and relative precision to be used for reals in geometric computation (e.g. to check for
// singularities)
template <typename Real> constexpr Real realPrecision = Real(1e-5);

template <typename Real> constexpr Real pi = Real(3.14159265358979323846264338327950288);

template <typename Real> constexpr Real tau = Real(2) * pi<Real>;

template <typename Real> bool fuzzyEqual(Real x, Real y, Real epsilon = realThreshold<Real>) {
  return std::abs(x - y) < epsilon;
}

/// Normalize radius to be between 0 and 2PI, e.g. -PI/4 becomes 7PI/8 and 5PI becomes PI.
template <typename Real> Real normalizeRadians(Real angle) {
  if (std::abs(angle - tau<Real>) < Real(0)) {
    return Real(0);
  }

  if (angle >= Real(0) && angle <= tau<Real>) {
    return angle;
  }

  return angle - std::floor(angle / tau<Real>) * tau<Real>;
}

/// Returns the smaller difference between two angles, result is negative if angle2 < angle1.
template <typename Real> Real deltaAngle(Real angle1, Real angle2) {
  Real diff = normalizeRadians(angle2 - angle1);
  if (diff > pi<Real>) {
    diff -= tau<Real>;
  }

  return diff;
}

/// Tests if angle is between a start and end angle (counter clockwise start to end, inclusive).
template <typename Real>
bool angleIsBetween(Real startAngle, Real endAngle, Real testAngle,
                    Real epsilon = realThreshold<Real>) {
  Real endSweep = normalizeRadians(endAngle - startAngle);
  Real midSweep = normalizeRadians(testAngle - startAngle);

  return midSweep < endSweep + epsilon;
}

template <typename Real>
bool angleIsWithinSweep(Real startAngle, Real sweepAngle, Real testAngle,
                        Real epsilon = realThreshold<Real>) {
  Real endAngle = startAngle + sweepAngle;
  if (sweepAngle < Real(0)) {
    return angleIsBetween(endAngle, startAngle, testAngle, epsilon);
  }

  return angleIsBetween(startAngle, endAngle, testAngle, epsilon);
}

/// Returns the solutions to for the quadratic equation -b +/- sqrt (b * b - 4 * a * c) / (2 * a).
template <typename Real>
std::pair<Real, Real> quadraticSolutions(Real a, Real b, Real c, Real discr) {
  // Function avoids loss in precision due to taking the difference of two floating point values
  // that are very near each other in value.
  // See:
  // https://math.stackexchange.com/questions/311382/solving-a-quadratic-equation-with-precision-when-using-floating-point-variables
  assert(fuzzyEqual(b * b - Real(4) * a * c, discr) && "discriminate is not correct");
  Real sqrtDiscr = std::sqrt(discr);
  Real denom = Real(2) * a;
  Real sol1;
  if (b < Real(0)) {
    sol1 = (-b + sqrtDiscr) / denom;
  } else {
    sol1 = (-b - sqrtDiscr) / denom;
  }

  Real sol2 = (c / a) / sol1;

  return std::make_pair(sol1, sol2);
}

template <typename T> std::size_t nextWrappingIndex(std::size_t index, const T &container) {
  if (index == container.size() - 1) {
    return 0;
  }

  return index + 1;
}

template <typename T> std::size_t prevWrappingIndex(std::size_t index, const T &container) {
  if (index == 0) {
    return container.size() - 1;
  }

  return index - 1;
}
} // namespace utils
} // namespace cavc

#endif // CAVC_MATHUTILS_H
