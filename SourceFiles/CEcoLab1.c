#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"
/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "CEcoLab1.h"

/* ==================== Реализация CEcoVector ==================== */

static int16_t ECOCALLMETHOD CEcoVector_QueryInterface(/* in */ IEcoVectorPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoVector* pCMe = (CEcoVector*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoVector) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoVector;
        pCMe->m_pVTblIEcoVector->AddRef((IEcoVector*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoVector_AddRef(/* in */ IEcoVectorPtr_t me) {
    CEcoVector* pCMe = (CEcoVector*)me;
    if (me == 0 ) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoVector_Release(/* in */ IEcoVectorPtr_t me) {
    CEcoVector* pCMe = (CEcoVector*)me;
    if (me == 0 ) {
        return -1;
    }
    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoVector((IEcoVector*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoVector_get_Size(/* in */ IEcoVectorPtr_t me) {
    CEcoVector* pCMe = (CEcoVector*)me;
    return (me == 0) ? 0 : pCMe->m_size;
}

static double ECOCALLMETHOD CEcoVector_get_Element(/* in */ IEcoVectorPtr_t me, /* in */ uint32_t index) {
    CEcoVector* pCMe = (CEcoVector*)me;
    if (me == 0 || index >= pCMe->m_size) {
        return 0.0;
    }
    return pCMe->m_data[index];
}

static int16_t ECOCALLMETHOD CEcoVector_set_Element(/* in */ IEcoVectorPtr_t me, /* in */ uint32_t index, /* in */ double value) {
    CEcoVector* pCMe = (CEcoVector*)me;
    if (me == 0 || index >= pCMe->m_size) {
        return ERR_ECO_POINTER;
    }
    pCMe->m_data[index] = value;
    return ERR_ECO_SUCCESES;
}

static double* ECOCALLMETHOD CEcoVector_get_Data(/* in */ IEcoVectorPtr_t me) {
    CEcoVector* pCMe = (CEcoVector*)me;
    return (me == 0) ? 0 : pCMe->m_data;
}

/* Таблица функций IEcoVector */
IEcoVectorVTbl g_xA1B2C3D4E5F67890ABCDEF1234567890VTbl = {
    CEcoVector_QueryInterface,
    CEcoVector_AddRef,
    CEcoVector_Release,
    CEcoVector_get_Size,
    CEcoVector_get_Element,
    CEcoVector_set_Element,
    CEcoVector_get_Data
};

int16_t ECOCALLMETHOD initCEcoVector(/*in*/ IEcoVector* me, /* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t size) {
    CEcoVector* pCMe = (CEcoVector*)me;
    uint32_t i;

    if (me == 0 || pIMem == 0) {
        return ERR_ECO_POINTER;
    }
    
    pCMe->m_pIMem = pIMem;
    pCMe->m_size = size;
    pCMe->m_data = (double*)pIMem->pVTbl->Alloc(pIMem, size * sizeof(double));
    
    if (pCMe->m_data == 0) {
        return ERR_ECO_OUTOFMEMORY;
    }
    
    /* Инициализация нулями */
    for (i = 0; i < size; i++) {
        pCMe->m_data[i] = 0.0;
    }
    
    return ERR_ECO_SUCCESES;
}

int16_t ECOCALLMETHOD createCEcoVector(/* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t size, /* out */ IEcoVector** ppIEcoVector) {
    CEcoVector* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;

    if (ppIEcoVector == 0 || pIMem == 0) {
        return result;
    }
    
    pCMe = (CEcoVector*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoVector));
    if (pCMe == 0) {
        return ERR_ECO_OUTOFMEMORY;
    }
    
    pCMe->m_pIMem = pIMem;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblIEcoVector = &g_xA1B2C3D4E5F67890ABCDEF1234567890VTbl;
    pCMe->m_size = 0;
    pCMe->m_data = 0;
    
    result = initCEcoVector((IEcoVector*)pCMe, pIMem, size);
    if (result != 0) {
        pIMem->pVTbl->Free(pIMem, pCMe);
        return result;
    }
    
    *ppIEcoVector = (IEcoVector*)pCMe;
    return ERR_ECO_SUCCESES;
}

void ECOCALLMETHOD deleteCEcoVector(/* in */ IEcoVector* pIEcoVector) {
    CEcoVector* pCMe = (CEcoVector*)pIEcoVector;
    IEcoMemoryAllocator1* pIMem = 0;
    
    if (pIEcoVector != 0 ) {
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_data != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_data);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
    }
}

static int16_t ECOCALLMETHOD CEcoMatrix_QueryInterface(/* in */ IEcoMatrixPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoMatrix) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoMatrix;
        pCMe->m_pVTblIEcoMatrix->AddRef((IEcoMatrix*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoMatrix_AddRef(/* in */ IEcoMatrixPtr_t me) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    if (me == 0 ) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoMatrix_Release(/* in */ IEcoMatrixPtr_t me) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    if (me == 0 ) {
        return -1;
    }
    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoMatrix((IEcoMatrix*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoMatrix_get_Rows(/* in */ IEcoMatrixPtr_t me) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    return (me == 0) ? 0 : pCMe->m_rows;
}

static uint32_t ECOCALLMETHOD CEcoMatrix_get_Cols(/* in */ IEcoMatrixPtr_t me) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    return (me == 0) ? 0 : pCMe->m_cols;
}

static double ECOCALLMETHOD CEcoMatrix_get_Element(/* in */ IEcoMatrixPtr_t me, /* in */ uint32_t row, /* in */ uint32_t col) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    if (me == 0 || row >= pCMe->m_rows || col >= pCMe->m_cols) {
        return 0.0;
    }
    return pCMe->m_data[row * pCMe->m_cols + col];
}

static int16_t ECOCALLMETHOD CEcoMatrix_set_Element(/* in */ IEcoMatrixPtr_t me, /* in */ uint32_t row, /* in */ uint32_t col, /* in */ double value) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    if (me == 0 || row >= pCMe->m_rows || col >= pCMe->m_cols) {
        return ERR_ECO_POINTER;
    }
    pCMe->m_data[row * pCMe->m_cols + col] = value;
    return ERR_ECO_SUCCESES;
}

static double* ECOCALLMETHOD CEcoMatrix_get_Data(/* in */ IEcoMatrixPtr_t me) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    return (me == 0) ? 0 : pCMe->m_data;
}

IEcoMatrixVTbl g_xB2C3D4E5F6078901BCDEF23456789012VTbl = {
    CEcoMatrix_QueryInterface,
    CEcoMatrix_AddRef,
    CEcoMatrix_Release,
    CEcoMatrix_get_Rows,
    CEcoMatrix_get_Cols,
    CEcoMatrix_get_Element,
    CEcoMatrix_set_Element,
    CEcoMatrix_get_Data
};

int16_t ECOCALLMETHOD initCEcoMatrix(/*in*/ IEcoMatrix* me, /* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t rows, /* in */ uint32_t cols) {
    CEcoMatrix* pCMe = (CEcoMatrix*)me;
    uint32_t i;

    if (me == 0 || pIMem == 0) {
        return ERR_ECO_POINTER;
    }
    
    pCMe->m_pIMem = pIMem;
    pCMe->m_rows = rows;
    pCMe->m_cols = cols;
    pCMe->m_data = (double*)pIMem->pVTbl->Alloc(pIMem, rows * cols * sizeof(double));
    
    if (pCMe->m_data == 0) {
        return ERR_ECO_OUTOFMEMORY;
    }
    
    for (i = 0; i < rows * cols; i++) {
        pCMe->m_data[i] = 0.0;
    }
    
    return ERR_ECO_SUCCESES;
}

int16_t ECOCALLMETHOD createCEcoMatrix(/* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t rows, /* in */ uint32_t cols, /* out */ IEcoMatrix** ppIEcoMatrix) {
    CEcoMatrix* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;

    if (ppIEcoMatrix == 0 || pIMem == 0) {
        return result;
    }
    
    pCMe = (CEcoMatrix*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoMatrix));
    if (pCMe == 0) {
        return ERR_ECO_OUTOFMEMORY;
    }
    
    pCMe->m_pIMem = pIMem;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblIEcoMatrix = &g_xB2C3D4E5F6078901BCDEF23456789012VTbl;
    pCMe->m_rows = 0;
    pCMe->m_cols = 0;
    pCMe->m_data = 0;
    
    result = initCEcoMatrix((IEcoMatrix*)pCMe, pIMem, rows, cols);
    if (result != 0) {
        pIMem->pVTbl->Free(pIMem, pCMe);
        return result;
    }
    
    *ppIEcoMatrix = (IEcoMatrix*)pCMe;
    return ERR_ECO_SUCCESES;
}

void ECOCALLMETHOD deleteCEcoMatrix(/* in */ IEcoMatrix* pIEcoMatrix) {
    CEcoMatrix* pCMe = (CEcoMatrix*)pIEcoMatrix;
    IEcoMemoryAllocator1* pIMem = 0;
    
    if (pIEcoMatrix != 0 ) {
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_data != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_data);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
    }
}


static int16_t ECOCALLMETHOD CEcoLinearAlgebra_QueryInterface(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoLinearAlgebra) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLinearAlgebra;
        pCMe->m_pVTblIEcoLinearAlgebra->AddRef((IEcoLinearAlgebra*)pCMe);
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoLinearAlgebra_AddRef(/* in */ IEcoLinearAlgebraPtr_t me) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    if (me == 0 ) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLinearAlgebra_Release(/* in */ IEcoLinearAlgebraPtr_t me) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    if (me == 0 ) {
        return -1;
    }
    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLinearAlgebra((IEcoLinearAlgebra*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static IEcoVector* ECOCALLMETHOD CEcoLinearAlgebra_new_Vector(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ uint32_t size) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    IEcoVector* pVector = 0;
    
    if (me == 0) {
        return 0;
    }
    
    if (createCEcoVector(pCMe->m_pIMem, size, &pVector) == 0) {
        return pVector;
    }
    
    return 0;
}

static IEcoMatrix* ECOCALLMETHOD CEcoLinearAlgebra_new_Matrix(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ uint32_t rows, /* in */ uint32_t cols) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    IEcoMatrix* pMatrix = 0;
    
    if (me == 0) {
        return 0;
    }
    
    if (createCEcoMatrix(pCMe->m_pIMem, rows, cols, &pMatrix) == 0) {
        return pMatrix;
    }
    
    return 0;
}

static IEcoMatrix* ECOCALLMETHOD CEcoLinearAlgebra_set_Diag(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ IEcoVector* vector) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    uint32_t size = 0;
    IEcoMatrix* pMatrix = 0;
    uint32_t i = 0;
    
    if (me == 0 || vector == 0) {
        return 0;
    }
    
    size = vector->pVTbl->get_Size(vector);
    pMatrix = pCMe->m_pVTblIEcoLinearAlgebra->new_Matrix(me, size, size);
    
    if (pMatrix == 0) {
        return 0;
    }
    
    /* Заполнение диагонали */
    for (i = 0; i < size; i++) {
        double value = vector->pVTbl->get_Element(vector, i);
        pMatrix->pVTbl->set_Element(pMatrix, i, i, value);
    }
    
    return pMatrix;
}

static IEcoVector* ECOCALLMETHOD CEcoLinearAlgebra_get_Diag(/* in */ IEcoLinearAlgebraPtr_t me, /* in */ IEcoMatrix* matrix) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    uint32_t rows = 0;
    uint32_t cols = 0;
    uint32_t min_dim = 0;
    IEcoVector* pVector = 0;
    uint32_t i = 0;
    
    if (me == 0 || matrix == 0) {
        return 0;
    }
    
    rows = matrix->pVTbl->get_Rows(matrix);
    cols = matrix->pVTbl->get_Cols(matrix);
    min_dim = (rows < cols) ? rows : cols;
    
    pVector = pCMe->m_pVTblIEcoLinearAlgebra->new_Vector(me, min_dim);
    
    if (pVector == 0) {
        return 0;
    }
    
    /* Извлечение диагонали */
    for (i = 0; i < min_dim; i++) {
        double value = matrix->pVTbl->get_Element(matrix, i, i);
        pVector->pVTbl->set_Element(pVector, i, value);
    }
    
    return pVector;
}

