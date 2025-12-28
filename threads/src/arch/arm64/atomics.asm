.CODE

PUBLIC sn_atomic_load_int8_t, sn_atomic_store_int8_t
PUBLIC sn_atomic_exchange_int8_t, sn_atomic_compare_exchange_int8_t
PUBLIC sn_atomic_fetch_add_int8_t, sn_atomic_fetch_sub_int8_t
PUBLIC sn_atomic_fetch_or_int8_t, sn_atomic_fetch_xor_int8_t
PUBLIC sn_atomic_fetch_and_int8_t

PUBLIC sn_atomic_load_int16_t, sn_atomic_store_int16_t
PUBLIC sn_atomic_exchange_int16_t, sn_atomic_compare_exchange_int16_t
PUBLIC sn_atomic_fetch_add_int16_t, sn_atomic_fetch_sub_int16_t
PUBLIC sn_atomic_fetch_or_int16_t, sn_atomic_fetch_xor_int16_t
PUBLIC sn_atomic_fetch_and_int16_t

PUBLIC sn_atomic_load_int32_t, sn_atomic_store_int32_t
PUBLIC sn_atomic_exchange_int32_t, sn_atomic_compare_exchange_int32_t
PUBLIC sn_atomic_fetch_add_int32_t, sn_atomic_fetch_sub_int32_t
PUBLIC sn_atomic_fetch_or_int32_t, sn_atomic_fetch_xor_int32_t
PUBLIC sn_atomic_fetch_and_int32_t

PUBLIC sn_atomic_load_int64_t, sn_atomic_store_int64_t
PUBLIC sn_atomic_exchange_int64_t, sn_atomic_compare_exchange_int64_t
PUBLIC sn_atomic_fetch_add_int64_t, sn_atomic_fetch_sub_int64_t
PUBLIC sn_atomic_fetch_or_int64_t, sn_atomic_fetch_xor_int64_t
PUBLIC sn_atomic_fetch_and_int64_t

PUBLIC sn_atomic_load_uint8_t, sn_atomic_store_uint8_t
PUBLIC sn_atomic_exchange_uint8_t, sn_atomic_compare_exchange_uint8_t
PUBLIC sn_atomic_fetch_add_uint8_t, sn_atomic_fetch_sub_uint8_t
PUBLIC sn_atomic_fetch_or_uint8_t, sn_atomic_fetch_xor_uint8_t
PUBLIC sn_atomic_fetch_and_uint8_t

PUBLIC sn_atomic_load_uint16_t, sn_atomic_store_uint16_t
PUBLIC sn_atomic_exchange_uint16_t, sn_atomic_compare_exchange_uint16_t
PUBLIC sn_atomic_fetch_add_uint16_t, sn_atomic_fetch_sub_uint16_t
PUBLIC sn_atomic_fetch_or_uint16_t, sn_atomic_fetch_xor_uint16_t
PUBLIC sn_atomic_fetch_and_uint16_t

PUBLIC sn_atomic_load_uint32_t, sn_atomic_store_uint32_t
PUBLIC sn_atomic_exchange_uint32_t, sn_atomic_compare_exchange_uint32_t
PUBLIC sn_atomic_fetch_add_uint32_t, sn_atomic_fetch_sub_uint32_t
PUBLIC sn_atomic_fetch_or_uint32_t, sn_atomic_fetch_xor_uint32_t
PUBLIC sn_atomic_fetch_and_uint32_t

PUBLIC sn_atomic_load_uint64_t, sn_atomic_store_uint64_t
PUBLIC sn_atomic_exchange_uint64_t, sn_atomic_compare_exchange_uint64_t
PUBLIC sn_atomic_fetch_add_uint64_t, sn_atomic_fetch_sub_uint64_t
PUBLIC sn_atomic_fetch_or_uint64_t, sn_atomic_fetch_xor_uint64_t
PUBLIC sn_atomic_fetch_and_uint64_t

PUBLIC sn_atomic_flag_test_and_set_explicit
PUBLIC sn_atomic_flag_clear_explicit
PUBLIC sn_atomic_flag_load_explicit

PUBLIC sn_memory_fence

PRE_ATOMIC_LOAD_FENCE MACRO memory_order
	LOCAL no_fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne no_fence
	mfence
no_fence:
ENDM

