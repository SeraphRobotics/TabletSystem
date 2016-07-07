#include "footrecognizer.h"
#include "DataStructures/scan.h"

// Class performs left/right foot recognizing
FootRecognizer::FootType FootRecognizer::recognize(const QString &scanPath)
{
    auto scan = new Scan(scanPath);
    auto grid = scan->getProcessedXYGrid();

    qreal leftSum = 0.0;
    qreal rightSum = 0.0;
    for (int i = 0; i < grid->nx() / 2; ++i)
    {
        for (int j = 0; j < grid->ny(); ++j)
        {
            leftSum += grid->at(i, j);
            rightSum += grid->at(grid->nx() - 1 - i, j);
        }
    }

    if (leftSum > rightSum)
        return FootType::LeftFoot;

    return FootType::RightFoot;
}