/* Таблица функций IEcoLinearAlgebra */
IEcoLinearAlgebraVTbl g_xC3D4E5F607890123CDEF345678901234VTbl = {
    CEcoLinearAlgebra_QueryInterface,
    CEcoLinearAlgebra_AddRef,
    CEcoLinearAlgebra_Release,
    CEcoLinearAlgebra_new_Vector,
    CEcoLinearAlgebra_new_Matrix,
    CEcoLinearAlgebra_set_Diag,
    CEcoLinearAlgebra_get_Diag
};

int16_t ECOCALLMETHOD initCEcoLinearAlgebra(/*in*/ IEcoLinearAlgebra* me, /* in */ IEcoSystem1* pISys) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    if (me == 0 || pISys == 0) {
        return result;
    }

    pCMe->m_pISys = pISys;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0) {
        return result;
    }

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);

    pIBus->pVTbl->Release(pIBus);

    return result;
}

int16_t ECOCALLMETHOD createCEcoLinearAlgebra(/* in */ IEcoSystem1* pISys, /* out */ IEcoLinearAlgebra** ppIEcoLinearAlgebra) {
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLinearAlgebra* pCMe = 0;
    int16_t result = ERR_ECO_POINTER;

    if (ppIEcoLinearAlgebra == 0 || pISys == 0) {
        return result;
    }
    
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0) {
        return result;
    }
    
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0) {
        pIBus->pVTbl->Release(pIBus);
        return result;
    }
    
    pCMe = (CEcoLinearAlgebra*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLinearAlgebra));
    if (pCMe == 0) {
        pIBus->pVTbl->Release(pIBus);
        pIMem->pVTbl->Release(pIMem);
        return ERR_ECO_OUTOFMEMORY;
    }
    
    pCMe->m_pIMem = pIMem;
    pCMe->m_pISys = pISys;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblIEcoLinearAlgebra = &g_xC3D4E5F607890123CDEF345678901234VTbl;
    
    result = initCEcoLinearAlgebra((IEcoLinearAlgebra*)pCMe, pISys);
    if (result != 0) {
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIBus->pVTbl->Release(pIBus);
        pIMem->pVTbl->Release(pIMem);
        return result;
    }
    
    *ppIEcoLinearAlgebra = (IEcoLinearAlgebra*)pCMe;
    pIBus->pVTbl->Release(pIBus);
    
    return ERR_ECO_SUCCESES;
}

