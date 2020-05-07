#include <iostream>
#include <string>
#include "schneider.h"

using namespace schneider;

auto FormatNumbers(const TNumbers& numbers) noexcept
{
    auto result{std::string("[")};
    for (auto i : numbers)
        result += std::to_string(i) + " ";
    result.back() = ']';
    return result;
}

auto FormatXYZ(const TXYZ& xyz) noexcept
{
    return FormatNumbers(xyz);
}

auto FormatXformMatrix(const TXFormMatrix& xfm) noexcept
{
    return std::string{"[" + FormatXYZ(xfm[0]) + "\n " + FormatXYZ(xfm[1]) + "\n " + FormatXYZ(xfm[2]) + "]"};
}

int main()
{
    TPitchYawRoll pyr{XYZ(1, 2, 3)};
    auto xfm{XFormMatrix(pyr)};
    TXYZ xyz{XYZ(11, 22, 33)};
    auto xyzRotated{Rotate3D(xfm, xyz)};
    TZauberon zauberon{ZauberonInitialize(1.0, xyz)};
    ZauberonNewPosition(1.0, zauberon);

    std::cout << "\n";
    std::cout << FormatNumbers(pyr) << "\n";
    std::cout << "\n";
    std::cout << FormatXformMatrix(xfm) << "\n";
    std::cout << "\n";
    std::cout << FormatXYZ(xyz) << "\n";
    std::cout << "\n";
    std::cout << FormatXYZ(xyzRotated) << "\n";
    return 0;
}
