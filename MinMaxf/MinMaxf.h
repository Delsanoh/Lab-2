// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа MINMAXF_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции MINMAXF_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#pragma once

#ifdef MINMAXF_EXPORTS
#define MINMAXF_API __declspec(dllexport)
#else
#define MINMAXF_API __declspec(dllimport)
#endif

extern "C" MINMAXF_API void find_min_max(int* arr, unsigned int size, int* min_out, int* max_out);