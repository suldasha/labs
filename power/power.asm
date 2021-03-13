global power

section .text
power:
    ;пролог функции
    push ebp
    mov ebp, esp
    sub esp, 8

    push ebx
    push esi
    push edi

    mov eax, [ebp+8] ;основание степени
    mov ebx, [ebp+12] ;показатель степени
    mov esi, 1
    mov edi, 1

    mov ecx, eax
    mov edx, eax

    ;Алгоритм работы: сумма накапливается пока внутренний счетчик не будет
    ;равен значению основания степени, после этого значение внешнего счетчика 
    ;увеличивается, внутренний цикл повторяется. Результат получен, когда
    ;внешний счетчик станет равен показателю степени.
    _power_sum:
        mov edi, 1

        ;edx накапливает сумму
        ;esi накапливает степень
        ;edi накапливает накапливает кол-во итераций по основанию степени
        .sum_factors:
            add edx, ecx
            jo .overflow
            inc edi
            cmp edi, eax
            jl .sum_factors

        mov ecx, edx
        inc esi
        cmp esi, ebx
        jl _power_sum

        push edx
        call [ebp+16] ;адрес возврата 

        .end:
            ;эпилог функции
            pop edi
            pop esi
            pop ebx

            mov esp, ebp
            pop ebp
            ret

        .overflow:
            ;в случае переполнения возвращается код ошибки 1
            xor edx, edx
            mov edx, 1
            push edx
            call [ebp+16]
            jmp .end
