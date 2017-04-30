
/**************************************************************************************************
 *                                                                                                *
 *                                      SSSSS    PPPPPP    SSSSS                                  * 
 *                                    SS   SS   PP   PP  SS   SS                                  * 
 *                                   SS        PP   PP  SS                                        *
 *                                   SSSSS    PPPPPP    SSSSS                                     *
 *                                      SS   PP            SS                                     *
 *                                SS   SS   PP       SS   SS                                      *
 *                                SSSSS    PP        SSSSS                                        *
 *                                                                                                *
 *                               SIGNAL  PROCESSING  SOLUTIONS                                    *
 *                                                                                                *
 **************************************************************************************************
 *                                                                                                *
 *                  Project:                                  *
 *                                                                                                *
 *                  Author:     Michael E. Spencer, Ph.D.                                         *
 *                                                                                                *
 *                  Copyright(c) Signal Processing Solutions, 2007                                *
 *                                                                                                *
 **************************************************************************************************
 *
 * File: algCreateBlocks.c
 *
 * This function calls XXX_create to create all the DSP Blocks, load power-up default parameters and
 * compute dependent parameters
 *
 *
 *
 */


#include <algorithms.h>
//#include <algCreateBlocks.h>
#include <main.h>

/* trace LOG buffer is used to print runtime information */
extern far LOG_Obj trace;



// Declare all DSP Block handles used in ASACS processing
pGAIN   GAIN_Handles[ GAIN_BLOCK_MAX_INSTANCE ];
pIIR    IIR_Handles[ IIR_BLOCK_MAX_INSTANCE ];
pFIR    FIR_Handles[ FIR_BLOCK_MAX_INSTANCE ];

pDRC    DRC_Handles[ DRC_BLOCK_MAX_INSTANCE ];
//pDEFT DEFT_Handles[ DEFT_BLOCK_MAX_INSTANCE ];
//pAFC  AFC_Handles[ AFC_BLOCK_MAX_INSTANCE ];
//pDRE  DRE_Handles[ DRE_BLOCK_MAX_INSTANCE ];
//pANC  ANC_Handles[ ANC_BLOCK_MAX_INSTANCE ];
//pTEST TEST_Handles[ TEST_BLOCK_MAX_INSTANCE ];



//
// Load some IIR filter params for testing:
//
// Designed with IIR_test_code.m

// HP Filter
// fs =      44100;
// FilterOrder =          6;
// PassType =          2;
// FilterType =          1;
// CutoffFreq =        600;
const int Nsos1 =          3;
const float sos1[] =
{
   9.4641917168837209e-001,   -1.8928383433767442e+000,   9.4641917168837209e-001,   -1.9496134525762481e+000,   9.5675887456014519e-001,
   9.4641917168837209e-001,   -1.8928383433767442e+000,   9.4641917168837209e-001,   -1.8792398422342265e+000,   8.8612734173520291e-001,
   9.4641917168837209e-001,   -1.8928383433767442e+000,   9.4641917168837209e-001,   -1.8408757290520066e+000,   8.4762262233313834e-001,
};



// LP Filter
// fs =      44100;
// FilterOrder =          6;
// PassType =          1;
// FilterType =          1;
// CutoffFreq =       7000;
const int Nsos2 =          3;
const float sos2[] =
{
   1.5043110443185759e-001,   3.0086220886371517e-001,   1.5043110443185759e-001,   -8.9130887053751429e-001,   6.4282556143091962e-001,
   1.5043110443185759e-001,   3.0086220886371517e-001,   1.5043110443185759e-001,   -6.8074070168769163e-001,   2.5471456910838702e-001,
   1.5043110443185759e-001,   3.0086220886371517e-001,   1.5043110443185759e-001,   -5.9903438777588691e-001,   1.0411669505866138e-001,
};


