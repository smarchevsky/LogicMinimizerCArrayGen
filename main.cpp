#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
/*
 * This code was created experimenting with logic minimization
 * SOP (sum of product) minimization basics: https://www.studyforfe.com/blog/logic-minimization/
 * It helps make a logic function like: A && !B && !C || !A && C
 *
 *
 * Espresso heuristic logic minimizer
 * (Seems better version of Karnaugh map, didn't dive into details)
 * Concept:        https://en.wikipedia.org/wiki/Espresso_heuristic_logic_minimizer
 * Implementation: https://github.com/classabbyamp/espresso-logic
 *
 *
 * STEPS to make it work:
 *
 *
 * PREPARE:
 *
 * 1. Clone, and make espresso-logic-minimizer
 *    git clone https://github.com/classabbyamp/espresso-logic
 *    $ cd espresso-src
 *    $ make
 *
 * 2. Clone or copy-paste this code.
 *
 *
 * TRY:
 *
 * 1. Paste you code inside a sourceFunction()
 *
 * 2. Generate output:
 *    - #define WHAT_ARE_WE_DOING GENERATE_TEXT_FOR_ESPRESSO
 *    - Run.
 *    - Copy output.
 *
 * 3. Go to espresso-minimizer folder /espresso-logic/bin/
 *    - Create *.pla file in this folder.
 *    - Copy your generated output to file *.pla file, save it.
 *
 * 4. Launch espresso and retrieve data:
 *    - Run espresso: ./espresso test.pla
 *    - Copy output.
 *    - Replace espressoOutStream string with espresso output.
 *
 * 5. Generate C arrays
 *    - #define WHAT_ARE_WE_DOING GENERATE_ARRAYS_FROM_ESPRESSO_OUT
 *    - Run
 *    - Replace: data0, mask0, data1, mask1 - with generated arrays.
 *
 * 6. Compare outputs.
 *    - #define WHAT_ARE_WE_DOING TEST_MINIMIZED_FUNCTION
 *    - Feel joy :)
 *
 *
 *    Yes, it would be great to generate arrays inside
 *  a single executeble, using espresso-logic code
 *  without copy-paste-save-run-copy-paste-run operations.
 *
 * Maybe later :)
 *
 *
 * This code is hardcoded for 8 bits in and 2 bits out, just a concept.
 * Feel free to make it better!
 */

#define LOG(x) std::cout << x << std::endl;
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

void testMinimizedFunction();
void printLogicMinArray();
void generateDataForEspressoMinimizer();

int main()
{

    // generateDataForEspressoMinimizer();
    printLogicMinArray();
    // testMinimizedFunction();

    return 0;
}

uint8_t minimizedFunction(uint8_t inBits)
{
    uint16_t data0[] = { 0xFF60, 0xDF0F, 0xB717, 0xF743, 0xEE0E, 0xEB20, 0xD710, 0xEB48, 0xFC44, 0xEC20, 0xEC48, 0xB818, 0xD810 };
    uint16_t data1[] = { 0xFF60, 0xF380, 0xED4D, 0xBE0E, 0xED40, 0xEE40, 0xF880, 0xF480, 0xE525, 0xE626, 0xB510, 0xB610, 0xE828, 0xF050, 0xD810 };

    uint8_t out = 0;

    // least bit calculation
    for (int i = 0; i < ARR_SIZE(data0); ++i) {
        uint16_t d = data0[i];
        if (((inBits ^ (d & 0xFF)) & (d >> 8)) == 0) {
            out |= 1;
            break;
        }
    }

    // second bit calculation
    for (int i = 0; i < ARR_SIZE(data1); ++i) {
        uint16_t d = data1[i];
        if (((inBits ^ (d & 0xFF)) & (d >> 8)) == 0) {
            out |= 2;
            break;
        }
    }

    // You can generate POS (product of sum) instead of SOP (sum of product)
    // inverting meaningful meaningful output bits in source function and "return ~out & 0b11" here
    // POS can be faster in case of a lot of 0 outputs (it breaks a loop earlier)

    return out;
}

