add_definitions (-fno-builtin -fno-leading-underscore -fno-strict-aliasing -fno-stack-protector -Wall -m32)




add_subdirectory (bio)
add_subdirectory (boot)
add_subdirectory (gdt)
add_subdirectory (idt)
add_subdirectory (int)
add_subdirectory (main)
#add_subdirectory (paging)
add_subdirectory (phys_mm)