POST_ATOMIC_LOAD_FENCE MACRO memory_order
	LOCAL fence, done
	cmp memory_order, 1 ; SN_MEMORY_ORDER_ACQUIRE
	je fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne done
fence:
	lfence
done:
ENDM

PRE_ATOMIC_STORE_FENCE MACRO memory_order
	LOCAL fence, done
	cmp memory_order, 2 ; SN_MEMORY_ORDER_RELEASE
	je fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne done
fence:
	sfence
done:
ENDM

POST_ATOMIC_STORE_FENCE MACRO memory_order
	LOCAL no_fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne no_fence
	mfence
no_fence:
ENDM

PRE_ATOMIC_RMW_FENCE MACRO memory_order
	LOCAL store_fence, done
	cmp memory_order, 2 ; SN_MEMORY_ORDER_RELEASE
	je store_fence
	cmp memory_order, 3 ; SN_MEMORY_ORDER_ACQ_REL
	je store_fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne done
	mfence
	jmp done
store_fence:
	sfence
done:
ENDM

POST_ATOMIC_RMW_FENCE MACRO memory_order
	LOCAL load_fence, done
	cmp memory_order, 1 ; SN_MEMORY_ORDER_ACQUIRE
	je load_fence
	cmp memory_order, 3 ; SN_MEMORY_ORDER_ACQ_REL
	je load_fence
	cmp memory_order, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne done
	mfence
	jmp done
load_fence:
	lfence
done:
ENDM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; LOAD OPERATIONS ;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, memory_order
sn_atomic_load_int8_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov al, byte ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_int8_t ENDP

; obj, memory_order
sn_atomic_load_int16_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov ax, word ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_int16_t ENDP

; obj, memory_order
sn_atomic_load_int32_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov eax, dword ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_int32_t ENDP

; obj, memory_order
sn_atomic_load_int64_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov rax, qword ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_int64_t ENDP

; obj, memory_order
sn_atomic_load_uint8_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov al, byte ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_uint8_t ENDP

; obj, memory_order
sn_atomic_load_uint16_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov ax, word ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_uint16_t ENDP

; obj, memory_order
sn_atomic_load_uint32_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov eax, dword ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_uint32_t ENDP

; obj, memory_order
sn_atomic_load_uint64_t PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov rax, qword ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_load_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; STORE OPERATIONS ;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_store_int8_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov byte ptr [rcx], dl
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_int8_t ENDP

; obj, value, memory_order
sn_atomic_store_int16_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov word ptr [rcx], dx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_int16_t ENDP

; obj, value, memory_order
sn_atomic_store_int32_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov dword ptr [rcx], edx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_int32_t ENDP

; obj, value, memory_order
sn_atomic_store_int64_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov qword ptr [rcx], rdx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_int64_t ENDP

; obj, value, memory_order
sn_atomic_store_uint8_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov byte ptr [rcx], dl
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_uint8_t ENDP

; obj, value, memory_order
sn_atomic_store_uint16_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov word ptr [rcx], dx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_uint16_t ENDP

; obj, value, memory_order
sn_atomic_store_uint32_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov dword ptr [rcx], edx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_uint32_t ENDP

; obj, value, memory_order
sn_atomic_store_uint64_t PROC
	PRE_ATOMIC_STORE_FENCE r8d
	mov qword ptr [rcx], rdx
	POST_ATOMIC_STORE_FENCE r8d
	ret
sn_atomic_store_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; EXCHANGE OPERATIONS ;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_exchange_int8_t PROC
	mov al, dl
	lock xchg al, byte ptr [rcx]
	ret
sn_atomic_exchange_int8_t ENDP

; obj, value, memory_order
sn_atomic_exchange_int16_t PROC
	mov ax, dx
	lock xchg ax, word ptr [rcx]
	ret
sn_atomic_exchange_int16_t ENDP

; obj, value, memory_order
sn_atomic_exchange_int32_t PROC
	mov eax, edx
	lock xchg eax, dword ptr [rcx]
	ret
sn_atomic_exchange_int32_t ENDP

; obj, value, memory_order
sn_atomic_exchange_int64_t PROC
	mov rax, rdx
	lock xchg rax, qword ptr [rcx]
	ret
sn_atomic_exchange_int64_t ENDP

