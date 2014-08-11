/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
// NOTE: This code is autogenerated, see src/generator/scripts/testgen.m
 
#include "gtest/gtest.h"
#include "terminal.hh"
#include "execution.hh"
#include "dispatch.hh"
#include "testnodes.hh"

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/**
 * Check TANH node behaves
 */

UNARY_NODE_TEST_SETUP(TANH)

INSTANTIATE_NODE_TEST_CASE_P(TANHTests,TANH,
Values
(
// tanh(real scalar of dimension: 1x1)
new CheckUnary<TANH>(OGRealScalar::create(     -5.0), OGRealScalar::create(     -0.9999092042625951), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(real row vector of dimension: 1x21)
new CheckUnary<TANH>(OGRealDenseMatrix::create({{-5.0,-4.5000000000000000,-4.0,-3.5000000000000000,-3.0,
-2.5000000000000000,-2.0,-1.5000000000000000,-1.0,-0.5000000000000000,
0.0,0.5000000000000000,1.0,1.5000000000000000,2.0,
2.5000000000000000,3.0,3.5000000000000000,4.0,4.5000000000000000,5.0}}), OGRealDenseMatrix::create({{-0.9999092042625951,-0.9997532108480275,-0.9993292997390670,-0.9981778976111987,-0.9950547536867305,
-0.9866142981514303,-0.9640275800758169,-0.9051482536448664,-0.7615941559557649,-0.4621171572600097,
0.0,0.4621171572600097,0.7615941559557649,0.9051482536448664,0.9640275800758169,
0.9866142981514303,0.9950547536867305,0.9981778976111987,0.9993292997390670,0.9997532108480275,0.9999092042625951}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(real column vector of dimension: 21x1)
new CheckUnary<TANH>(OGRealDenseMatrix::create({{-5.0},{-4.5000000000000000},{-4.0},{-3.5000000000000000},{-3.0},
{-2.5000000000000000},{-2.0},{-1.5000000000000000},{-1.0},{-0.5000000000000000},
{0.0},{0.5000000000000000},{1.0},{1.5000000000000000},{2.0},
{2.5000000000000000},{3.0},{3.5000000000000000},{4.0},{4.5000000000000000},
{5.0}}), OGRealDenseMatrix::create({{-0.9999092042625951},{-0.9997532108480275},{-0.9993292997390670},{-0.9981778976111987},{-0.9950547536867305},
{-0.9866142981514303},{-0.9640275800758169},{-0.9051482536448664},{-0.7615941559557649},{-0.4621171572600097},
{0.0},{0.4621171572600097},{0.7615941559557649},{0.9051482536448664},{0.9640275800758169},
{0.9866142981514303},{0.9950547536867305},{0.9981778976111987},{0.9993292997390670},{0.9997532108480275},
{0.9999092042625951}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(real matrix of dimension: 3x7)
new CheckUnary<TANH>(OGRealDenseMatrix::create({{-5.0,-3.5000000000000000,-2.0,-0.5000000000000000,1.0,
2.5000000000000000,4.0},{-4.5000000000000000,-3.0,-1.5000000000000000,0.0,1.5000000000000000,
3.0,4.5000000000000000},{-4.0,-2.5000000000000000,-1.0,0.5000000000000000,2.0,
3.5000000000000000,5.0}}), OGRealDenseMatrix::create({{-0.9999092042625951,-0.9981778976111987,-0.9640275800758169,-0.4621171572600097,0.7615941559557649,
0.9866142981514303,0.9993292997390670},{-0.9997532108480275,-0.9950547536867305,-0.9051482536448664,0.0,0.9051482536448664,
0.9950547536867305,0.9997532108480275},{-0.9993292997390670,-0.9866142981514303,-0.7615941559557649,0.4621171572600097,0.9640275800758169,
0.9981778976111987,0.9999092042625951}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(complex scalar of dimension: 1x1)
new CheckUnary<TANH>(OGComplexScalar::create({     -7.0,     -7.0}), OGComplexScalar::create({     -0.9999997725968209,     -0.0000016474365566}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(complex row vector of dimension: 1x225)
new CheckUnary<TANH>(OGComplexDenseMatrix::create({{{-7.0,-7.0},{-7.0,-6.0},{-7.0,-5.0},{-7.0,-4.0},{-7.0,-3.0},
{-7.0,-2.0},{-7.0,-1.0},{-7.0,0.0},{-7.0,1.0},{-7.0,2.0},
{-7.0,3.0},{-7.0,4.0},{-7.0,5.0},{-7.0,6.0},{-7.0,7.0},
{-6.0,-7.0},{-6.0,-6.0},{-6.0,-5.0},{-6.0,-4.0},{-6.0,-3.0},
{-6.0,-2.0},{-6.0,-1.0},{-6.0,0.0},{-6.0,1.0},{-6.0,2.0},
{-6.0,3.0},{-6.0,4.0},{-6.0,5.0},{-6.0,6.0},{-6.0,7.0},
{-5.0,-7.0},{-5.0,-6.0},{-5.0,-5.0},{-5.0,-4.0},{-5.0,-3.0},
{-5.0,-2.0},{-5.0,-1.0},{-5.0,0.0},{-5.0,1.0},{-5.0,2.0},
{-5.0,3.0},{-5.0,4.0},{-5.0,5.0},{-5.0,6.0},{-5.0,7.0},
{-4.0,-7.0},{-4.0,-6.0},{-4.0,-5.0},{-4.0,-4.0},{-4.0,-3.0},
{-4.0,-2.0},{-4.0,-1.0},{-4.0,0.0},{-4.0,1.0},{-4.0,2.0},
{-4.0,3.0},{-4.0,4.0},{-4.0,5.0},{-4.0,6.0},{-4.0,7.0},
{-3.0,-7.0},{-3.0,-6.0},{-3.0,-5.0},{-3.0,-4.0},{-3.0,-3.0},
{-3.0,-2.0},{-3.0,-1.0},{-3.0,0.0},{-3.0,1.0},{-3.0,2.0},
{-3.0,3.0},{-3.0,4.0},{-3.0,5.0},{-3.0,6.0},{-3.0,7.0},
{-2.0,-7.0},{-2.0,-6.0},{-2.0,-5.0},{-2.0,-4.0},{-2.0,-3.0},
{-2.0,-2.0},{-2.0,-1.0},{-2.0,0.0},{-2.0,1.0},{-2.0,2.0},
{-2.0,3.0},{-2.0,4.0},{-2.0,5.0},{-2.0,6.0},{-2.0,7.0},
{-1.0,-7.0},{-1.0,-6.0},{-1.0,-5.0},{-1.0,-4.0},{-1.0,-3.0},
{-1.0,-2.0},{-1.0,-1.0},{-1.0,0.0},{-1.0,1.0},{-1.0,2.0},
{-1.0,3.0},{-1.0,4.0},{-1.0,5.0},{-1.0,6.0},{-1.0,7.0},
{0.0,-7.0},{0.0,-6.0},{0.0,-5.0},{0.0,-4.0},{0.0,-3.0},
{0.0,-2.0},{0.0,-1.0},{0.0,0.0},{0.0,1.0},{0.0,2.0},
{0.0,3.0},{0.0,4.0},{0.0,5.0},{0.0,6.0},{0.0,7.0},
{1.0,-7.0},{1.0,-6.0},{1.0,-5.0},{1.0,-4.0},{1.0,-3.0},
{1.0,-2.0},{1.0,-1.0},{1.0,0.0},{1.0,1.0},{1.0,2.0},
{1.0,3.0},{1.0,4.0},{1.0,5.0},{1.0,6.0},{1.0,7.0},
{2.0,-7.0},{2.0,-6.0},{2.0,-5.0},{2.0,-4.0},{2.0,-3.0},
{2.0,-2.0},{2.0,-1.0},{2.0,0.0},{2.0,1.0},{2.0,2.0},
{2.0,3.0},{2.0,4.0},{2.0,5.0},{2.0,6.0},{2.0,7.0},
{3.0,-7.0},{3.0,-6.0},{3.0,-5.0},{3.0,-4.0},{3.0,-3.0},
{3.0,-2.0},{3.0,-1.0},{3.0,0.0},{3.0,1.0},{3.0,2.0},
{3.0,3.0},{3.0,4.0},{3.0,5.0},{3.0,6.0},{3.0,7.0},
{4.0,-7.0},{4.0,-6.0},{4.0,-5.0},{4.0,-4.0},{4.0,-3.0},
{4.0,-2.0},{4.0,-1.0},{4.0,0.0},{4.0,1.0},{4.0,2.0},
{4.0,3.0},{4.0,4.0},{4.0,5.0},{4.0,6.0},{4.0,7.0},
{5.0,-7.0},{5.0,-6.0},{5.0,-5.0},{5.0,-4.0},{5.0,-3.0},
{5.0,-2.0},{5.0,-1.0},{5.0,0.0},{5.0,1.0},{5.0,2.0},
{5.0,3.0},{5.0,4.0},{5.0,5.0},{5.0,6.0},{5.0,7.0},
{6.0,-7.0},{6.0,-6.0},{6.0,-5.0},{6.0,-4.0},{6.0,-3.0},
{6.0,-2.0},{6.0,-1.0},{6.0,0.0},{6.0,1.0},{6.0,2.0},
{6.0,3.0},{6.0,4.0},{6.0,5.0},{6.0,6.0},{6.0,7.0},
{7.0,-7.0},{7.0,-6.0},{7.0,-5.0},{7.0,-4.0},{7.0,-3.0},
{7.0,-2.0},{7.0,-1.0},{7.0,0.0},{7.0,1.0},{7.0,2.0},
{7.0,3.0},{7.0,4.0},{7.0,5.0},{7.0,6.0},{7.0,7.0}}}), OGComplexDenseMatrix::create({{{-0.9999997725968209,-0.0000016474365566},{-0.9999985966229836,0.0000008923503301},{-1.0000013954247120,0.0000009047396175},{-1.0000002419735892,-0.0000016453599892},{-0.9999984031828296,0.0000004646832806},
{-1.0000010870466844,0.0000012586073872},{-1.0000006920751880,-0.0000015122148958},{-0.9999983369439446,0.0},{-1.0000006920751880,0.0000015122148958},{-1.0000010870466844,-0.0000012586073872},
{-0.9999984031828296,-0.0000004646832806},{-1.0000002419735892,0.0000016453599892},{-1.0000013954247120,-0.0000009047396175},{-0.9999985966229836,-0.0000008923503301},{-0.9999997725968209,0.0000016474365566},
{-0.9999983196423102,-0.0000121729834498},{-0.9999896303961914,0.0000065935675293},{-1.0000103108981198,0.0000066852313903},{-1.0000017878939040,-0.0000121576760586},{-0.9999882010834399,0.0000034335357991},
{-1.0000080322394340,0.0000092999851809},{-1.0000051137397155,-0.0000111738901060},{-0.9999877116507956,0.0},{-1.0000051137397155,0.0000111738901060},{-1.0000080322394340,-0.0000092999851809},
{-0.9999882010834399,-0.0000034335357991},{-1.0000017878939040,0.0000121576760586},{-1.0000103108981198,-0.0000066852313903},{-0.9999896303961914,-0.0000065935675293},{-0.9999983196423102,0.0000121729834498},
{-0.9999875803117128,-0.0000899458918131},{-0.9999233799277077,0.0000487170126927},{-1.0000761892591825,0.0000494008040731},{-1.0000132074347849,-0.0000898347764697},{-0.9999128201513540,0.0000253686762077},
{-1.0000593501490003,0.0000687216388012},{-1.0000377833796010,-0.0000825671983423},{-0.9999092042625953,0.0},{-1.0000377833796010,0.0000825671983423},{-1.0000593501490003,-0.0000687216388012},
{-0.9999128201513540,-0.0000253686762077},{-1.0000132074347849,0.0000898347764697},{-1.0000761892591825,-0.0000494008040731},{-0.9999233799277077,-0.0000487170126927},{-0.9999875803117128,0.0000899458918131},
{-0.9999080429230788,-0.0006645624514749},{-0.9994339325466381,0.0003597965782916},{-1.0005630461157933,0.0003652030545113},{-1.0000974040746433,-0.0006638501648111},{-0.9993559873814732,0.0001873462046295},
{-1.0004385132020523,0.0005079806234700},{-1.0002790562344657,-0.0006102409213763},{-0.9993292997390670,0.0},{-1.0002790562344657,0.0006102409213763},{-1.0004385132020523,-0.0005079806234700},
{-0.9993559873814732,-0.0001873462046295},{-1.0000974040746433,0.0006638501648111},{-1.0005630461157933,-0.0003652030545113},{-0.9994339325466381,-0.0003597965782916},{-0.9999080429230788,0.0006645624514749},
{-0.9993103079835997,-0.0049075833952323},{-0.9958218066674143,0.0026489645969457},{-1.0041647106948151,0.0027082358362241},{-1.0007095360672329,-0.0049082580674961},{-0.9952503011786028,0.0013786327196593},
{-1.0032386273536096,0.0037640256415042},{-1.0020549882458116,-0.0045171372766584},{-0.9950547536867302,0.0},{-1.0020549882458116,0.0045171372766584},{-1.0032386273536096,-0.0037640256415042},
{-0.9952503011786028,-0.0013786327196593},{-1.0007095360672329,0.0049082580674961},{-1.0041647106948151,-0.0027082358362241},{-0.9958218066674143,-0.0026489645969457},{-0.9993103079835997,0.0049075833952323},
{-0.9943504089532536,-0.0360943135932625},{-0.9693745758917579,0.0190597919760790},{-1.0310080051524910,0.0205530165682556},{-1.0046823121902351,-0.0364233692474037},{-0.9653858790221330,0.0098843750383225},
{-1.0238355945704727,0.0283929528682323},{-1.0147936161466335,-0.0338128260798967},{-0.9640275800758168,0.0},{-1.0147936161466335,0.0338128260798967},{-1.0238355945704727,-0.0283929528682323},
{-0.9653858790221330,-0.0098843750383225},{-1.0046823121902351,0.0364233692474037},{-1.0310080051524910,-0.0205530165682556},{-0.9693745758917579,-0.0190597919760790},{-0.9943504089532536,0.0360943135932625},
{-0.9302187270788692,-0.2540714033150390},{-0.7874123562675460,0.1164930816631243},{-1.2407479829240700,0.1861094776473042},{-1.0028105075835050,-0.2735530828073073},{-0.7680176472869112,0.0591685395660507},
{-1.1667362572409199,0.2434582011857253},{-1.0839233273386946,-0.2717525853195118},{-0.7615941559557649,0.0},{-1.0839233273386946,0.2717525853195118},{-1.1667362572409199,-0.2434582011857253},
{-0.7680176472869112,-0.0591685395660507},{-1.0028105075835050,0.2735530828073073},{-1.2407479829240700,-0.1861094776473042},{-0.7874123562675460,-0.1164930816631243},{-0.9302187270788692,0.2540714033150390},
{0.0,-0.8714479827243188},{0.0,0.2910061913847492},{0.0,3.3805150062465859},{0.0,-1.1578212823495775},{0.0,0.1425465430742778},
{0.0,2.1850398632615189},{0.0,-1.5574077246549021},{0.0,0.0},{0.0,1.5574077246549021},{0.0,-2.1850398632615189},
{0.0,-0.1425465430742778},{0.0,1.1578212823495775},{0.0,-3.3805150062465859},{0.0,-0.2910061913847492},{0.0,0.8714479827243188},
{0.9302187270788692,-0.2540714033150390},{0.7874123562675460,0.1164930816631243},{1.2407479829240700,0.1861094776473042},{1.0028105075835050,-0.2735530828073073},{0.7680176472869112,0.0591685395660507},
{1.1667362572409199,0.2434582011857253},{1.0839233273386946,-0.2717525853195118},{0.7615941559557649,0.0},{1.0839233273386946,0.2717525853195118},{1.1667362572409199,-0.2434582011857253},
{0.7680176472869112,-0.0591685395660507},{1.0028105075835050,0.2735530828073073},{1.2407479829240700,-0.1861094776473042},{0.7874123562675460,-0.1164930816631243},{0.9302187270788692,0.2540714033150390},
{0.9943504089532536,-0.0360943135932625},{0.9693745758917579,0.0190597919760790},{1.0310080051524910,0.0205530165682556},{1.0046823121902351,-0.0364233692474037},{0.9653858790221330,0.0098843750383225},
{1.0238355945704727,0.0283929528682323},{1.0147936161466335,-0.0338128260798967},{0.9640275800758168,0.0},{1.0147936161466335,0.0338128260798967},{1.0238355945704727,-0.0283929528682323},
{0.9653858790221330,-0.0098843750383225},{1.0046823121902351,0.0364233692474037},{1.0310080051524910,-0.0205530165682556},{0.9693745758917579,-0.0190597919760790},{0.9943504089532536,0.0360943135932625},
{0.9993103079835997,-0.0049075833952323},{0.9958218066674143,0.0026489645969457},{1.0041647106948151,0.0027082358362241},{1.0007095360672329,-0.0049082580674961},{0.9952503011786028,0.0013786327196593},
{1.0032386273536096,0.0037640256415042},{1.0020549882458116,-0.0045171372766584},{0.9950547536867302,0.0},{1.0020549882458116,0.0045171372766584},{1.0032386273536096,-0.0037640256415042},
{0.9952503011786028,-0.0013786327196593},{1.0007095360672329,0.0049082580674961},{1.0041647106948151,-0.0027082358362241},{0.9958218066674143,-0.0026489645969457},{0.9993103079835997,0.0049075833952323},
{0.9999080429230788,-0.0006645624514749},{0.9994339325466381,0.0003597965782916},{1.0005630461157933,0.0003652030545113},{1.0000974040746433,-0.0006638501648111},{0.9993559873814732,0.0001873462046295},
{1.0004385132020523,0.0005079806234700},{1.0002790562344657,-0.0006102409213763},{0.9993292997390670,0.0},{1.0002790562344657,0.0006102409213763},{1.0004385132020523,-0.0005079806234700},
{0.9993559873814732,-0.0001873462046295},{1.0000974040746433,0.0006638501648111},{1.0005630461157933,-0.0003652030545113},{0.9994339325466381,-0.0003597965782916},{0.9999080429230788,0.0006645624514749},
{0.9999875803117128,-0.0000899458918131},{0.9999233799277077,0.0000487170126927},{1.0000761892591825,0.0000494008040731},{1.0000132074347849,-0.0000898347764697},{0.9999128201513540,0.0000253686762077},
{1.0000593501490003,0.0000687216388012},{1.0000377833796010,-0.0000825671983423},{0.9999092042625953,0.0},{1.0000377833796010,0.0000825671983423},{1.0000593501490003,-0.0000687216388012},
{0.9999128201513540,-0.0000253686762077},{1.0000132074347849,0.0000898347764697},{1.0000761892591825,-0.0000494008040731},{0.9999233799277077,-0.0000487170126927},{0.9999875803117128,0.0000899458918131},
{0.9999983196423102,-0.0000121729834498},{0.9999896303961914,0.0000065935675293},{1.0000103108981198,0.0000066852313903},{1.0000017878939040,-0.0000121576760586},{0.9999882010834399,0.0000034335357991},
{1.0000080322394340,0.0000092999851809},{1.0000051137397155,-0.0000111738901060},{0.9999877116507956,0.0},{1.0000051137397155,0.0000111738901060},{1.0000080322394340,-0.0000092999851809},
{0.9999882010834399,-0.0000034335357991},{1.0000017878939040,0.0000121576760586},{1.0000103108981198,-0.0000066852313903},{0.9999896303961914,-0.0000065935675293},{0.9999983196423102,0.0000121729834498},
{0.9999997725968209,-0.0000016474365566},{0.9999985966229836,0.0000008923503301},{1.0000013954247120,0.0000009047396175},{1.0000002419735892,-0.0000016453599892},{0.9999984031828296,0.0000004646832806},
{1.0000010870466844,0.0000012586073872},{1.0000006920751880,-0.0000015122148958},{0.9999983369439446,0.0},{1.0000006920751880,0.0000015122148958},{1.0000010870466844,-0.0000012586073872},
{0.9999984031828296,-0.0000004646832806},{1.0000002419735892,0.0000016453599892},{1.0000013954247120,-0.0000009047396175},{0.9999985966229836,-0.0000008923503301},{0.9999997725968209,0.0000016474365566}}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(complex column vector of dimension: 225x1)
new CheckUnary<TANH>(OGComplexDenseMatrix::create({{{-7.0,-7.0}},{{-7.0,-6.0}},{{-7.0,-5.0}},{{-7.0,-4.0}},{{-7.0,-3.0}},
{{-7.0,-2.0}},{{-7.0,-1.0}},{{-7.0,0.0}},{{-7.0,1.0}},{{-7.0,2.0}},
{{-7.0,3.0}},{{-7.0,4.0}},{{-7.0,5.0}},{{-7.0,6.0}},{{-7.0,7.0}},
{{-6.0,-7.0}},{{-6.0,-6.0}},{{-6.0,-5.0}},{{-6.0,-4.0}},{{-6.0,-3.0}},
{{-6.0,-2.0}},{{-6.0,-1.0}},{{-6.0,0.0}},{{-6.0,1.0}},{{-6.0,2.0}},
{{-6.0,3.0}},{{-6.0,4.0}},{{-6.0,5.0}},{{-6.0,6.0}},{{-6.0,7.0}},
{{-5.0,-7.0}},{{-5.0,-6.0}},{{-5.0,-5.0}},{{-5.0,-4.0}},{{-5.0,-3.0}},
{{-5.0,-2.0}},{{-5.0,-1.0}},{{-5.0,0.0}},{{-5.0,1.0}},{{-5.0,2.0}},
{{-5.0,3.0}},{{-5.0,4.0}},{{-5.0,5.0}},{{-5.0,6.0}},{{-5.0,7.0}},
{{-4.0,-7.0}},{{-4.0,-6.0}},{{-4.0,-5.0}},{{-4.0,-4.0}},{{-4.0,-3.0}},
{{-4.0,-2.0}},{{-4.0,-1.0}},{{-4.0,0.0}},{{-4.0,1.0}},{{-4.0,2.0}},
{{-4.0,3.0}},{{-4.0,4.0}},{{-4.0,5.0}},{{-4.0,6.0}},{{-4.0,7.0}},
{{-3.0,-7.0}},{{-3.0,-6.0}},{{-3.0,-5.0}},{{-3.0,-4.0}},{{-3.0,-3.0}},
{{-3.0,-2.0}},{{-3.0,-1.0}},{{-3.0,0.0}},{{-3.0,1.0}},{{-3.0,2.0}},
{{-3.0,3.0}},{{-3.0,4.0}},{{-3.0,5.0}},{{-3.0,6.0}},{{-3.0,7.0}},
{{-2.0,-7.0}},{{-2.0,-6.0}},{{-2.0,-5.0}},{{-2.0,-4.0}},{{-2.0,-3.0}},
{{-2.0,-2.0}},{{-2.0,-1.0}},{{-2.0,0.0}},{{-2.0,1.0}},{{-2.0,2.0}},
{{-2.0,3.0}},{{-2.0,4.0}},{{-2.0,5.0}},{{-2.0,6.0}},{{-2.0,7.0}},
{{-1.0,-7.0}},{{-1.0,-6.0}},{{-1.0,-5.0}},{{-1.0,-4.0}},{{-1.0,-3.0}},
{{-1.0,-2.0}},{{-1.0,-1.0}},{{-1.0,0.0}},{{-1.0,1.0}},{{-1.0,2.0}},
{{-1.0,3.0}},{{-1.0,4.0}},{{-1.0,5.0}},{{-1.0,6.0}},{{-1.0,7.0}},
{{0.0,-7.0}},{{0.0,-6.0}},{{0.0,-5.0}},{{0.0,-4.0}},{{0.0,-3.0}},
{{0.0,-2.0}},{{0.0,-1.0}},{{0.0,0.0}},{{0.0,1.0}},{{0.0,2.0}},
{{0.0,3.0}},{{0.0,4.0}},{{0.0,5.0}},{{0.0,6.0}},{{0.0,7.0}},
{{1.0,-7.0}},{{1.0,-6.0}},{{1.0,-5.0}},{{1.0,-4.0}},{{1.0,-3.0}},
{{1.0,-2.0}},{{1.0,-1.0}},{{1.0,0.0}},{{1.0,1.0}},{{1.0,2.0}},
{{1.0,3.0}},{{1.0,4.0}},{{1.0,5.0}},{{1.0,6.0}},{{1.0,7.0}},
{{2.0,-7.0}},{{2.0,-6.0}},{{2.0,-5.0}},{{2.0,-4.0}},{{2.0,-3.0}},
{{2.0,-2.0}},{{2.0,-1.0}},{{2.0,0.0}},{{2.0,1.0}},{{2.0,2.0}},
{{2.0,3.0}},{{2.0,4.0}},{{2.0,5.0}},{{2.0,6.0}},{{2.0,7.0}},
{{3.0,-7.0}},{{3.0,-6.0}},{{3.0,-5.0}},{{3.0,-4.0}},{{3.0,-3.0}},
{{3.0,-2.0}},{{3.0,-1.0}},{{3.0,0.0}},{{3.0,1.0}},{{3.0,2.0}},
{{3.0,3.0}},{{3.0,4.0}},{{3.0,5.0}},{{3.0,6.0}},{{3.0,7.0}},
{{4.0,-7.0}},{{4.0,-6.0}},{{4.0,-5.0}},{{4.0,-4.0}},{{4.0,-3.0}},
{{4.0,-2.0}},{{4.0,-1.0}},{{4.0,0.0}},{{4.0,1.0}},{{4.0,2.0}},
{{4.0,3.0}},{{4.0,4.0}},{{4.0,5.0}},{{4.0,6.0}},{{4.0,7.0}},
{{5.0,-7.0}},{{5.0,-6.0}},{{5.0,-5.0}},{{5.0,-4.0}},{{5.0,-3.0}},
{{5.0,-2.0}},{{5.0,-1.0}},{{5.0,0.0}},{{5.0,1.0}},{{5.0,2.0}},
{{5.0,3.0}},{{5.0,4.0}},{{5.0,5.0}},{{5.0,6.0}},{{5.0,7.0}},
{{6.0,-7.0}},{{6.0,-6.0}},{{6.0,-5.0}},{{6.0,-4.0}},{{6.0,-3.0}},
{{6.0,-2.0}},{{6.0,-1.0}},{{6.0,0.0}},{{6.0,1.0}},{{6.0,2.0}},
{{6.0,3.0}},{{6.0,4.0}},{{6.0,5.0}},{{6.0,6.0}},{{6.0,7.0}},
{{7.0,-7.0}},{{7.0,-6.0}},{{7.0,-5.0}},{{7.0,-4.0}},{{7.0,-3.0}},
{{7.0,-2.0}},{{7.0,-1.0}},{{7.0,0.0}},{{7.0,1.0}},{{7.0,2.0}},
{{7.0,3.0}},{{7.0,4.0}},{{7.0,5.0}},{{7.0,6.0}},{{7.0,7.0}}}), OGComplexDenseMatrix::create({{{-0.9999997725968209,-0.0000016474365566}},{{-0.9999985966229836,0.0000008923503301}},{{-1.0000013954247120,0.0000009047396175}},{{-1.0000002419735892,-0.0000016453599892}},{{-0.9999984031828296,0.0000004646832806}},
{{-1.0000010870466844,0.0000012586073872}},{{-1.0000006920751880,-0.0000015122148958}},{{-0.9999983369439446,0.0}},{{-1.0000006920751880,0.0000015122148958}},{{-1.0000010870466844,-0.0000012586073872}},
{{-0.9999984031828296,-0.0000004646832806}},{{-1.0000002419735892,0.0000016453599892}},{{-1.0000013954247120,-0.0000009047396175}},{{-0.9999985966229836,-0.0000008923503301}},{{-0.9999997725968209,0.0000016474365566}},
{{-0.9999983196423102,-0.0000121729834498}},{{-0.9999896303961914,0.0000065935675293}},{{-1.0000103108981198,0.0000066852313903}},{{-1.0000017878939040,-0.0000121576760586}},{{-0.9999882010834399,0.0000034335357991}},
{{-1.0000080322394340,0.0000092999851809}},{{-1.0000051137397155,-0.0000111738901060}},{{-0.9999877116507956,0.0}},{{-1.0000051137397155,0.0000111738901060}},{{-1.0000080322394340,-0.0000092999851809}},
{{-0.9999882010834399,-0.0000034335357991}},{{-1.0000017878939040,0.0000121576760586}},{{-1.0000103108981198,-0.0000066852313903}},{{-0.9999896303961914,-0.0000065935675293}},{{-0.9999983196423102,0.0000121729834498}},
{{-0.9999875803117128,-0.0000899458918131}},{{-0.9999233799277077,0.0000487170126927}},{{-1.0000761892591825,0.0000494008040731}},{{-1.0000132074347849,-0.0000898347764697}},{{-0.9999128201513540,0.0000253686762077}},
{{-1.0000593501490003,0.0000687216388012}},{{-1.0000377833796010,-0.0000825671983423}},{{-0.9999092042625953,0.0}},{{-1.0000377833796010,0.0000825671983423}},{{-1.0000593501490003,-0.0000687216388012}},
{{-0.9999128201513540,-0.0000253686762077}},{{-1.0000132074347849,0.0000898347764697}},{{-1.0000761892591825,-0.0000494008040731}},{{-0.9999233799277077,-0.0000487170126927}},{{-0.9999875803117128,0.0000899458918131}},
{{-0.9999080429230788,-0.0006645624514749}},{{-0.9994339325466381,0.0003597965782916}},{{-1.0005630461157933,0.0003652030545113}},{{-1.0000974040746433,-0.0006638501648111}},{{-0.9993559873814732,0.0001873462046295}},
{{-1.0004385132020523,0.0005079806234700}},{{-1.0002790562344657,-0.0006102409213763}},{{-0.9993292997390670,0.0}},{{-1.0002790562344657,0.0006102409213763}},{{-1.0004385132020523,-0.0005079806234700}},
{{-0.9993559873814732,-0.0001873462046295}},{{-1.0000974040746433,0.0006638501648111}},{{-1.0005630461157933,-0.0003652030545113}},{{-0.9994339325466381,-0.0003597965782916}},{{-0.9999080429230788,0.0006645624514749}},
{{-0.9993103079835997,-0.0049075833952323}},{{-0.9958218066674143,0.0026489645969457}},{{-1.0041647106948151,0.0027082358362241}},{{-1.0007095360672329,-0.0049082580674961}},{{-0.9952503011786028,0.0013786327196593}},
{{-1.0032386273536096,0.0037640256415042}},{{-1.0020549882458116,-0.0045171372766584}},{{-0.9950547536867302,0.0}},{{-1.0020549882458116,0.0045171372766584}},{{-1.0032386273536096,-0.0037640256415042}},
{{-0.9952503011786028,-0.0013786327196593}},{{-1.0007095360672329,0.0049082580674961}},{{-1.0041647106948151,-0.0027082358362241}},{{-0.9958218066674143,-0.0026489645969457}},{{-0.9993103079835997,0.0049075833952323}},
{{-0.9943504089532536,-0.0360943135932625}},{{-0.9693745758917579,0.0190597919760790}},{{-1.0310080051524910,0.0205530165682556}},{{-1.0046823121902351,-0.0364233692474037}},{{-0.9653858790221330,0.0098843750383225}},
{{-1.0238355945704727,0.0283929528682323}},{{-1.0147936161466335,-0.0338128260798967}},{{-0.9640275800758168,0.0}},{{-1.0147936161466335,0.0338128260798967}},{{-1.0238355945704727,-0.0283929528682323}},
{{-0.9653858790221330,-0.0098843750383225}},{{-1.0046823121902351,0.0364233692474037}},{{-1.0310080051524910,-0.0205530165682556}},{{-0.9693745758917579,-0.0190597919760790}},{{-0.9943504089532536,0.0360943135932625}},
{{-0.9302187270788692,-0.2540714033150390}},{{-0.7874123562675460,0.1164930816631243}},{{-1.2407479829240700,0.1861094776473042}},{{-1.0028105075835050,-0.2735530828073073}},{{-0.7680176472869112,0.0591685395660507}},
{{-1.1667362572409199,0.2434582011857253}},{{-1.0839233273386946,-0.2717525853195118}},{{-0.7615941559557649,0.0}},{{-1.0839233273386946,0.2717525853195118}},{{-1.1667362572409199,-0.2434582011857253}},
{{-0.7680176472869112,-0.0591685395660507}},{{-1.0028105075835050,0.2735530828073073}},{{-1.2407479829240700,-0.1861094776473042}},{{-0.7874123562675460,-0.1164930816631243}},{{-0.9302187270788692,0.2540714033150390}},
{{0.0,-0.8714479827243188}},{{0.0,0.2910061913847492}},{{0.0,3.3805150062465859}},{{0.0,-1.1578212823495775}},{{0.0,0.1425465430742778}},
{{0.0,2.1850398632615189}},{{0.0,-1.5574077246549021}},{{0.0,0.0}},{{0.0,1.5574077246549021}},{{0.0,-2.1850398632615189}},
{{0.0,-0.1425465430742778}},{{0.0,1.1578212823495775}},{{0.0,-3.3805150062465859}},{{0.0,-0.2910061913847492}},{{0.0,0.8714479827243188}},
{{0.9302187270788692,-0.2540714033150390}},{{0.7874123562675460,0.1164930816631243}},{{1.2407479829240700,0.1861094776473042}},{{1.0028105075835050,-0.2735530828073073}},{{0.7680176472869112,0.0591685395660507}},
{{1.1667362572409199,0.2434582011857253}},{{1.0839233273386946,-0.2717525853195118}},{{0.7615941559557649,0.0}},{{1.0839233273386946,0.2717525853195118}},{{1.1667362572409199,-0.2434582011857253}},
{{0.7680176472869112,-0.0591685395660507}},{{1.0028105075835050,0.2735530828073073}},{{1.2407479829240700,-0.1861094776473042}},{{0.7874123562675460,-0.1164930816631243}},{{0.9302187270788692,0.2540714033150390}},
{{0.9943504089532536,-0.0360943135932625}},{{0.9693745758917579,0.0190597919760790}},{{1.0310080051524910,0.0205530165682556}},{{1.0046823121902351,-0.0364233692474037}},{{0.9653858790221330,0.0098843750383225}},
{{1.0238355945704727,0.0283929528682323}},{{1.0147936161466335,-0.0338128260798967}},{{0.9640275800758168,0.0}},{{1.0147936161466335,0.0338128260798967}},{{1.0238355945704727,-0.0283929528682323}},
{{0.9653858790221330,-0.0098843750383225}},{{1.0046823121902351,0.0364233692474037}},{{1.0310080051524910,-0.0205530165682556}},{{0.9693745758917579,-0.0190597919760790}},{{0.9943504089532536,0.0360943135932625}},
{{0.9993103079835997,-0.0049075833952323}},{{0.9958218066674143,0.0026489645969457}},{{1.0041647106948151,0.0027082358362241}},{{1.0007095360672329,-0.0049082580674961}},{{0.9952503011786028,0.0013786327196593}},
{{1.0032386273536096,0.0037640256415042}},{{1.0020549882458116,-0.0045171372766584}},{{0.9950547536867302,0.0}},{{1.0020549882458116,0.0045171372766584}},{{1.0032386273536096,-0.0037640256415042}},
{{0.9952503011786028,-0.0013786327196593}},{{1.0007095360672329,0.0049082580674961}},{{1.0041647106948151,-0.0027082358362241}},{{0.9958218066674143,-0.0026489645969457}},{{0.9993103079835997,0.0049075833952323}},
{{0.9999080429230788,-0.0006645624514749}},{{0.9994339325466381,0.0003597965782916}},{{1.0005630461157933,0.0003652030545113}},{{1.0000974040746433,-0.0006638501648111}},{{0.9993559873814732,0.0001873462046295}},
{{1.0004385132020523,0.0005079806234700}},{{1.0002790562344657,-0.0006102409213763}},{{0.9993292997390670,0.0}},{{1.0002790562344657,0.0006102409213763}},{{1.0004385132020523,-0.0005079806234700}},
{{0.9993559873814732,-0.0001873462046295}},{{1.0000974040746433,0.0006638501648111}},{{1.0005630461157933,-0.0003652030545113}},{{0.9994339325466381,-0.0003597965782916}},{{0.9999080429230788,0.0006645624514749}},
{{0.9999875803117128,-0.0000899458918131}},{{0.9999233799277077,0.0000487170126927}},{{1.0000761892591825,0.0000494008040731}},{{1.0000132074347849,-0.0000898347764697}},{{0.9999128201513540,0.0000253686762077}},
{{1.0000593501490003,0.0000687216388012}},{{1.0000377833796010,-0.0000825671983423}},{{0.9999092042625953,0.0}},{{1.0000377833796010,0.0000825671983423}},{{1.0000593501490003,-0.0000687216388012}},
{{0.9999128201513540,-0.0000253686762077}},{{1.0000132074347849,0.0000898347764697}},{{1.0000761892591825,-0.0000494008040731}},{{0.9999233799277077,-0.0000487170126927}},{{0.9999875803117128,0.0000899458918131}},
{{0.9999983196423102,-0.0000121729834498}},{{0.9999896303961914,0.0000065935675293}},{{1.0000103108981198,0.0000066852313903}},{{1.0000017878939040,-0.0000121576760586}},{{0.9999882010834399,0.0000034335357991}},
{{1.0000080322394340,0.0000092999851809}},{{1.0000051137397155,-0.0000111738901060}},{{0.9999877116507956,0.0}},{{1.0000051137397155,0.0000111738901060}},{{1.0000080322394340,-0.0000092999851809}},
{{0.9999882010834399,-0.0000034335357991}},{{1.0000017878939040,0.0000121576760586}},{{1.0000103108981198,-0.0000066852313903}},{{0.9999896303961914,-0.0000065935675293}},{{0.9999983196423102,0.0000121729834498}},
{{0.9999997725968209,-0.0000016474365566}},{{0.9999985966229836,0.0000008923503301}},{{1.0000013954247120,0.0000009047396175}},{{1.0000002419735892,-0.0000016453599892}},{{0.9999984031828296,0.0000004646832806}},
{{1.0000010870466844,0.0000012586073872}},{{1.0000006920751880,-0.0000015122148958}},{{0.9999983369439446,0.0}},{{1.0000006920751880,0.0000015122148958}},{{1.0000010870466844,-0.0000012586073872}},
{{0.9999984031828296,-0.0000004646832806}},{{1.0000002419735892,0.0000016453599892}},{{1.0000013954247120,-0.0000009047396175}},{{0.9999985966229836,-0.0000008923503301}},{{0.9999997725968209,0.0000016474365566}}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13),
// tanh(complex matrix of dimension: 9x25)
new CheckUnary<TANH>(OGComplexDenseMatrix::create({{{-7.0,-7.0},{-7.0,2.0},{-6.0,-4.0},{-6.0,5.0},{-5.0,-1.0},
{-4.0,-7.0},{-4.0,2.0},{-3.0,-4.0},{-3.0,5.0},{-2.0,-1.0},
{-1.0,-7.0},{-1.0,2.0},{0.0,-4.0},{0.0,5.0},{1.0,-1.0},
{2.0,-7.0},{2.0,2.0},{3.0,-4.0},{3.0,5.0},{4.0,-1.0},
{5.0,-7.0},{5.0,2.0},{6.0,-4.0},{6.0,5.0},{7.0,-1.0}},{{-7.0,-6.0},{-7.0,3.0},{-6.0,-3.0},{-6.0,6.0},{-5.0,0.0},
{-4.0,-6.0},{-4.0,3.0},{-3.0,-3.0},{-3.0,6.0},{-2.0,0.0},
{-1.0,-6.0},{-1.0,3.0},{0.0,-3.0},{0.0,6.0},{1.0,0.0},
{2.0,-6.0},{2.0,3.0},{3.0,-3.0},{3.0,6.0},{4.0,0.0},
{5.0,-6.0},{5.0,3.0},{6.0,-3.0},{6.0,6.0},{7.0,0.0}},{{-7.0,-5.0},{-7.0,4.0},{-6.0,-2.0},{-6.0,7.0},{-5.0,1.0},
{-4.0,-5.0},{-4.0,4.0},{-3.0,-2.0},{-3.0,7.0},{-2.0,1.0},
{-1.0,-5.0},{-1.0,4.0},{0.0,-2.0},{0.0,7.0},{1.0,1.0},
{2.0,-5.0},{2.0,4.0},{3.0,-2.0},{3.0,7.0},{4.0,1.0},
{5.0,-5.0},{5.0,4.0},{6.0,-2.0},{6.0,7.0},{7.0,1.0}},{{-7.0,-4.0},{-7.0,5.0},{-6.0,-1.0},{-5.0,-7.0},{-5.0,2.0},
{-4.0,-4.0},{-4.0,5.0},{-3.0,-1.0},{-2.0,-7.0},{-2.0,2.0},
{-1.0,-4.0},{-1.0,5.0},{0.0,-1.0},{1.0,-7.0},{1.0,2.0},
{2.0,-4.0},{2.0,5.0},{3.0,-1.0},{4.0,-7.0},{4.0,2.0},
{5.0,-4.0},{5.0,5.0},{6.0,-1.0},{7.0,-7.0},{7.0,2.0}},{{-7.0,-3.0},{-7.0,6.0},{-6.0,0.0},{-5.0,-6.0},{-5.0,3.0},
{-4.0,-3.0},{-4.0,6.0},{-3.0,0.0},{-2.0,-6.0},{-2.0,3.0},
{-1.0,-3.0},{-1.0,6.0},{0.0,0.0},{1.0,-6.0},{1.0,3.0},
{2.0,-3.0},{2.0,6.0},{3.0,0.0},{4.0,-6.0},{4.0,3.0},
{5.0,-3.0},{5.0,6.0},{6.0,0.0},{7.0,-6.0},{7.0,3.0}},
{{-7.0,-2.0},{-7.0,7.0},{-6.0,1.0},{-5.0,-5.0},{-5.0,4.0},
{-4.0,-2.0},{-4.0,7.0},{-3.0,1.0},{-2.0,-5.0},{-2.0,4.0},
{-1.0,-2.0},{-1.0,7.0},{0.0,1.0},{1.0,-5.0},{1.0,4.0},
{2.0,-2.0},{2.0,7.0},{3.0,1.0},{4.0,-5.0},{4.0,4.0},
{5.0,-2.0},{5.0,7.0},{6.0,1.0},{7.0,-5.0},{7.0,4.0}},{{-7.0,-1.0},{-6.0,-7.0},{-6.0,2.0},{-5.0,-4.0},{-5.0,5.0},
{-4.0,-1.0},{-3.0,-7.0},{-3.0,2.0},{-2.0,-4.0},{-2.0,5.0},
{-1.0,-1.0},{0.0,-7.0},{0.0,2.0},{1.0,-4.0},{1.0,5.0},
{2.0,-1.0},{3.0,-7.0},{3.0,2.0},{4.0,-4.0},{4.0,5.0},
{5.0,-1.0},{6.0,-7.0},{6.0,2.0},{7.0,-4.0},{7.0,5.0}},{{-7.0,0.0},{-6.0,-6.0},{-6.0,3.0},{-5.0,-3.0},{-5.0,6.0},
{-4.0,0.0},{-3.0,-6.0},{-3.0,3.0},{-2.0,-3.0},{-2.0,6.0},
{-1.0,0.0},{0.0,-6.0},{0.0,3.0},{1.0,-3.0},{1.0,6.0},
{2.0,0.0},{3.0,-6.0},{3.0,3.0},{4.0,-3.0},{4.0,6.0},
{5.0,0.0},{6.0,-6.0},{6.0,3.0},{7.0,-3.0},{7.0,6.0}},{{-7.0,1.0},{-6.0,-5.0},{-6.0,4.0},{-5.0,-2.0},{-5.0,7.0},
{-4.0,1.0},{-3.0,-5.0},{-3.0,4.0},{-2.0,-2.0},{-2.0,7.0},
{-1.0,1.0},{0.0,-5.0},{0.0,4.0},{1.0,-2.0},{1.0,7.0},
{2.0,1.0},{3.0,-5.0},{3.0,4.0},{4.0,-2.0},{4.0,7.0},
{5.0,1.0},{6.0,-5.0},{6.0,4.0},{7.0,-2.0},{7.0,7.0}}}), OGComplexDenseMatrix::create({{{-0.9999997725968209,-0.0000016474365566},{-1.0000010870466844,-0.0000012586073872},{-1.0000017878939040,-0.0000121576760586},{-1.0000103108981198,-0.0000066852313903},{-1.0000377833796010,-0.0000825671983423},
{-0.9999080429230788,-0.0006645624514749},{-1.0004385132020523,-0.0005079806234700},{-1.0007095360672329,-0.0049082580674961},{-1.0041647106948151,-0.0027082358362241},{-1.0147936161466335,-0.0338128260798967},
{-0.9302187270788692,-0.2540714033150390},{-1.1667362572409199,-0.2434582011857253},{0.0,-1.1578212823495775},{0.0,-3.3805150062465859},{1.0839233273386946,-0.2717525853195118},
{0.9943504089532536,-0.0360943135932625},{1.0238355945704727,-0.0283929528682323},{1.0007095360672329,-0.0049082580674961},{1.0041647106948151,-0.0027082358362241},{1.0002790562344657,-0.0006102409213763},
{0.9999875803117128,-0.0000899458918131},{1.0000593501490003,-0.0000687216388012},{1.0000017878939040,-0.0000121576760586},{1.0000103108981198,-0.0000066852313903},{1.0000006920751880,-0.0000015122148958}},{{-0.9999985966229836,0.0000008923503301},{-0.9999984031828296,-0.0000004646832806},{-0.9999882010834399,0.0000034335357991},{-0.9999896303961914,-0.0000065935675293},{-0.9999092042625953,0.0},
{-0.9994339325466381,0.0003597965782916},{-0.9993559873814732,-0.0001873462046295},{-0.9952503011786028,0.0013786327196593},{-0.9958218066674143,-0.0026489645969457},{-0.9640275800758168,0.0},
{-0.7874123562675460,0.1164930816631243},{-0.7680176472869112,-0.0591685395660507},{0.0,0.1425465430742778},{0.0,-0.2910061913847492},{0.7615941559557649,0.0},
{0.9693745758917579,0.0190597919760790},{0.9653858790221330,-0.0098843750383225},{0.9952503011786028,0.0013786327196593},{0.9958218066674143,-0.0026489645969457},{0.9993292997390670,0.0},
{0.9999233799277077,0.0000487170126927},{0.9999128201513540,-0.0000253686762077},{0.9999882010834399,0.0000034335357991},{0.9999896303961914,-0.0000065935675293},{0.9999983369439446,0.0}},{{-1.0000013954247120,0.0000009047396175},{-1.0000002419735892,0.0000016453599892},{-1.0000080322394340,0.0000092999851809},{-0.9999983196423102,0.0000121729834498},{-1.0000377833796010,0.0000825671983423},
{-1.0005630461157933,0.0003652030545113},{-1.0000974040746433,0.0006638501648111},{-1.0032386273536096,0.0037640256415042},{-0.9993103079835997,0.0049075833952323},{-1.0147936161466335,0.0338128260798967},
{-1.2407479829240700,0.1861094776473042},{-1.0028105075835050,0.2735530828073073},{0.0,2.1850398632615189},{0.0,0.8714479827243188},{1.0839233273386946,0.2717525853195118},
{1.0310080051524910,0.0205530165682556},{1.0046823121902351,0.0364233692474037},{1.0032386273536096,0.0037640256415042},{0.9993103079835997,0.0049075833952323},{1.0002790562344657,0.0006102409213763},
{1.0000761892591825,0.0000494008040731},{1.0000132074347849,0.0000898347764697},{1.0000080322394340,0.0000092999851809},{0.9999983196423102,0.0000121729834498},{1.0000006920751880,0.0000015122148958}},{{-1.0000002419735892,-0.0000016453599892},{-1.0000013954247120,-0.0000009047396175},{-1.0000051137397155,-0.0000111738901060},{-0.9999875803117128,-0.0000899458918131},{-1.0000593501490003,-0.0000687216388012},
{-1.0000974040746433,-0.0006638501648111},{-1.0005630461157933,-0.0003652030545113},{-1.0020549882458116,-0.0045171372766584},{-0.9943504089532536,-0.0360943135932625},{-1.0238355945704727,-0.0283929528682323},
{-1.0028105075835050,-0.2735530828073073},{-1.2407479829240700,-0.1861094776473042},{0.0,-1.5574077246549021},{0.9302187270788692,-0.2540714033150390},{1.1667362572409199,-0.2434582011857253},
{1.0046823121902351,-0.0364233692474037},{1.0310080051524910,-0.0205530165682556},{1.0020549882458116,-0.0045171372766584},{0.9999080429230788,-0.0006645624514749},{1.0004385132020523,-0.0005079806234700},
{1.0000132074347849,-0.0000898347764697},{1.0000761892591825,-0.0000494008040731},{1.0000051137397155,-0.0000111738901060},{0.9999997725968209,-0.0000016474365566},{1.0000010870466844,-0.0000012586073872}},{{-0.9999984031828296,0.0000004646832806},{-0.9999985966229836,-0.0000008923503301},{-0.9999877116507956,0.0},{-0.9999233799277077,0.0000487170126927},{-0.9999128201513540,-0.0000253686762077},
{-0.9993559873814732,0.0001873462046295},{-0.9994339325466381,-0.0003597965782916},{-0.9950547536867302,0.0},{-0.9693745758917579,0.0190597919760790},{-0.9653858790221330,-0.0098843750383225},
{-0.7680176472869112,0.0591685395660507},{-0.7874123562675460,-0.1164930816631243},{0.0,0.0},{0.7874123562675460,0.1164930816631243},{0.7680176472869112,-0.0591685395660507},
{0.9653858790221330,0.0098843750383225},{0.9693745758917579,-0.0190597919760790},{0.9950547536867302,0.0},{0.9994339325466381,0.0003597965782916},{0.9993559873814732,-0.0001873462046295},
{0.9999128201513540,0.0000253686762077},{0.9999233799277077,-0.0000487170126927},{0.9999877116507956,0.0},{0.9999985966229836,0.0000008923503301},{0.9999984031828296,-0.0000004646832806}},
{{-1.0000010870466844,0.0000012586073872},{-0.9999997725968209,0.0000016474365566},{-1.0000051137397155,0.0000111738901060},{-1.0000761892591825,0.0000494008040731},{-1.0000132074347849,0.0000898347764697},
{-1.0004385132020523,0.0005079806234700},{-0.9999080429230788,0.0006645624514749},{-1.0020549882458116,0.0045171372766584},{-1.0310080051524910,0.0205530165682556},{-1.0046823121902351,0.0364233692474037},
{-1.1667362572409199,0.2434582011857253},{-0.9302187270788692,0.2540714033150390},{0.0,1.5574077246549021},{1.2407479829240700,0.1861094776473042},{1.0028105075835050,0.2735530828073073},
{1.0238355945704727,0.0283929528682323},{0.9943504089532536,0.0360943135932625},{1.0020549882458116,0.0045171372766584},{1.0005630461157933,0.0003652030545113},{1.0000974040746433,0.0006638501648111},
{1.0000593501490003,0.0000687216388012},{0.9999875803117128,0.0000899458918131},{1.0000051137397155,0.0000111738901060},{1.0000013954247120,0.0000009047396175},{1.0000002419735892,0.0000016453599892}},{{-1.0000006920751880,-0.0000015122148958},{-0.9999983196423102,-0.0000121729834498},{-1.0000080322394340,-0.0000092999851809},{-1.0000132074347849,-0.0000898347764697},{-1.0000761892591825,-0.0000494008040731},
{-1.0002790562344657,-0.0006102409213763},{-0.9993103079835997,-0.0049075833952323},{-1.0032386273536096,-0.0037640256415042},{-1.0046823121902351,-0.0364233692474037},{-1.0310080051524910,-0.0205530165682556},
{-1.0839233273386946,-0.2717525853195118},{0.0,-0.8714479827243188},{0.0,-2.1850398632615189},{1.0028105075835050,-0.2735530828073073},{1.2407479829240700,-0.1861094776473042},
{1.0147936161466335,-0.0338128260798967},{0.9993103079835997,-0.0049075833952323},{1.0032386273536096,-0.0037640256415042},{1.0000974040746433,-0.0006638501648111},{1.0005630461157933,-0.0003652030545113},
{1.0000377833796010,-0.0000825671983423},{0.9999983196423102,-0.0000121729834498},{1.0000080322394340,-0.0000092999851809},{1.0000002419735892,-0.0000016453599892},{1.0000013954247120,-0.0000009047396175}},{{-0.9999983369439446,0.0},{-0.9999896303961914,0.0000065935675293},{-0.9999882010834399,-0.0000034335357991},{-0.9999128201513540,0.0000253686762077},{-0.9999233799277077,-0.0000487170126927},
{-0.9993292997390670,0.0},{-0.9958218066674143,0.0026489645969457},{-0.9952503011786028,-0.0013786327196593},{-0.9653858790221330,0.0098843750383225},{-0.9693745758917579,-0.0190597919760790},
{-0.7615941559557649,0.0},{0.0,0.2910061913847492},{0.0,-0.1425465430742778},{0.7680176472869112,0.0591685395660507},{0.7874123562675460,-0.1164930816631243},
{0.9640275800758168,0.0},{0.9958218066674143,0.0026489645969457},{0.9952503011786028,-0.0013786327196593},{0.9993559873814732,0.0001873462046295},{0.9994339325466381,-0.0003597965782916},
{0.9999092042625953,0.0},{0.9999896303961914,0.0000065935675293},{0.9999882010834399,-0.0000034335357991},{0.9999984031828296,0.0000004646832806},{0.9999985966229836,-0.0000008923503301}},{{-1.0000006920751880,0.0000015122148958},{-1.0000103108981198,0.0000066852313903},{-1.0000017878939040,0.0000121576760586},{-1.0000593501490003,0.0000687216388012},{-0.9999875803117128,0.0000899458918131},
{-1.0002790562344657,0.0006102409213763},{-1.0041647106948151,0.0027082358362241},{-1.0007095360672329,0.0049082580674961},{-1.0238355945704727,0.0283929528682323},{-0.9943504089532536,0.0360943135932625},
{-1.0839233273386946,0.2717525853195118},{0.0,3.3805150062465859},{0.0,1.1578212823495775},{1.1667362572409199,0.2434582011857253},{0.9302187270788692,0.2540714033150390},
{1.0147936161466335,0.0338128260798967},{1.0041647106948151,0.0027082358362241},{1.0007095360672329,0.0049082580674961},{1.0004385132020523,0.0005079806234700},{0.9999080429230788,0.0006645624514749},
{1.0000377833796010,0.0000825671983423},{1.0000103108981198,0.0000066852313903},{1.0000017878939040,0.0000121576760586},{1.0000010870466844,0.0000012586073872},{0.9999997725968209,0.0000016474365566}}}), MATHSEQUAL,
// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm's and will be fixed shortly.
1e-13, 1e-13)
)
);

