#include <chrono>
#include <iostream>
#include <string>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

// auto FormatNumbers(const TNumbers& numbers) noexcept
//{
//    auto result{std::string("[")};
//    for (auto i : numbers)
//        result += std::to_string(i) + " ";
//    result.back() = ']';
//    return result;
//}
//
// auto FormatXformMatrix(const TXFormMatrix& xfm) noexcept
//{
//    return std::string{"[" + FormatXYZ(xfm[0]) + "\n " + FormatXYZ(xfm[1]) + "\n " + FormatXYZ(xfm[2]) + "]"};
//}
//

auto FormatXYZ(const TXYZ& xyz) noexcept
{
    auto result{std::string("")};
    for (auto i : xyz)
        result += std::to_string(i) + " ";
    return result;
}

int main()
{
    auto zauberons{Zauberons(1000)};
    {
        std::cout << "Initialization ... ";
        auto xyzInitial{XYZ(50000.0, 50000.0, 50000.0)};
        auto start = std::chrono::steady_clock::now();
        IMap([xyzInitial](const TZauberon& z) { return ZauberonInitialize(1, xyzInitial); }, zauberons);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Elapsed Seconds: " << elapsed_seconds.count() << "s\n\n";
    }

    {
        std::cout << "New Poistion\n";
        auto start = std::chrono::steady_clock::now();
        WithOpen("zauberons.dat", [&zauberons](TOutput& os) {
            PrintLnString("# X Y Z", os);
            DoTimes(10000, [&zauberons, &os](const TIndex i) {
                PrintLnString("# Iteration: " + std::to_string(i), os);
                if (!(i % 1000))
                    std::cout << i << "\n";
                DoTimes(zauberons.size(), [&zauberons](const TIndex i) { ZauberonNewPosition(1, zauberons[i]); });
                DoTimes(zauberons.size(),
                        [&zauberons, &os](const TIndex i) { PrintLnString(FormatXYZ(zauberons[i].xyz), os); });
            });
        });
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Elapsed Seconds: " << elapsed_seconds.count() << "s\n\n";
    }

    return 0;
}
