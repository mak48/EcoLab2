/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IEcoLab1
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_ECOLAB1_H__
#define __I_ECOLAB1_H__

#include "IEcoBase1.h"

/* IEcoLab1 IID = {277FC00C-3562-4096-AFCF-C125B94EEC90} */
#ifndef __IID_IEcoLab1
static const UGUID IID_IEcoLab1 = {0x01, 0x10, {0x27, 0x7F, 0xC0, 0x0C, 0x35, 0x62, 0x40, 0x96, 0xAF, 0xCF, 0xC1, 0x25, 0xB9, 0x4E, 0xEC, 0x90}};
#endif /* __IID_IEcoLab1 */

/* IEcoVector IID = {A1B2C3D4-E5F6-7890-ABCD-EF1234567890} */
#ifndef __IID_IEcoVector
static const UGUID IID_IEcoVector = {0x01, 0x10, {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90}};
#endif /* __IID_IEcoVector */

/* IEcoMatrix IID = {B2C3D4E5-F607-8901-BCDE-F23456789012} */
#ifndef __IID_IEcoMatrix
static const UGUID IID_IEcoMatrix = {0x01, 0x10, {0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x07, 0x89, 0x01, 0xBC, 0xDE, 0xF2, 0x34, 0x56, 0x78, 0x90, 0x12}};
#endif /* __IID_IEcoMatrix */

/* IEcoLinearAlgebra IID = {C3D4E5F6-0789-0123-CDEF-345678901234} */
#ifndef __IID_IEcoLinearAlgebra
static const UGUID IID_IEcoLinearAlgebra = {0x01, 0x10, {0xC3, 0xD4, 0xE5, 0xF6, 0x07, 0x89, 0x01, 0x23, 0xCD, 0xEF, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34}};
#endif /* __IID_IEcoLinearAlgebra */

typedef struct IEcoLab1* IEcoLab1Ptr_t;
typedef struct IEcoVector* IEcoVectorPtr_t;
typedef struct IEcoMatrix* IEcoMatrixPtr_t;
typedef struct IEcoLinearAlgebra* IEcoLinearAlgebraPtr_t;

/* Интерфейс IEcoVector */
typedef struct IEcoVectorVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoVectorPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoVectorPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoVectorPtr_t me);

    /* IEcoVector */
    uint32_t (ECOCALLMETHOD *get_Size)(/* in */ IEcoVectorPtr_t me);
    double (ECOCALLMETHOD *get_Element)(/* in */ IEcoVectorPtr_t me, /* in */ uint32_t index);
    int16_t (ECOCALLMETHOD *set_Element)(/* in */ IEcoVectorPtr_t me, /* in */ uint32_t index, /* in */ double value);
    double* (ECOCALLMETHOD *get_Data)(/* in */ IEcoVectorPtr_t me);

} IEcoVectorVTbl, *IEcoVectorVTblPtr;

interface IEcoVector {
    struct IEcoVectorVTbl *pVTbl;
} IEcoVector;

/* Интерфейс IEcoMatrix */
typedef struct IEcoMatrixVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoMatrixPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoMatrixPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoMatrixPtr_t me);

    /* IEcoMatrix */
    uint32_t (ECOCALLMETHOD *get_Rows)(/* in */ IEcoMatrixPtr_t me);
    uint32_t (ECOCALLMETHOD *get_Cols)(/* in */ IEcoMatrixPtr_t me);
    double (ECOCALLMETHOD *get_Element)(/* in */ IEcoMatrixPtr_t me, /* in */ uint32_t row, /* in */ uint32_t col);
    int16_t (ECOCALLMETHOD *set_Element)(/* in */ IEcoMatrixPtr_t me, /* in */ uint32_t row, /* in */ uint32_t col, /* in */ double value);
    double* (ECOCALLMETHOD *get_Data)(/* in */ IEcoMatrixPtr_t me);

} IEcoMatrixVTbl, *IEcoMatrixVTblPtr;

interface IEcoMatrix {
    struct IEcoMatrixVTbl *pVTbl;
} IEcoMatrix;

/* Интерфейс IEcoLinearAlgebra */
typedef struct IEcoLinearAlgebraVTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLinearAlgebraPtr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLinearAlgebraPtr_t me);

    /* IEcoLinearAlgebra */
    IEcoVector* (ECOCALLMETHOD *new_Vector)(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ uint32_t size);
    IEcoMatrix* (ECOCALLMETHOD *new_Matrix)(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ uint32_t rows, /* in */ uint32_t cols);
    IEcoMatrix* (ECOCALLMETHOD *set_Diag)(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ IEcoVector* vector);
    IEcoVector* (ECOCALLMETHOD *get_Diag)(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ IEcoMatrix* matrix);

} IEcoLinearAlgebraVTbl, *IEcoLinearAlgebraVTblPtr;

interface IEcoLinearAlgebra {
    struct IEcoLinearAlgebraVTbl *pVTbl;
} IEcoLinearAlgebra;

typedef struct IEcoLab1VTbl {

    /* IEcoUnknown */
    int16_t (ECOCALLMETHOD *QueryInterface)(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ voidptr_t* ppv);
    uint32_t (ECOCALLMETHOD *AddRef)(/* in */ IEcoLab1Ptr_t me);
    uint32_t (ECOCALLMETHOD *Release)(/* in */ IEcoLab1Ptr_t me);

    /* IEcoLab1 */
    int16_t (ECOCALLMETHOD *MyFunction)(/* in */ IEcoLab1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** CopyName);

} IEcoLab1VTbl, *IEcoLab1VTblPtr;

interface IEcoLab1 {
    struct IEcoLab1VTbl *pVTbl;
} IEcoLab1;

#endif /* __I_ECOLAB1_H__ */