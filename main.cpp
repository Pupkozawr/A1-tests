#include <algorithm>
#include <iostream>

#include <vector>

#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <set>

//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <random>
#include <stdexcept>


class Point {
public:
  double x = 0;
  double y = 0;
  Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Circle {
public:
  Point center;
  double radius = 0;
  Circle(Point c, double r) : center(c), radius(r) {}
};

bool isInCircle(Point& p, Circle& c) {
  double dx = p.x - c.center.x;
  double dy = p.y - c.center.y;
  return dx * dx + dy * dy <= c.radius * c.radius;
}

bool isInAllCirclesCross(Point& p, std::vector<Circle>& circles) {
  for (auto& circle : circles) {
    if (!isInCircle(p, circle)) {
      return false;
    }
  }
  return true;
}

void CrossingBox(std::vector<Circle>& circles, double& minX, double& maxX, double& minY, double& maxY) {
  minX = 10000000;
  maxX = -10000000;
  minY = 10000000;
  maxY = -10000000;

  for (auto& circle : circles) {
    minX = std::min(minX, circle.center.x - circle.radius);
    maxX = std::max(maxX, circle.center.x + circle.radius);
    minY = std::min(minY, circle.center.y - circle.radius);
    maxY = std::max(maxY, circle.center.y + circle.radius);
  }
}

double randomDouble(double min, double max) {
  return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

double MonteCarlo(std::vector<Circle>& circles, double minX, double maxX, double minY, double maxY, int numPoints) {
  CrossingBox(circles, minX, maxX, minY, maxY);

  // srand(time(0));

  int points = 0;

  for (int i = 0; i < numPoints; ++i) {
    Point p(randomDouble(minX, maxX), randomDouble(minY, maxY));
    if (isInAllCirclesCross(p, circles)) {
      points++;
    }
  }

  double areaRect = (maxX - minX) * (maxY - minY);

  return (static_cast<double>(points) / numPoints) * areaRect;
}

double calculateCorrectSquare() {
  return 0.25 * 3.14159265358979323846 + 1.25 * asin(0.8) - 1.0;
}

int main() {
  // std::vector<Circle> circles;
  //
  // for (int i = 0; i < 3; ++i) {
  //   double x = 0;
  //   double y = 0;
  //   double r = 0;
  //   std::cin >> x >> y >> r;
  //   circles.push_back(Circle(Point(x, y), r));
  // }
  //
  // // srand(clock());
  // int numPoints = 1000000;
  // double area = MonteCarlo(circles, numPoints);
  //
  // std::cout.precision(20);
  // std::cout << area;
  //
  // return 0;
    std::vector<Circle> circles = {
        Circle(Point(1.0, 1.0), 1.0),
        Circle(Point(1.5, 2.0), sqrt(5.0)/2.0),
        Circle(Point(2.0, 1.5), sqrt(5.0)/2.0)
    };

    double exactArea = calculateCorrectSquare();
    std::cout << "Concrete formula: " << exactArea << std::endl;

    double wideMinX = 0.0;
    double wideMaxX = 3.0;
    double wideMinY = 0.0;
    double wideMaxY = 3.0;

    double narrowMinX = 1.0;
    double narrowMaxX = 2.0;
    double narrowMinY = 1.0;
    double narrowMaxY = 2.0;

    std::cout << "Wide square: " << (wideMaxX - wideMinX) * (wideMaxY - wideMinY) << std::endl;
    std::cout << "Tiny square: " << (narrowMaxX - narrowMinX) * (narrowMaxY - narrowMinY) << std::endl;

    std::ofstream outputFile("experiment_results.xls");
    outputFile << "N,WideArea,WideError%,TinyArea,TinyError%\n";

    for (int N = 100; N <= 100000; N += 500) {
        srand(N * 123);

        double wideArea = MonteCarlo(circles, wideMinX, wideMaxX, wideMinY, wideMaxY, N);
        double wideError = std::abs(wideArea - exactArea) / exactArea * 100;

        double narrowArea = MonteCarlo(circles, narrowMinX, narrowMaxX, narrowMinY, narrowMaxY, N);
        double narrowError = std::abs(narrowArea - exactArea) / exactArea * 100;

        outputFile << N << "," << wideArea << "," << wideError << ","
                   << narrowArea << "," << narrowError << "\n";

        std::cout << N << "," << wideArea << "," << wideError << ","
                 << narrowArea << "," << narrowError << "\n";

        if (N % 10000 == 0) {
            std::cout << "N = " << N << ": Wide = " << wideArea
                      << " (error " << wideError << "%), "
                      << "Tiny = " << narrowArea
                      << " (error " << narrowError << "%)" << std::endl;
        }
    }

    outputFile.close();
    std::cout << "Results saved in experiment_results.xls" << std::endl;

    return 0;
}