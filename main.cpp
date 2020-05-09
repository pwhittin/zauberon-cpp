#include <chrono>
#include <iostream>
#include <string>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

int main()
{
    static const TIndex ZAUBERON_COUNT{1000};
    static const TIndex ITERATIONS{10000};
    static const TIndex ITERATION_DISPLAY_DIVISOR{1000};

    TArray(TZauberon, ZAUBERON_COUNT) zauberons;

    {
        Print("Initialization ... ");
        TXYZ xyzInitial;
        XYZ(50000.0, 50000.0, 50000.0, xyzInitial);
        auto start{NOW};
        DoTimes(Count(zauberons),
                [xyzInitial, &zauberons] DOTIMES_FN(i) { ZauberonInitialize(1, xyzInitial, zauberons[i]); });
        auto end{NOW};
        PrintLn("Elapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    {
        auto start{NOW};
        WithOpen("zauberons.dat", [&zauberons] OPEN_FN(os) {
            PrintLn("# X Y Z", os);
            DoTimes(ITERATIONS, [&zauberons, &os] DOTIMES_FN(i) {
                PrintLn("# Iteration: " + std::to_string(i), os);
                if (!(i % ITERATION_DISPLAY_DIVISOR))
                    std::cout << i << "\n";
                DoTimes(Count(zauberons), [&zauberons] DOTIMES_FN(i) { ZauberonNewPosition(1, zauberons[i]); });
//                DoTimes(Count(zauberons),
//                        [&zauberons, &os] DOTIMES_FN(i) { PrintLn(FormatNumbers(zauberons[i].xyz), os); });
            });
        });
        auto end{NOW};
        PrintLn("\nElapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    return 0;
}
