add_definitions (-O3)

add_library(kernel_pmm STATIC pmm_functions.c pmm_init.c)
