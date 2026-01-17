#include <cmath>

float lerp(const float x) {
    return x;
}

float easeOutQuad(const float x) {
    return 1 - (1 - x) * (1 - x);
}

float easeInOutBack(const float x) {
    const float c1 = 1.70158;
    const float c2 = c1 * 1.525;

    return x < 0.5
    ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;

}
float easeOutBounce(float x) {
const float n1 = 7.5625;
const float d1 = 2.75;
    
    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        x -= 1.5 / d1;
        return n1 * x * x + 0.75;
    } else if (x < 2.5 / d1) {
        x -= 2.25 / d1;
        return n1 * x * x + 0.9375;
    } else {
        x -= 2.625 / d1;
        return n1 * x * x + 0.984375;
    }

}

float easeInOutBounce(const float x) {
return x < 0.5
  ? (1 - easeOutBounce(1 - 2 * x)) / 2
  : (1 + easeOutBounce(2 * x - 1)) / 2;

}

float easeOutBack(const float x) {
    const float c1 = 1.70158;
    const float c3 = c1 + 1;

    return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
}

float easeInOutQuart(const float x) {
    return x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2;
}
