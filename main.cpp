#include <chrono>
#include <iostream>
#include <string>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

int main()
{
    auto zauberons{Zauberons(1000)};
    {
        std::cout << "Initialization ... ";
        auto xyzInitial{XYZ(50000.0, 50000.0, 50000.0)};
        auto start{NOW};
        IMap([xyzInitial](const TZauberon& z) { return ZauberonInitialize(1, xyzInitial); }, zauberons);
        auto end{NOW};
        std::cout << "Elapsed Time: " << FormatElapsedTime(ELAPSED_TIME(end, start)) << "\n\n";
    }

    {
        auto start{NOW};
        WithOpen("zauberons.dat", OPEN_FUNCTION_CAPTURE(&zauberons, os) {
            PrintLn("# X Y Z", os);
            DoTimes(10000000, [&zauberons, &os](const TIndex i) {
                PrintLn("# Iteration: " + std::to_string(i), os);
                if (!(i % 1000))
                    std::cout << i << "\n";
                DoTimes(zauberons.size(), [&zauberons](const TIndex i) { ZauberonNewPosition(1, zauberons[i]); });
                DoTimes(zauberons.size(),
                        [&zauberons, &os](const TIndex i) { PrintLn(FormatNumbers(zauberons[i].xyz), os); });
            });
        });
        auto end{NOW};
        std::cout << "\nElapsed Time: " << FormatElapsedTime(ELAPSED_TIME(end, start)) << "\n\n";
    }

    return 0;
}
