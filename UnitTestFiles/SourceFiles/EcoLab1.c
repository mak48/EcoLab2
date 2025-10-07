/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdio.h>
#include <windows.h>

/* Функции для вывода матриц и векторов */
void print_vector(IEcoVector* pVector, const char* name) {
    uint32_t i = 0;
    uint32_t size = 0;
    
    if (pVector != 0) {
        size = pVector->pVTbl->get_Size(pVector);
        printf("%s = [", name);
        for (i = 0; i < size; i++) {
            printf("%.2f", pVector->pVTbl->get_Element(pVector, i));
            if (i < size - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

void print_matrix(IEcoMatrix* pMatrix, const char* name) {
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t rows = 0;
    uint32_t cols = 0;
    
    if (pMatrix != 0) {
        rows = pMatrix->pVTbl->get_Rows(pMatrix);
        cols = pMatrix->pVTbl->get_Cols(pMatrix);
        printf("%s = \n", name);
        for (i = 0; i < rows; i++) {
            printf("    [");
            for (j = 0; j < cols; j++) {
                printf("%8.2f", pMatrix->pVTbl->get_Element(pMatrix, i, j));
            }
            printf("]\n");
        }
    }
}

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    IEcoLab1* pIEcoLab1 = 0;
    IEcoLinearAlgebra* pILinearAlgebra = 0;
    IEcoVector* pVector = 0;
    IEcoMatrix* pMatrix = 0;
    IEcoVector* pDiagVector = 0;
    
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double time_taken = 0.0;
    double manual_time = 0.0;
    double diag_time = 0.0;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t k = 0;
    uint32_t large_size = 0;
    uint32_t test_size = 0;
    IEcoMatrix* pRectMatrix = 0;
    IEcoVector* pRectDiag = 0;
    IEcoVector* pLargeVector = 0;
    IEcoMatrix* pLargeMatrix = 0;
    IEcoMatrix* pManualMatrix = 0;
    IEcoMatrix* pDiagMatrix = 0;
    IEcoVector* pTestVector = 0;

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        goto Release;
    }
#endif

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0 || pIMem == 0) {
        goto Release;
    }

    /* Выделение блока памяти */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        goto Release;
    }

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoLinearAlgebra, (void**)&pILinearAlgebra);
    if (result != 0 || pILinearAlgebra == 0) {
        goto Release;
    }
    QueryPerformanceFrequency(&frequency);

    /* Тест 1: создание матрицы с заданными диагональными элементами (diag)*/
    printf("Test 1\n");
    QueryPerformanceCounter(&start);

    pVector = pILinearAlgebra->pVTbl->new_Vector(pILinearAlgebra, 3);
    pVector->pVTbl->set_Element(pVector, 0, 1.0);
    pVector->pVTbl->set_Element(pVector, 1, 2.0);
    pVector->pVTbl->set_Element(pVector, 2, 3.0);
    pMatrix = pILinearAlgebra->pVTbl->set_Diag(pILinearAlgebra, pVector);

    QueryPerformanceCounter(&end);
    time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    print_vector(pVector, "Input vector");
    print_matrix(pMatrix, "Matrix");
    printf("Time: %.6f sec\n\n", time_taken);

    /* Тест 2: получение диагонали из матрицы */
    printf("Test 2\n");
    QueryPerformanceCounter(&start);

    pDiagVector = pILinearAlgebra->pVTbl->get_Diag(pILinearAlgebra, pMatrix);

    QueryPerformanceCounter(&end);
    time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    print_matrix(pMatrix, "Matrix");
    print_vector(pDiagVector, "Vector");
    printf("Time: %.6f sec\n\n", time_taken);

    /* Тест 3: неквадратная матрица */
    printf("Test 3\n");
    pRectMatrix = pILinearAlgebra->pVTbl->new_Matrix(pILinearAlgebra, 2, 3);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            pRectMatrix->pVTbl->set_Element(pRectMatrix, i, j, (double)(i * 3 + j + 1));
        }
    }

    QueryPerformanceCounter(&start);
    pRectDiag = pILinearAlgebra->pVTbl->get_Diag(pILinearAlgebra, pRectMatrix);
    QueryPerformanceCounter(&end);
    time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    print_matrix(pRectMatrix, "Matrix");
    print_vector(pRectDiag, "Vector");
    printf("Time: %.6f sec\n\n", time_taken);

    /* Тест 4: матрицы с размером от 100х100 до 1000х1000 */
    printf("Test 4\n");
    printf("Size    | Time (seconds)\n");
    printf("--------|----------------\n");
    
    for (test_size = 100; test_size <= 1000; test_size += 100) {
        pLargeVector = pILinearAlgebra->pVTbl->new_Vector(pILinearAlgebra, test_size);
        for (i = 0; i < test_size; i++) {
            pLargeVector->pVTbl->set_Element(pLargeVector, i, (double)(i + 1));
        }

        QueryPerformanceCounter(&start);
        pLargeMatrix = pILinearAlgebra->pVTbl->set_Diag(pILinearAlgebra, pLargeVector);
        QueryPerformanceCounter(&end);
        time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

        printf("%dx%-5d | %.7f\n", test_size, test_size, time_taken);

        if (pLargeVector != 0) {
            pLargeVector->pVTbl->Release(pLargeVector);
            pLargeVector = 0;
        }
        if (pLargeMatrix != 0) {
            pLargeMatrix->pVTbl->Release(pLargeMatrix);
            pLargeMatrix = 0;
        }
    }
    printf("\n");
    result = pIEcoLab1->pVTbl->MyFunction(pIEcoLab1, name, &copyName);

    /* Освлбождение блока памяти */
    pIMem->pVTbl->Free(pIMem, name);

Release:
    /* Освобождение созданных объектов */
    if (pVector != 0) {
        pVector->pVTbl->Release(pVector);
    }
    if (pMatrix != 0) {
        pMatrix->pVTbl->Release(pMatrix);
    }
    if (pDiagVector != 0) {
        pDiagVector->pVTbl->Release(pDiagVector);
    }
    if (pRectMatrix != 0) {
        pRectMatrix->pVTbl->Release(pRectMatrix);
    }
    if (pRectDiag != 0) {
        pRectDiag->pVTbl->Release(pRectDiag);
    }
    if (pLargeVector != 0) {
        pLargeVector->pVTbl->Release(pLargeVector);
    }
    if (pLargeMatrix != 0) {
        pLargeMatrix->pVTbl->Release(pLargeMatrix);
    }
    if (pManualMatrix != 0) {
        pManualMatrix->pVTbl->Release(pManualMatrix);
    }
    if (pDiagMatrix != 0) {
        pDiagMatrix->pVTbl->Release(pDiagMatrix);
    }
    if (pTestVector != 0) {
        pTestVector->pVTbl->Release(pTestVector);
    }
    if (pILinearAlgebra != 0) {
        pILinearAlgebra->pVTbl->Release(pILinearAlgebra);
    }

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}