void ECOCALLMETHOD deleteCEcoLinearAlgebra(/* in */ IEcoLinearAlgebra* pIEcoLinearAlgebra) {
    CEcoLinearAlgebra* pCMe = (CEcoLinearAlgebra*)pIEcoLinearAlgebra;
    IEcoMemoryAllocator1* pIMem = 0;
    
    if (pIEcoLinearAlgebra != 0 ) {
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}

/* ==================== Реализация CEcoLab1 ==================== */
static int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ IEcoLab1Ptr_t me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*)me;

    if (me == 0 || ppv == 0) {
        return ERR_ECO_POINTER;
    }

    if ( IsEqualUGUID(riid, &IID_IEcoLab1) || IsEqualUGUID(riid, &IID_IEcoUnknown) ) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*)pCMe);
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoLinearAlgebra) ) {
        if (pCMe->m_pILinearAlgebra == 0) {
            createCEcoLinearAlgebra(pCMe->m_pISys, &pCMe->m_pILinearAlgebra);
        }
        if (pCMe->m_pILinearAlgebra != 0) {
            *ppv = pCMe->m_pILinearAlgebra;
            pCMe->m_pILinearAlgebra->pVTbl->AddRef(pCMe->m_pILinearAlgebra);
        } else {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorX) ) {
        if (pCMe->m_pIX == 0) {
            /* Попытка получить интерфейс через агрегирование */
            IEcoInterfaceBus1* pIBus = 0;
            int16_t result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
            if (result == 0 && pIBus != 0) {
                /* Пробуем получить компонент E (агрегирует B) */
                result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
                if (result == 0 && pCMe->m_pIY != 0) {
                    result = pCMe->m_pIY->pVTbl->QueryInterface(pCMe->m_pIY, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
                }
                else {
                    /* Пробуем получить компонент D (включает A) */
                    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
                    if (result == 0 && pCMe->m_pIY != 0) {
                        result = pCMe->m_pIY->pVTbl->QueryInterface(pCMe->m_pIY, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
                    }
                    else {
                        /* Пробуем компонент B напрямую */
                        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
                        if (result != 0 || pCMe->m_pIX == 0) {
                            /* Пробуем компонент A напрямую */
                            result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIX);
                        }
                    }
                }
                pIBus->pVTbl->Release(pIBus);
            }
        }
        if (pCMe->m_pIX != 0) {
            *ppv = pCMe->m_pIX;
            pCMe->m_pIX->pVTbl->AddRef(pCMe->m_pIX);
        } else {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
    }
    else if ( IsEqualUGUID(riid, &IID_IEcoCalculatorY) ) {
        if (pCMe->m_pIY == 0) {
            /* Попытка получить интерфейс через агрегирование */
            IEcoInterfaceBus1* pIBus = 0;
            int16_t result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
            if (result == 0 && pIBus != 0) {
                /* Пробуем получить компонент E (агрегирует B) */
                result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
                if (result != 0 || pCMe->m_pIY == 0) {
                    /* Пробуем получить компонент D (включает A) */
                    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, pCMe->m_pIUnkOuter, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIY);
                }
                pIBus->pVTbl->Release(pIBus);
            }
        }
        if (pCMe->m_pIY != 0) {
            *ppv = pCMe->m_pIY;
            pCMe->m_pIY->pVTbl->AddRef(pCMe->m_pIY);
        } else {
            *ppv = 0;
            return ERR_ECO_NOINTERFACE;
        }
    }
    else {
        *ppv = 0;
        return ERR_ECO_NOINTERFACE;
    }
    return ERR_ECO_SUCCESES;
}

static uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (me == 0 ) {
        return -1;
    }
    return ++pCMe->m_cRef;
}

static uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ IEcoLab1Ptr_t me) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    if (me == 0 ) {
        return -1;
    }
    --pCMe->m_cRef;
    if ( pCMe->m_cRef == 0 ) {
        deleteCEcoLab1((IEcoLab1*)pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

static int16_t ECOCALLMETHOD CEcoLab1_MyFunction(/* in */ IEcoLab1Ptr_t me, /* in */ char_t* Name, /* out */ char_t** copyName) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    int16_t index = 0;

    if (me == 0 || Name == 0 || copyName == 0) {
        return ERR_ECO_POINTER;
    }

    while(Name[index] != 0) {
        index++;
    }
    pCMe->m_Name = (char_t*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, index + 1);
    index = 0;
    while(Name[index] != 0) {
        pCMe->m_Name[index] = Name[index];
        index++;
    }
    *copyName = pCMe->m_Name;

    return ERR_ECO_SUCCESES;
}

int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ IEcoLab1Ptr_t me, /* in */ struct IEcoUnknown *pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    IEcoInterfaceBus1* pIBus = 0;
    int16_t result = -1;

    if (me == 0 ) {
        return result;
    }

    pCMe->m_pISys = (IEcoSystem1*)pIUnkSystem;

    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0) {
        return result;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pCMe->m_pIMem);

    pIBus->pVTbl->Release(pIBus);

    return result;
}

IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_MyFunction
};


int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*)&CID_EcoMemoryManager1;

    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    if (pIUnkOuter != 0) {
        result = pIUnkOuter->pVTbl->QueryInterface(pIUnkOuter, &IID_IEcoLab1, (void**)ppIEcoLab1);
        if (result == 0) {
            return result;
        }
    }

    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem, (void **)&pISys);
    if (result != 0 && pISys == 0) {
        return result;
    }

    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0) {
        pISys->pVTbl->Release(pISys);
        return result;
    }

    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**)&pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*)pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0 && pIMem == 0) {
        pISys->pVTbl->Release(pISys);
        pIBus->pVTbl->Release(pIBus);
        return result;
    }

    pCMe = (CEcoLab1*)pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));
    if (pCMe == 0) {
        pISys->pVTbl->Release(pISys);
        pIBus->pVTbl->Release(pIBus);
        pIMem->pVTbl->Release(pIMem);
        return ERR_ECO_OUTOFMEMORY;
    }

    pCMe->m_pISys = pISys;
    pCMe->m_pIMem = pIMem;
    pCMe->m_cRef = 1;
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;
    pCMe->m_Name = 0;
    pCMe->m_pILinearAlgebra = 0;
    pCMe->m_pIX = 0;
    pCMe->m_pIY = 0;
    pCMe->m_pIUnkOuter = pIUnkOuter;

    *ppIEcoLab1 = (IEcoLab1*)pCMe;

    pIBus->pVTbl->Release(pIBus);

    return 0;
}

void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*)pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0 ) {
        pIMem = pCMe->m_pIMem;
        if ( pCMe->m_Name != 0 ) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if ( pCMe->m_pILinearAlgebra != 0 ) {
            pCMe->m_pILinearAlgebra->pVTbl->Release(pCMe->m_pILinearAlgebra);
        }
        if ( pCMe->m_pIX != 0 ) {
            pCMe->m_pIX->pVTbl->Release(pCMe->m_pIX);
        }
        if ( pCMe->m_pIY != 0 ) {
            pCMe->m_pIY->pVTbl->Release(pCMe->m_pIY);
        }
        if ( pCMe->m_pISys != 0 ) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}