// FIR1
const int N1 =        257;
const float coefs1[] =
{
   -1.5565617664642917e-005,
   1.1892404166180428e-004,
   1.9829715735772207e-004,
   1.8265025356118121e-004,
   7.6364700514703701e-005,
   -7.2385765669724691e-005,
   -1.9269920469313013e-004,
   -2.2378704971428849e-004,
   -1.4487515873851459e-004,
   1.2150514910610419e-005,
   1.7461228534669456e-004,
   2.6163142546913341e-004,
   2.2338931847975732e-004,
   6.8488241896466615e-005,
   -1.3514277294019144e-004,
   -2.8879579474842224e-004,
   -3.0969914970274887e-004,
   -1.7436096104414997e-004,
   6.3618910858238918e-005,
   2.9296185451276670e-004,
   3.9546535442988931e-004,
   3.0562643347107574e-004,
   4.9648928353139367e-005,
   -2.5835988660973745e-004,
   -4.6568317992287054e-004,
   -4.5536453520422261e-004,
   -2.1008689205629543e-004,
   1.6847259370095471e-004,
   4.9954158051441599e-004,
   6.0809940282264587e-004,
   4.1560033048260674e-004,
   -9.6656180304261387e-006,
   -4.7276860950942691e-004,
   -7.3970424907400056e-004,
   -6.5395169860376452e-004,
   -2.2474875075452754e-004,
   3.6131833758048365e-004,
   8.1896829139003016e-004,
   9.0136113046344966e-004,
   5.3064536876170612e-004,
   -1.4601664773659841e-004,
   -8.1087559085595419e-004,
   -1.1228006318814421e-003,
   -8.9010213079720709e-004,
   -1.8241953021613531e-004,
   6.8138492522356632e-004,
   1.2742539870800459e-003,
   1.2699390177078613e-003,
   6.1871005007415019e-004,
   -4.0324529779596713e-004,
   -1.3069563366447101e-003,
   -1.6222794415404185e-003,
   -1.1394494791525443e-003,
   -3.7829112897910110e-005,
   1.1733430468723902e-003,
   1.8874059234301281e-003,
   1.7013549718930716e-003,
   6.3743400143731108e-004,
   -8.3416601576270333e-004,
   -1.9988960137202741e-003,
   -2.2418558908844401e-003,
   -1.3680398359378414e-003,
   2.6601699904907339e-004,
   1.8907155456728078e-003,
   2.6822995804759052e-003,
   2.1766010635092584e-003,
   5.3163225636147272e-004,
   -1.5056569994380789e-003,
   -2.9337296251462936e-003,
   -2.9847641885861966e-003,
   -1.5307772127536930e-003,
   8.0428398056027136e-004,
   2.9048527578473364e-003,
   3.6919304211332502e-003,
   2.6714019455650875e-003,
   2.2658243366318484e-004,
   -2.5114920302862840e-003,
   -4.1810576839477146e-003,
   -3.8607100515155785e-003,
   -1.5667921613890016e-003,
   1.6865701098743322e-003,
   4.3266765179730956e-003,
   4.9752201514871967e-003,
   3.1580028097992949e-003,
   -3.8951452645082465e-004,
   -4.0041735729073125e-003,
   -5.8653765910166447e-003,
   -4.9014795581926018e-003,
   -1.3860481752129841e-003,
   3.0989730109304723e-003,
   6.3616772688077300e-003,
   6.6584665069522441e-003,
   3.6073298670734884e-003,
   -1.5137749219455868e-003,
   -6.2804299040400430e-003,
   -8.2521904329351664e-003,
   -6.2009272465357087e-003,
   -8.2868521022166810e-004,
   5.4258018938933006e-003,
   9.4691324007053386e-003,
   9.0553046904353011e-003,
   3.9913316416811655e-003,
   -3.5818961036824626e-003,
   -1.0054356424262011e-002,
   -1.2027222329619471e-002,
   -8.0446118721010131e-003,
   4.8128091811767466e-004,
   9.6887017955709399e-003,
   1.4951678641714351e-002,
   1.3123430009181569e-002,
   4.2852888968802957e-003,
   -7.9146493259390997e-003,
   -1.7654528695130147e-002,
   -1.9576891000579695e-002,
   -1.1530849426153051e-002,
   3.8992031391450552e-003,
   1.9966612072870600e-002,
   2.8435950257325364e-002,
   2.3376678782207151e-002,
   4.4698810084459655e-003,
   -2.1738019562013999e-002,
   -4.3454476168807446e-002,
   -4.7777367618186607e-002,
   -2.5873330053980403e-002,
   2.2851072067329566e-002,
   8.9379796739677525e-002,
   1.5729270154913472e-001,
   2.0792318176779839e-001,
   2.2664086328874739e-001,
   2.0792318176779839e-001,
   1.5729270154913472e-001,
   8.9379796739677525e-002,
   2.2851072067329566e-002,
   -2.5873330053980403e-002,
   -4.7777367618186607e-002,
   -4.3454476168807446e-002,
   -2.1738019562013999e-002,
   4.4698810084459655e-003,
   2.3376678782207151e-002,
   2.8435950257325364e-002,
   1.9966612072870600e-002,
   3.8992031391450552e-003,
   -1.1530849426153051e-002,
   -1.9576891000579695e-002,
   -1.7654528695130147e-002,
   -7.9146493259390997e-003,
   4.2852888968802957e-003,
   1.3123430009181569e-002,
   1.4951678641714351e-002,
   9.6887017955709399e-003,
   4.8128091811767466e-004,
   -8.0446118721010131e-003,
   -1.2027222329619471e-002,
   -1.0054356424262011e-002,
   -3.5818961036824626e-003,
   3.9913316416811655e-003,
   9.0553046904353011e-003,
   9.4691324007053386e-003,
   5.4258018938933006e-003,
   -8.2868521022166810e-004,
   -6.2009272465357087e-003,
   -8.2521904329351664e-003,
   -6.2804299040400430e-003,
   -1.5137749219455868e-003,
   3.6073298670734884e-003,
   6.6584665069522441e-003,
   6.3616772688077300e-003,
   3.0989730109304723e-003,
   -1.3860481752129841e-003,
   -4.9014795581926018e-003,
   -5.8653765910166447e-003,
   -4.0041735729073125e-003,
   -3.8951452645082465e-004,
   3.1580028097992949e-003,
   4.9752201514871967e-003,
   4.3266765179730956e-003,
   1.6865701098743322e-003,
   -1.5667921613890016e-003,
   -3.8607100515155785e-003,
   -4.1810576839477146e-003,
   -2.5114920302862840e-003,
   2.2658243366318484e-004,
   2.6714019455650875e-003,
   3.6919304211332502e-003,
   2.9048527578473364e-003,
   8.0428398056027136e-004,
   -1.5307772127536930e-003,
   -2.9847641885861966e-003,
   -2.9337296251462936e-003,
   -1.5056569994380789e-003,
   5.3163225636147272e-004,
   2.1766010635092584e-003,
   2.6822995804759052e-003,
   1.8907155456728078e-003,
   2.6601699904907339e-004,
   -1.3680398359378414e-003,
   -2.2418558908844401e-003,
   -1.9988960137202741e-003,
   -8.3416601576270333e-004,
   6.3743400143731108e-004,
   1.7013549718930716e-003,
   1.8874059234301281e-003,
   1.1733430468723902e-003,
   -3.7829112897910110e-005,
   -1.1394494791525443e-003,
   -1.6222794415404185e-003,
   -1.3069563366447101e-003,
   -4.0324529779596713e-004,
   6.1871005007415019e-004,
   1.2699390177078613e-003,
   1.2742539870800459e-003,
   6.8138492522356632e-004,
   -1.8241953021613531e-004,
   -8.9010213079720709e-004,
   -1.1228006318814421e-003,
   -8.1087559085595419e-004,
   -1.4601664773659841e-004,
   5.3064536876170612e-004,
   9.0136113046344966e-004,
   8.1896829139003016e-004,
   3.6131833758048365e-004,
   -2.2474875075452754e-004,
   -6.5395169860376452e-004,
   -7.3970424907400056e-004,
   -4.7276860950942691e-004,
   -9.6656180304261387e-006,
   4.1560033048260674e-004,
   6.0809940282264587e-004,
   4.9954158051441599e-004,
   1.6847259370095471e-004,
   -2.1008689205629543e-004,
   -4.5536453520422261e-004,
   -4.6568317992287054e-004,
   -2.5835988660973745e-004,
   4.9648928353139367e-005,
   3.0562643347107574e-004,
   3.9546535442988931e-004,
   2.9296185451276670e-004,
   6.3618910858238918e-005,
   -1.7436096104414997e-004,
   -3.0969914970274887e-004,
   -2.8879579474842224e-004,
   -1.3514277294019144e-004,
   6.8488241896466615e-005,
   2.2338931847975732e-004,
   2.6163142546913341e-004,
   1.7461228534669456e-004,
   1.2150514910610419e-005,
   -1.4487515873851459e-004,
   -2.2378704971428849e-004,
   -1.9269920469313013e-004,
   -7.2385765669724691e-005,
   7.6364700514703701e-005,
   1.8265025356118121e-004,
   1.9829715735772207e-004,
   1.1892404166180428e-004,
   -1.5565617664642917e-005,
};


