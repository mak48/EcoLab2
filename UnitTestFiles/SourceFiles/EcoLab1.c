#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdio.h>
#include <windows.h>
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
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

int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    
    IEcoLab1* pIEcoLab1 = 0;
    IEcoLinearAlgebra* pILinearAlgebra = 0;
    struct IEcoCalculatorX* pIX = 0;
    struct IEcoCalculatorY* pIY = 0;
    
    IEcoVector* pVector = 0;
    IEcoMatrix* pMatrix = 0;
    IEcoVector* pDiagVector = 0;
    
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double time_taken = 0.0;
    int32_t calc_result = 0;
    uint32_t i = 0;
    uint32_t j = 0;

    /* Дополнительные переменные для теста 4 */
	struct IEcoCalculatorX* pIX2 = 0;
    struct IEcoLab1* pIEcoLab1_2 = 0;

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
    /* Регистрация ВСЕХ компонентов калькулятора */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorA, (IEcoUnknown*)GetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E);
    if (result != 0 ) {
        printf("Failed to register CalculatorA\n");
    }
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorB, (IEcoUnknown*)GetIEcoComponentFactoryPtr_AE202E543CE54550899603BD70C62565);
    if (result != 0 ) {
        printf("Failed to register CalculatorB\n");
    }
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorD, (IEcoUnknown*)GetIEcoComponentFactoryPtr_3A8E44677E82475CB4A3719ED8397E61);
    if (result != 0 ) {
        printf("Failed to register CalculatorD\n");
    }
    
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoCalculatorE, (IEcoUnknown*)GetIEcoComponentFactoryPtr_872FEF1DE3314B87AD44D1E7C232C2F0);
    if (result != 0 ) {
        printf("Failed to register CalculatorE\n");
    }

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

    /* Тест 1: Получение IEcoLinearAlgebra через QueryInterface */
    printf("Test 1: IEcoLinearAlgebra through QueryInterface\n");
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoLinearAlgebra, (void**)&pILinearAlgebra);
    if (result == 0 && pILinearAlgebra != 0) {
        printf("Successfull: IEcoLinearAlgebra\n");
        
        /* Демонстрация работы с линейной алгеброй */
        pVector = pILinearAlgebra->pVTbl->new_Vector(pILinearAlgebra, 3);
        pVector->pVTbl->set_Element(pVector, 0, 1.0);
        pVector->pVTbl->set_Element(pVector, 1, 2.0);
        pVector->pVTbl->set_Element(pVector, 2, 3.0);
        pMatrix = pILinearAlgebra->pVTbl->set_Diag(pILinearAlgebra, pVector);
        
        print_vector(pVector, "Vector");
        print_matrix(pMatrix, "Diag matrix");
        
        pILinearAlgebra->pVTbl->Release(pILinearAlgebra);
        pILinearAlgebra = 0;
    } else {
        printf("Didnt receive IEcoLinearAlgebra\n");
    }
    printf("\n");

    /* Тест 2: Получение IEcoCalculatorX через QueryInterface */
    printf("Test 2: IEcoCalculatorX through QueryInterface\n");

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("Successfull: IEcoCalculatorX\n");
        
        /* Демонстрация работы с калькулятором */
        calc_result = pIX->pVTbl->Addition(pIX, 3, 5);
        printf("3 + 5 = %d\n", calc_result);
        
        calc_result = pIX->pVTbl->Subtraction(pIX, 10, 4);
        printf("10 - 4 = %d\n", calc_result);
        
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    } else {
        printf("Didnt receive IEcoCalculatorX\n");
    }
    printf("\n");

    /* Тест 3: Получение IEcoCalculatorY через QueryInterface */
    printf("Test 3: IEcoCalculatorY through QueryInterface\n");
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY);
    if (result == 0 && pIY != 0) {
        printf("Successfull: IEcoCalculatorY\n");
        
        /* Демонстрация работы с калькулятором */
        calc_result = pIY->pVTbl->Multiplication(pIY, 3, 5);
        printf("3 * 5 = %d\n", calc_result);
        
        calc_result = pIY->pVTbl->Division(pIY, 15, 3);
        printf("15 / 3 = %d\n", calc_result);
        
        pIY->pVTbl->Release(pIY);
        pIY = 0;
    } else {
        printf("Didnt receive IEcoCalculatorY\n");
    }
    printf("\n");

    /* Тест 4: Демонстрация свойств интерфейсов */
    printf("Test 4\n");
    
    /* Получаем IX из Lab1 */
    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX);
    if (result == 0 && pIX != 0) {
        printf("1. Successfull: IX from IEcoLab1\n");
        
        /* Из IX получить IY */
        result = pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pIY);
        if (result == 0 && pIY != 0) {
            printf("2. Successfull: IY from IX\n");
            
            /* Из IY получить обратно IX */
            result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pIX2);
            if (result == 0 && pIX2 != 0) {
                printf("3. Successfull: IX from IY\n");
                
                /* Проверяем */
                calc_result = pIX2->pVTbl->Addition(pIX2, 7, 3);
                printf("7 + 3 = %d (from pIX2)\n", calc_result);
                
                pIX2->pVTbl->Release(pIX2);
            }
            
            /* Из IY получить IEcoLab1 */
            result = pIY->pVTbl->QueryInterface(pIY, &IID_IEcoLab1, (void**)&pIEcoLab1_2);
            if (result == 0 && pIEcoLab1_2 != 0) {
                printf("4. Successfull: IEcoLab1 from IY\n");
                pIEcoLab1_2->pVTbl->Release(pIEcoLab1_2);
            }
            
            pIY->pVTbl->Release(pIY);
            pIY = 0;
        }
        
        pIX->pVTbl->Release(pIX);
        pIX = 0;
    }
    printf("\n");

    result = pIEcoLab1->pVTbl->MyFunction(pIEcoLab1, name, &copyName);

    /* Освобождение блока памяти */
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
    if (pILinearAlgebra != 0) {
        pILinearAlgebra->pVTbl->Release(pILinearAlgebra);
    }
    if (pIX != 0) {
        pIX->pVTbl->Release(pIX);
    }
    if (pIY != 0) {
        pIY->pVTbl->Release(pIY);
    }
    if (pIX2 != 0) {
        pIX2->pVTbl->Release(pIX2);
    }
    if (pIEcoLab1_2 != 0) {
        pIEcoLab1_2->pVTbl->Release(pIEcoLab1_2);
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