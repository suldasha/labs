SECTION .data
msgOF db 'Переполнение. Введите число меньше.', 0xa
msgOF_len equ ($ - msgOF)
msgIN db 'Неправильный формат. Введите натуральное число.', 0xa
msgIN_len equ ($ - msgIN)

case_1 db '1', 0xa
case_0 db '0', 0xa
star db '*'

len equ 255
case_len equ 2

SECTION .bss
number resb len
digit resb 1

SECTION .text
global _start

_start:
    mov eax, 3 ;ввод числа
    mov ebx, 0
    mov ecx, number
    mov edx, len
    int 80h

    dec eax ;отбрасываем '\0'
    push eax ;сохраняем длину перед входом в ф-ю
    call Correct 
    pop eax
    mov ecx, eax ;пеередаем длину строки в счетчик есх
    xor eax, eax
    mov ebp, 10 ;для работы с каждой отдельной цифрой числа используем 10
    call Str_to_num
    mov ebx, 2 ;для начала факторизации берем самый маленький множитель
    call Factorize

    mov eax, 4 ;завершаем вывод единицой
    mov ebx, 1
    mov ecx, case_1
    mov edx, case_len
    int 80h

    mov eax, 1
    mov ebx, 0
    int 80h

; проверяет правильность введенного числа (натуральное)
; использует длину строки в еах и строку в number
; завершает проверку при достижении конца строки или обнаружении ошибки
Correct:
    .Digit:
        ;проверяем посимвольно, обращаемся к элементам по индексу eax - 1
        movzx edx, byte [number + eax - 1] 
        cmp edx, '0' ;если не цифра - выводим сообщение об ошибке
        jb Err_input
        cmp edx, '9'
        ja Err_input

        dec eax
        cmp eax, 0
        jne .Digit
    ret

;превращает строку number длины есх в число в еах
Str_to_num:
    .Digit:
        xor ebx, ebx
        mov bl, byte [number + esi] ;берем цифры по индексу esi
        mul ebp ;добавляем разряд
        jo Overflow ;если выставлен флаг переполнения - выводим ошибку

        sub bl, '0'
        add eax, ebx
        inc esi
        cmp esi, ecx
        jl .Digit

    cmp eax, 0 ;особый случай - если введен 0
    je .Output_0
    ret

    .Output_0:
        mov eax, 4
        mov ebx, 1
        mov ecx, case_0
        mov edx, case_len
        int 80h

        mov eax, 1
        mov ebx, 0
        int 80h

;факторизует число из еах множителем из ebx, пока число не станет равно 1
Factorize:
    .Do:
        xor edx, edx ;если исходное число стало 1, факт-ия закончена
        cmp eax, 1
        je .Return

        mov esi, eax ;сохраняем старое число
        div ebx ;делим на множитель и если ост.== 0, то запоминаем мн-ль
        cmp edx, 0
        je .Is_fact

        mov eax, esi ;если ост != 0, возвращаем старое число и переключаем мн-ль
        inc ebx
        jmp .Do

    .Is_fact:
        push eax
        call Print_factor
        pop eax
        mov ebx, 2 ;начинаем заново перебор множителей
        jmp .Do

    .Return:
        ret

;печатает множитель из edx и знак умножения
Print_factor:
    xor esi, esi
    xor eax, eax

    mov eax, ebx
    xor ebx, ebx
    mov ebp, esp
    xor ecx, ecx

    .Factor:
        xor edx, edx
        mov ebx, 10 ;берем поразрядно цифры
        div ebx
        add edx, '0'
        push edx ;кладем на стек каждую цифру
        inc esi
        cmp eax, 0
        jne .Factor

    .Output:
        pop edx
        mov [digit], dl

        mov eax, 4
        mov ebx, 1
        mov ecx, digit
        mov edx, 1
        int 80h

        dec esi
        cmp esi, 0
        jne .Output

    
    .Star:
        mov eax, 4
        mov ebx, 1
        mov ecx, star
        mov edx, 1
        int 80h
            
    mov esp, ebp
    ret

;сообщает об ошибке и завершает программу, если число не явл.натуральным/не явл.числом
Err_input:
    mov eax, 4
    mov ebx, 1
    mov ecx, msgIN
    mov edx, msgIN_len
    int 80h

    mov eax, 1
    mov ebx, 0
    int 80h

;сообщает об ошибке и завершает программу, если обнаруженно переполнение
Overflow:
    mov eax, 4
    mov ebx, 1
    mov ecx, msgOF
    mov edx, msgOF_len
    int 80h

    mov eax, 1
    mov ebx, 0
    int 80h