//=========================================================================
// Alocate memory for handles, parameters, state vectors, etc. and load
// default parameters for ASACS.
//=========================================================================
void algCreateBlocks()
{
    int i, pp;


    LOG_printf(&trace, "    Called algCreateBlocks() (.c)");


    // Create all the DSP Block handle structures:

    for( i = 0; i < GAIN_BLOCK_MAX_INSTANCE ; i++ )
        GAIN_Handles[i] = GAIN_create();

    for( i = 0; i < IIR_BLOCK_MAX_INSTANCE ; i++ )
        IIR_Handles[i] = IIR_create();

    for( i = 0; i < FIR_BLOCK_MAX_INSTANCE ; i++ )
        FIR_Handles[i] = FIR_create();

    for( i = 0; i < DRC_BLOCK_MAX_INSTANCE ; i++ )
        DRC_Handles[i] = DRC_create();

    for( i = 0; i < DEFT_BLOCK_MAX_INSTANCE ; i++ )
//      DEFT_Handles[i] = DEFT_create();

    for( i = 0; i < AFC_BLOCK_MAX_INSTANCE ; i++ )
//      AFC_Handles[i] = AFC_create();

    for( i = 0; i < DRE_BLOCK_MAX_INSTANCE ; i++ )
//      DRE_Handles[i] = DRE_create();

    for( i = 0; i < ANC_BLOCK_MAX_INSTANCE ; i++ )
//      ANC_Handles[i] = ANC_create();

    for( i = 0; i < TEST_BLOCK_MAX_INSTANCE ; i++ )
//      TEST_Handles[i] = TEST_create();



// For now, Load Factory defaults:

    GAIN_Handles[0]->gain = 1.0;        // fixthis. not the place to set gains
    GAIN_update( GAIN_Handles[0] );

    GAIN_Handles[2]->gain = 1.0;    
    GAIN_update( GAIN_Handles[2] );

    GAIN_Handles[3]->gain = 1.0;    
    GAIN_update( GAIN_Handles[3] );

    GAIN_Handles[4]->gain = 1.0;
    GAIN_update( GAIN_Handles[4] );

    GAIN_Handles[5]->gain = 1.0;
    GAIN_update( GAIN_Handles[5] );

    GAIN_Handles[6]->gain = 1.0;
    GAIN_update( GAIN_Handles[6] );

    GAIN_Handles[7]->gain = 1.0;
    GAIN_update( GAIN_Handles[7] );





    // HP Filters:

    // Load sos coefs directly into handle:
    pp = IIR_Handles[0]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[0]->Nsos[pp] = Nsos1;               // set number of sos's
    for( i = 0; i < 5*Nsos1; i++ )
        *( IIR_Handles[0]->sos[pp] + i) = sos1[i];  // write coefs from const space
    IIR_Handles[0]->pingPong = pp;                  // update pingPong to point to new coefs

    // Load sos coefs directly into handle:
    pp = IIR_Handles[2]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[2]->Nsos[pp] = Nsos1;               // set number of sos's
    for( i = 0; i < 5*Nsos1; i++ )
        *( IIR_Handles[2]->sos[pp] + i) = sos1[i];  // write coefs from const space
    IIR_Handles[2]->pingPong = pp;                  // update pingPong to point to new coefs

    // Load sos coefs directly into handle:
    pp = IIR_Handles[0]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[3]->Nsos[pp] = Nsos1;               // set number of sos's
    for( i = 0; i < 5*Nsos1; i++ )
        *( IIR_Handles[3]->sos[pp] + i) = sos1[i];  // write coefs from const space
    IIR_Handles[3]->pingPong = pp;                  // update pingPong to point to new coefs

    // Load sos coefs directly into handle:
    pp = IIR_Handles[5]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[5]->Nsos[pp] = Nsos1;               // set number of sos's
    for( i = 0; i < 5*Nsos1; i++ )
        *( IIR_Handles[5]->sos[pp] + i) = sos1[i];  // write coefs from const space
    IIR_Handles[5]->pingPong = pp;                  // update pingPong to point to new coefs


    // LP Filters:

    // Load sos coefs directly into handle:
    pp = IIR_Handles[1]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[1]->Nsos[pp] = Nsos2;               // set number of sos's
    for( i = 0; i < 5*Nsos2; i++ )
        *( IIR_Handles[1]->sos[pp] + i) = sos2[i];  // write coefs from const space
    IIR_Handles[1]->pingPong = pp;                  // update pingPong to point to new coefs

    // Load sos coefs directly into handle:
    pp = IIR_Handles[4]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[4]->Nsos[pp] = Nsos2;               // set number of sos's
    for( i = 0; i < 5*Nsos2; i++ )
        *( IIR_Handles[4]->sos[pp] + i) = sos2[i];  // write coefs from const space
    IIR_Handles[4]->pingPong = pp;                  // update pingPong to point to new coefs

    // Load sos coefs directly into handle:
    pp = IIR_Handles[6]->pingPong ^ 1;              // Get pingPong and toggle
    IIR_Handles[6]->Nsos[pp] = Nsos2;               // set number of sos's
    for( i = 0; i < 5*Nsos2; i++ )
        *( IIR_Handles[6]->sos[pp] + i) = sos2[i];  // write coefs from const space
    IIR_Handles[6]->pingPong = pp;                  // update pingPong to point to new coefs


    
    // Test FIR:

    // Load FIR coefs directly into handle:
    pp = FIR_Handles[0]->pingPong ^ 1;              // Get pingPong and toggle
    FIR_Handles[0]->FIR_Spec = COEFS;
    FIR_Handles[0]->N[pp] = N1;                     // set number of taps
    FIR_Handles[0]->symmetry = 0;
    for( i = 0; i < N1; i++ )
        *( FIR_Handles[0]->coefs[pp] + i) = coefs1[i];  // write coefs from const space
    FIR_Handles[0]->pingPong = pp;                  // update pingPong to point to new coefs



    return;
}


