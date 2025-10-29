#ifndef __C_ECOLAB1_H__
#define __C_ECOLAB1_H__

#include "IEcoLab1.h"
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/* Структура вектора */
typedef struct CEcoVector {
    /* Таблица функций интерфейса IEcoVector */
    IEcoVectorVTbl* m_pVTblIEcoVector;
    /* Счетчик ссылок */
    uint32_t m_cRef;
    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;
    /* Данные вектора */
    uint32_t m_size;
    double* m_data;
} CEcoVector, *CEcoVectorPtr;

/* Структура матрицы */
typedef struct CEcoMatrix {
    /* Таблица функций интерфейса IEcoMatrix */
    IEcoMatrixVTbl* m_pVTblIEcoMatrix;
    /* Счетчик ссылок */
    uint32_t m_cRef;
    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;
    /* Данные матрицы */
    uint32_t m_rows;
    uint32_t m_cols;
    double* m_data;
} CEcoMatrix, *CEcoMatrixPtr;

/* Структура линейной алгебры */
typedef struct CEcoLinearAlgebra {
    /* Таблица функций интерфейса IEcoLinearAlgebra */
    IEcoLinearAlgebraVTbl* m_pVTblIEcoLinearAlgebra;
    /* Счетчик ссылок */
    uint32_t m_cRef;
    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;
    /* Системный интерфейс */
    IEcoSystem1* m_pISys;
} CEcoLinearAlgebra, *CEcoLinearAlgebraPtr;

typedef struct CEcoLab1 {
    /* Таблица функций интерфейса IEcoLab1 */
    IEcoLab1VTbl* m_pVTblIEcoLab1;
    /* Счетчик ссылок */
    uint32_t m_cRef;
    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;
    /* Системный интерфейс */
    IEcoSystem1* m_pISys;
    /* Данные экземпляра */
    char_t* m_Name;
    
    /* Агрегированные интерфейсы */
    IEcoLinearAlgebra* m_pILinearAlgebra;
    IEcoCalculatorX* m_pIX;
    IEcoCalculatorY* m_pIY;
    
    /* Внешний агрегатор (если есть) */
    IEcoUnknown* m_pIUnkOuter;

} CEcoLab1, *CEcoLab1Ptr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ IEcoUnknown *pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1);
/* Удаление */
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1);

/* Функции для вектора */
int16_t ECOCALLMETHOD initCEcoVector(/*in*/ IEcoVector* me, /* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t size);
int16_t ECOCALLMETHOD createCEcoVector(/* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t size, /* out */ IEcoVector** ppIEcoVector);
void ECOCALLMETHOD deleteCEcoVector(/* in */ IEcoVector* pIEcoVector);

/* Функции для матрицы */
int16_t ECOCALLMETHOD initCEcoMatrix(/*in*/ IEcoMatrix* me, /* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t rows, /* in */ uint32_t cols);
int16_t ECOCALLMETHOD createCEcoMatrix(/* in */ IEcoMemoryAllocator1* pIMem, /* in */ uint32_t rows, /* in */ uint32_t cols, /* out */ IEcoMatrix** ppIEcoMatrix);
void ECOCALLMETHOD deleteCEcoMatrix(/* in */ IEcoMatrix* pIEcoMatrix);

/* Функции для линейной алгебры */
int16_t ECOCALLMETHOD initCEcoLinearAlgebra(/*in*/ IEcoLinearAlgebra* me, /* in */ IEcoSystem1* pISys);
int16_t ECOCALLMETHOD createCEcoLinearAlgebra(/* in */ IEcoSystem1* pISys, /* out */ IEcoLinearAlgebra** ppIEcoLinearAlgebra);
void ECOCALLMETHOD deleteCEcoLinearAlgebra(/* in */ IEcoLinearAlgebra* pIEcoLinearAlgebra);

#endif /* __C_ECOLAB1_H__ */