; obj, value, memory_order
sn_atomic_exchange_uint8_t PROC
	mov al, dl
	lock xchg al, byte ptr [rcx]
	ret
sn_atomic_exchange_uint8_t ENDP

; obj, value, memory_order
sn_atomic_exchange_uint16_t PROC
	mov ax, dx
	lock xchg ax, word ptr [rcx]
	ret
sn_atomic_exchange_uint16_t ENDP

; obj, value, memory_order
sn_atomic_exchange_uint32_t PROC
	mov eax, edx
	lock xchg eax, dword ptr [rcx]
	ret
sn_atomic_exchange_uint32_t ENDP

; obj, value, memory_order
sn_atomic_exchange_uint64_t PROC
	mov rax, rdx
	lock xchg rax, qword ptr [rcx]
	ret
sn_atomic_exchange_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; COMPARE EXCHANGE OPERATIONS ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, expect, value, success, fail
sn_atomic_compare_exchange_int8_t PROC
	mov al, byte ptr [rdx]
	lock cmpxchg byte ptr [rcx], r8b
	mov byte ptr [rdx], al
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_int8_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_int16_t PROC
	mov ax, word ptr [rdx]
	lock cmpxchg word ptr [rcx], r8w
	mov word ptr [rdx], ax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_int16_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_int32_t PROC
	mov eax, dword ptr [rdx]
	lock cmpxchg dword ptr [rcx], r8d
	mov dword ptr [rdx], eax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_int32_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_int64_t PROC
	mov rax, qword ptr [rdx]
	lock cmpxchg qword ptr [rcx], r8
	mov qword ptr [rdx], rax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_int64_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_uint8_t PROC
	mov al, byte ptr [rdx]
	lock cmpxchg byte ptr [rcx], r8b
	mov byte ptr [rdx], al
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_uint8_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_uint16_t PROC
	mov ax, word ptr [rdx]
	lock cmpxchg word ptr [rcx], r8w
	mov word ptr [rdx], ax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_uint16_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_uint32_t PROC
	mov eax, dword ptr [rdx]
	lock cmpxchg dword ptr [rcx], r8d
	mov dword ptr [rdx], eax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_uint32_t ENDP

; obj, expect, value, success, fail
sn_atomic_compare_exchange_uint64_t PROC
	mov rax, qword ptr [rdx]
	lock cmpxchg qword ptr [rcx], r8
	mov qword ptr [rdx], rax
	jne not_equal
	mov al, 1
	ret
not_equal:
	xor al, al
	ret
sn_atomic_compare_exchange_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FETCH ADD OPERATIONS ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_fetch_add_int8_t PROC
	mov al, dl
	lock xadd byte ptr [rcx], al
	ret
sn_atomic_fetch_add_int8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_int16_t PROC
	mov ax, dx
	lock xadd word ptr [rcx], ax
	ret
sn_atomic_fetch_add_int16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_int32_t PROC
	mov eax, edx
	lock xadd dword ptr [rcx], eax
	ret
sn_atomic_fetch_add_int32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_int64_t PROC
	mov rax, rdx
	lock xadd qword ptr [rcx], rax
	ret
sn_atomic_fetch_add_int64_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_uint8_t PROC
	mov al, dl
	lock xadd byte ptr [rcx], al
	ret
sn_atomic_fetch_add_uint8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_uint16_t PROC
	mov ax, dx
	lock xadd word ptr [rcx], ax
	ret
sn_atomic_fetch_add_uint16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_uint32_t PROC
	mov eax, edx
	lock xadd dword ptr [rcx], eax
	ret
sn_atomic_fetch_add_uint32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_add_uint64_t PROC
	mov rax, rdx
	lock xadd qword ptr [rcx], rax
	ret
sn_atomic_fetch_add_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FETCH SUB OPERATIONS ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_fetch_sub_int8_t PROC
	mov al, dl
	neg al
	lock xadd byte ptr [rcx], al
	ret
sn_atomic_fetch_sub_int8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_int16_t PROC
	mov ax, dx
	neg ax
	lock xadd word ptr [rcx], ax
	ret
sn_atomic_fetch_sub_int16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_int32_t PROC
	mov eax, edx
	neg eax
	lock xadd dword ptr [rcx], eax
	ret
