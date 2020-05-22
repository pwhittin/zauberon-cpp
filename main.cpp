#include <chrono>
#include <iostream>
#include <string>
#include "functional-utils.h"
#include "schneider.h"

using namespace functional_utils;
using namespace schneider;

int main()
{
    static const TIndex ZAUBERON_COUNT{1};
    static const TIndex ITERATIONS{10000};
    static const TIndex ITERATION_DISPLAY_DIVISOR{ITERATIONS / 10};

    TArray(TZauberon, ZAUBERON_COUNT) zauberons;

    {
        Print("Initialization ... ");
        auto start{NOW};
        DoTimes(Count(zauberons), [&zauberons] DOTIMES_FN(i) { ZauberonInitialize(zauberons[i]); });
        auto end{NOW};
        PrintLn("Elapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    {
        auto start{NOW};
        WithOpen("zauberons.dat", [&zauberons] OPEN_FN(os) {
            auto maxX{std::numeric_limits<TNumber>::min()};
            auto maxY{std::numeric_limits<TNumber>::min()};
            auto minX{std::numeric_limits<TNumber>::max()};
            auto minY{std::numeric_limits<TNumber>::max()};
            PrintLn("# X Y Z", os);
            DoTimes(ITERATIONS, [&maxX, &maxY, &minX, &minY, &os, &zauberons] DOTIMES_FN(i) {
                PrintLn("# Iteration: " + std::to_string(i), os);
                if (!(i % ITERATION_DISPLAY_DIVISOR))
                    std::cout << i << "\n";
                DoTimes(Count(zauberons), [&zauberons] DOTIMES_FN(i) { ZauberonNewPosition(zauberons[i]); });
                DoTimes(Count(zauberons), [&maxX, &maxY, &minX, &minY, &os, &zauberons] DOTIMES_FN(i) {
                    PrintLn(FormatNumbers(zauberons[i].xyzGrid), os);
                    maxX = (zauberons[i].xyzGrid[XYZ_X] > maxX) ? zauberons[i].xyzGrid[XYZ_X] : maxX;
                    maxY = (zauberons[i].xyzGrid[XYZ_Y] > maxY) ? zauberons[i].xyzGrid[XYZ_Y] : maxY;
                    minX = (zauberons[i].xyzGrid[XYZ_X] < minX) ? zauberons[i].xyzGrid[XYZ_X] : minX;
                    minY = (zauberons[i].xyzGrid[XYZ_Y] < minY) ? zauberons[i].xyzGrid[XYZ_Y] : minY;
                });
            });
            PrintLn("");
            PrintLn("set xrange [" + std::to_string(minX) + ":" + std::to_string(maxX) + "]");
            PrintLn("set yrange [" + std::to_string(minY) + ":" + std::to_string(maxY) + "]");
            PrintLn("set ticslevel 0");
            PrintLn("splot \"zauberons.dat\" u 1:2:3 with dots");
            PrintLn("");
        });
        auto end{NOW};
        PrintLn("\nElapsed Time: " + FormatElapsedTime(ELAPSED_TIME(end, start)) + "\n");
    }

    return 0;
}
