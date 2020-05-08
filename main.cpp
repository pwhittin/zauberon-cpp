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
        Print("Initialization ... ");
        auto xyzInitial{XYZ(50000.0, 50000.0, 50000.0)};
        auto start{NOW};
        IMap([xyzInitial] UNARY_TYPE_NAME(TZauberon&, z) { return ZauberonInitialize(1, xyzInitial); }, zauberons);
        auto end{NOW};
        PrintLn("Elapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    {
        auto start{NOW};
        WithOpen("zauberons.dat", [&zauberons] OPEN_FN(os) {
            PrintLn("# X Y Z", os);
            DoTimes(10000, [&zauberons, &os] DOTIMES_FN(i) {
                PrintLn("# Iteration: " + std::to_string(i), os);
                if (!(i % 1000))
                    std::cout << i << "\n";
                DoTimes(Count(zauberons), [&zauberons] DOTIMES_FN(i) { ZauberonNewPosition(1, zauberons[i]); });
                DoTimes(Count(zauberons),
                        [&zauberons, &os] DOTIMES_FN(i) { PrintLn(FormatNumbers(zauberons[i].xyz), os); });
            });
        });
        auto end{NOW};
        PrintLn("\nElapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    return 0;
}