uint8_t sourceFunction(uint8_t input)
{
    // bunch-of-if-else example
    if (input > 13 && input < 27)
        return 0b11;
    if (input > 23 && input < 37)
        return 0b01;
    if (input > 33 && input < 47)
        return 0b10;
    if (input > 43 && input < 57)
        return 0b11;
    if (input > 53 && input < 67)
        return 0b10;
    if (input > 63 && input < 77)
        return 0b01;
    if (input > 73 && input < 87)
        return 0b10;
    if (input > 83 && input < 97)
        return 0b11;
    if (input >= 128 && input < 141)
        return 0b10;
    return 0;
}

void generateDataForEspressoMinimizer()
{
    printf(".i 8\n");
    printf(".o 2\n");
    for (int i = 0; i < 256; ++i) {
        uint8_t result = sourceFunction(i);
        printf("%.8b %.2b\n", i, result);
    }
    printf(".e\n");
}

void testMinimizedFunction()
{
    bool everythingIsFine = true;
    for (int i = 0; i < 256; ++i) {
        auto sourceResult = sourceFunction(i);
        auto minimizedResult = minimizedFunction(i);
        if (sourceResult != minimizedResult) {
            printf("For %.8b source: %.2b, minimized: %.2b\n",
                i, sourceResult, minimizedResult);
            everythingIsFine = false;
        }
    }
    printf(everythingIsFine ? "Yay! Seems no error!\n" : "Seems error occured.\n");
}

void printLogicMinArray()
{
    // paste espresso output here
    std::istringstream espressoOutStream(R"(
01100000 11
00-01111 01
0-01-111 01
0100-011 01
1000--00 10
000-111- 01
001-0-00 01
00-1-000 01
010-1-00 01
010-11-1 10
0-00111- 10
010001-- 01
010-00-0 10
010-000- 10
10000--- 10
1000-0-- 10
001--1-1 10
001--11- 10
001-00-- 01
0-01-0-0 10
0-01-00- 10
010-10-- 01
001-1--- 10
0-011--- 01
0101---- 10
00-10--- 11
)");

    // Output text is four arrays data
    // (TODO: single array with masks for all bits)

    // data0 mask0 for the least bit
    // data1 mask1 for the second bit

    // Data of '-1----01', all '-' replaced with 0: 01000001
    // It can be 1, nothing changes, because it will be masked

    // Mask of '-1----01', all '-' replaced with 0 (bits we don't care)
    // all other digits - with 1 (we care): 01000011

    std::string readingLine;

    auto doStuff = [](std::ostringstream& dataStream,
                       const std::string& readingLine) {
        static std::string dataLine, maskLine;

        dataLine = readingLine;
        std::replace(dataLine.begin(), dataLine.end(), '-', '0');

        maskLine = readingLine;
        std::replace(maskLine.begin(), maskLine.end(), '0', '1');
        std::replace(maskLine.begin(), maskLine.end(), '-', '0');

        std::bitset<8> dataBits(dataLine);
        std::bitset<8> maskBits(maskLine);

        dataStream << "0x"
                   << std::setfill('0') << std::setw(2) << maskBits.to_ulong()
                   << std::setfill('0') << std::setw(2) << dataBits.to_ulong();
    };

    std::ostringstream dataStream0, dataStream1;
    dataStream0 << std::uppercase << std::hex;
    dataStream1 << std::uppercase << std::hex;

    bool first0 = true, first1 = true;
    dataStream0 << "uint16_t data0[] = { ";
    dataStream1 << "uint16_t data1[] = { ";

    while (std::getline(espressoOutStream, readingLine)) {
        if (readingLine.size() != (8 + 1 + 2))
            continue;
        if (readingLine[10] == '1') {
            if (first0) {
                first0 = false;
            } else {
                dataStream0 << ", ";
            }
            doStuff(dataStream0, readingLine);
        }

        if (readingLine[9] == '1') {
            if (first1) {
                first1 = false;
            } else {
                dataStream1 << ", ";
            }
            doStuff(dataStream1, readingLine);
        }
    }

    dataStream0 << " };";
    dataStream1 << " };";

    LOG(dataStream0.str());
    LOG(dataStream1.str());
}
