add_executable (kernel main.c entry.asm)



#set_target_properties (kernel PROPERTIES LINK_FLAGS "-m32 -L/usr/lib32/ -lgcc -T /home/daniel/Projekte/Programmieren/Kernel/src/main/linkerscript.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -T /home/daniel/Projekte/Programmieren/Kernel/src/main/linkerscript.ld")
target_link_libraries (kernel kernel_bio kernel_boot kernel_gdt kernel_idt kernel_int kernel_pmm)