sn_atomic_fetch_sub_int32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_int64_t PROC
	mov rax, rdx
	neg rax
	lock xadd qword ptr [rcx], rax
	ret
sn_atomic_fetch_sub_int64_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_uint8_t PROC
	mov al, dl
	neg al
	lock xadd byte ptr [rcx], al
	ret
sn_atomic_fetch_sub_uint8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_uint16_t PROC
	mov ax, dx
	neg ax
	lock xadd word ptr [rcx], ax
	ret
sn_atomic_fetch_sub_uint16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_uint32_t PROC
	mov eax, edx
	neg eax
	lock xadd dword ptr [rcx], eax
	ret
sn_atomic_fetch_sub_uint32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_sub_uint64_t PROC
	mov rax, rdx
	neg rax
	lock xadd qword ptr [rcx], rax
	ret
sn_atomic_fetch_sub_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FETCH OR OPERATIONS ;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_fetch_or_int8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	or r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_int8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_int16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	or r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_int16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_int32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	or r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_int32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_int64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	or r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_int64_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_uint8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	or r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_uint8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_uint16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	or r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_uint16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_uint32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	or r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_uint32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_or_uint64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	or r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_or_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FETCH XOR OPERATIONS ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_fetch_xor_int8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	xor r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_int8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_int16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	xor r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_int16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_int32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	xor r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_int32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_int64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	xor r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_int64_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_uint8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	xor r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_uint8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_uint16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	xor r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_uint16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_uint32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	xor r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_uint32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_xor_uint64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	xor r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_xor_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FETCH AND OPERATIONS ;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, value, memory_order
sn_atomic_fetch_and_int8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	and r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_int8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_int16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	and r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_int16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_int32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	and r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_int32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_int64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	and r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_int64_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_uint8_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov al, byte ptr [rcx]

retry:
	mov r10b, al
	and r10b, dl
	lock cmpxchg byte ptr [rcx], r10b
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_uint8_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_uint16_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov ax, word ptr [rcx]

retry:
	mov r10w, ax
	and r10w, dx
	lock cmpxchg word ptr [rcx], r10w
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_uint16_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_uint32_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov eax, dword ptr [rcx]

retry:
	mov r10d, eax
	and r10d, edx
	lock cmpxchg dword ptr [rcx], r10d
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_uint32_t ENDP

; obj, value, memory_order
sn_atomic_fetch_and_uint64_t PROC
	PRE_ATOMIC_RMW_FENCE r8d

	mov rax, qword ptr [rcx]

retry:
	mov r10, rax
	and r10, rdx
	lock cmpxchg qword ptr [rcx], r10
	jne retry

	POST_ATOMIC_RMW_FENCE r8d
	ret
sn_atomic_fetch_and_uint64_t ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MEMORY FENCE FUNCTION ;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; memory_order of fence
sn_memory_fence PROC
	cmp ecx, 0 ; SN_MEMORY_ORDER_NONE
	je done
	cmp ecx, 1 ; SN_EMMORY_ORDER_ACQUIRE
	je load_fence
	cmp ecx, 2; SN_MEMORY_ORDER_RELEASE
	je store_fence
	cmp ecx, 3 ; SN_MEMORY_ORDER_ACQ_REL
	je full_fence
	cmp ecx, 4 ; SN_MEMORY_ORDER_TOTAL_ORDER
	jne done
full_fence:
	mfence
	ret
store_fence:
	sfence
	ret
load_fence:
	lfence
done:
	ret
sn_memory_fence ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATOMIC FLAG OPERATIONS ;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; obj, memory_order
sn_atomic_flag_test_and_set_explicit PROC
	mov al, 1
	lock xchg al, byte ptr [rcx]
	ret
sn_atomic_flag_test_and_set_explicit ENDP

; obj, memory_order
sn_atomic_flag_clear_explicit PROC
	PRE_ATOMIC_RMW_FENCE edx
	mov byte ptr [rcx], 0
	POST_ATOMIC_RMW_FENCE edx
	ret
sn_atomic_flag_clear_explicit ENDP

; obj, memory_order
sn_atomic_flag_load_explicit PROC
	PRE_ATOMIC_LOAD_FENCE edx
	mov al, byte ptr [rcx]
	POST_ATOMIC_LOAD_FENCE edx
	ret
sn_atomic_flag_load_explicit ENDP

END
