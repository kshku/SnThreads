AREA |.text|, CODE, READONLY

EXPORT sn_atomic_load_int8_t, sn_atomic_store_int8_t
EXPORT sn_atomic_exchange_int8_t, sn_atomic_compare_exchange_int8_t
EXPORT sn_atomic_fetch_add_int8_t, sn_atomic_fetch_sub_int8_t
EXPORT sn_atomic_fetch_or_int8_t, sn_atomic_fetch_xor_int8_t
EXPORT sn_atomic_fetch_and_int8_t

EXPORT sn_atomic_load_int16_t, sn_atomic_store_int16_t
EXPORT sn_atomic_exchange_int16_t, sn_atomic_compare_exchange_int16_t
EXPORT sn_atomic_fetch_add_int16_t, sn_atomic_fetch_sub_int16_t
EXPORT sn_atomic_fetch_or_int16_t, sn_atomic_fetch_xor_int16_t
EXPORT sn_atomic_fetch_and_int16_t

EXPORT sn_atomic_load_int32_t, sn_atomic_store_int32_t
EXPORT sn_atomic_exchange_int32_t, sn_atomic_compare_exchange_int32_t
EXPORT sn_atomic_fetch_add_int32_t, sn_atomic_fetch_sub_int32_t
EXPORT sn_atomic_fetch_or_int32_t, sn_atomic_fetch_xor_int32_t
EXPORT sn_atomic_fetch_and_int32_t

EXPORT sn_atomic_load_int64_t, sn_atomic_store_int64_t
EXPORT sn_atomic_exchange_int64_t, sn_atomic_compare_exchange_int64_t
EXPORT sn_atomic_fetch_add_int64_t, sn_atomic_fetch_sub_int64_t
EXPORT sn_atomic_fetch_or_int64_t, sn_atomic_fetch_xor_int64_t
EXPORT sn_atomic_fetch_and_int64_t

EXPORT sn_atomic_load_uint8_t, sn_atomic_store_uint8_t
EXPORT sn_atomic_exchange_uint8_t, sn_atomic_compare_exchange_uint8_t
EXPORT sn_atomic_fetch_add_uint8_t, sn_atomic_fetch_sub_uint8_t
EXPORT sn_atomic_fetch_or_uint8_t, sn_atomic_fetch_xor_uint8_t
EXPORT sn_atomic_fetch_and_uint8_t

EXPORT sn_atomic_load_uint16_t, sn_atomic_store_uint16_t
EXPORT sn_atomic_exchange_uint16_t, sn_atomic_compare_exchange_uint16_t
EXPORT sn_atomic_fetch_add_uint16_t, sn_atomic_fetch_sub_uint16_t
EXPORT sn_atomic_fetch_or_uint16_t, sn_atomic_fetch_xor_uint16_t
EXPORT sn_atomic_fetch_and_uint16_t

EXPORT sn_atomic_load_uint32_t, sn_atomic_store_uint32_t
EXPORT sn_atomic_exchange_uint32_t, sn_atomic_compare_exchange_uint32_t
EXPORT sn_atomic_fetch_add_uint32_t, sn_atomic_fetch_sub_uint32_t
EXPORT sn_atomic_fetch_or_uint32_t, sn_atomic_fetch_xor_uint32_t
EXPORT sn_atomic_fetch_and_uint32_t

EXPORT sn_atomic_load_uint64_t, sn_atomic_store_uint64_t
EXPORT sn_atomic_exchange_uint64_t, sn_atomic_compare_exchange_uint64_t
EXPORT sn_atomic_fetch_add_uint64_t, sn_atomic_fetch_sub_uint64_t
EXPORT sn_atomic_fetch_or_uint64_t, sn_atomic_fetch_xor_uint64_t
EXPORT sn_atomic_fetch_and_uint64_t

EXPORT sn_atomic_flag_test_and_set_explicit
EXPORT sn_atomic_flag_clear_explicit
EXPORT sn_atomic_flag_load_explicit

EXPORT sn_memory_fence

EXPORT sn_pause_instruction

; Fence macros
    MACRO
    PRE_ATOMIC_LOAD_FENCE $order
    cmp $order, #4
    bne {skip}
    dmb ish
{skip}
    MEND

    MACRO
    POST_ATOMIC_LOAD_FENCE $order
    cmp $order, #1
    beq {fence}
    cmp $order, #4
    bne {skip}
{fence}
    dmb ishld
{skip}
    MEND

    MACRO
    PRE_ATOMIC_STORE_FENCE $order
    cmp $order, #2
    beq {fence}
    cmp $order, #4
    bne {skip}
{fence}
    dmb ishst
{skip}
    MEND

    MACRO
    POST_ATOMIC_STORE_FENCE $order
    cmp $order, #4
    bne {skip}
    dmb ish
{skip}
    MEND

    MACRO
    PRE_ATOMIC_RMW_FENCE $order
    cmp $order, #2
    beq {store_fence}
    cmp $order, #3
    beq {store_fence}
    cmp $order, #4
    bne {skip}
    dmb ish
    b {skip}
{store_fence}
    dmb ishst
{skip}
    MEND

    MACRO
    POST_ATOMIC_RMW_FENCE $order
    cmp $order, #1
    beq {load_fence}
    cmp $order, #3
    beq {load_fence}
    cmp $order, #4
    bne {skip}
    dmb ish
    b {skip}
{load_fence}
    dmb ishld
{skip}
    MEND

; Load
    MACRO
    LOAD_FN $name, $instr
$name PROC
    PRE_ATOMIC_LOAD_FENCE W1
    $instr w0, [x0]
    POST_ATOMIC_LOAD_FENCE W1
    ret
    ENDP
    MEND

    MACRO
    LOAD64_FN $name
$name PROC
    PRE_ATOMIC_LOAD_FENCE W1
    ldr x0, [x0]
    POST_ATOMIC_LOAD_FENCE W1
    ret
    ENDP
    MEND

    LOAD_FN sn_atomic_load_int8_t, ldrb
    LOAD_FN sn_atomic_load_int16_t, ldrh
    LOAD_FN sn_atomic_load_int32_t, ldr
    LOAD64_FN sn_atomic_load_int64_t
    LOAD_FN sn_atomic_load_uint8_t, ldrb
    LOAD_FN sn_atomic_load_uint16_t, ldrh
    LOAD_FN sn_atomic_load_uint32_t, ldr
    LOAD64_FN sn_atomic_load_uint64_t

; Store
    MACRO
    STORE_FN $name, $instr
$name PROC
    PRE_ATOMIC_STORE_FENCE W2
    $instr w1, [x0]
    POST_ATOMIC_STORE_FENCE W2
    ret
    ENDP
    MEND

    MACRO
    STORE64_FN $name
$name PROC
    PRE_ATOMIC_STORE_FENCE W2
    str x1, [x0]
    POST_ATOMIC_STORE_FENCE W2
    ret
    ENDP
    MEND

    STORE_FN sn_atomic_store_int8_t, strb
    STORE_FN sn_atomic_store_int16_t, strh
    STORE_FN sn_atomic_store_int32_t, str
    STORE64_FN sn_atomic_store_int64_t
    STORE_FN sn_atomic_store_uint8_t, strb
    STORE_FN sn_atomic_store_uint16_t, strh
    STORE_FN sn_atomic_store_uint32_t, str
    STORE64_FN sn_atomic_store_uint64_t

; Exchange via LL/SC
    MACRO
    EXCHANGE_FN $name, $ldxr, $stxr, $mov_ret
$name PROC
    dmb ish
{retry}
    $ldxr
    $stxr
    cbnz w3, {retry}
    $mov_ret
    dmb ish
    ret
    ENDP
    MEND

    EXCHANGE_FN sn_atomic_exchange_int8_t, ldxrb w4, [x0], stxrb w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_int16_t, ldxrh w4, [x0], stxrh w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_int32_t, ldxr w4, [x0], stxr w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_int64_t, ldxr x4, [x0], stxr w3, x1, [x0], mov x0, x4
    EXCHANGE_FN sn_atomic_exchange_uint8_t, ldxrb w4, [x0], stxrb w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_uint16_t, ldxrh w4, [x0], stxrh w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_uint32_t, ldxr w4, [x0], stxr w3, w1, [x0], mov w0, w4
    EXCHANGE_FN sn_atomic_exchange_uint64_t, ldxr x4, [x0], stxr w3, x1, [x0], mov x0, x4

; Compare-exchange via LL/SC
    MACRO
    CAS_FN $name, $ldr_expect, $ldxr, $stxr, $cmp, $str_update
$name PROC
    dmb ish
    $ldr_expect
{retry}
    $ldxr
    $cmp
    bne {done}
    $stxr
    cbnz w6, {retry}
    dmb ish
    mov w0, #1
    ret
{done}
    $str_update
    mov w0, #0
    ret
    ENDP
    MEND

    CAS_FN sn_atomic_compare_exchange_int8_t, ldrb w5, [x1], ldxrb w4, [x0], stxrb w6, w2, [x0], cmp w4, w5, strb w4, [x1]
    CAS_FN sn_atomic_compare_exchange_int16_t, ldrh w5, [x1], ldxrh w4, [x0], stxrh w6, w2, [x0], cmp w4, w5, strh w4, [x1]
    CAS_FN sn_atomic_compare_exchange_int32_t, ldr w5, [x1], ldxr w4, [x0], stxr w6, w2, [x0], cmp w4, w5, str w4, [x1]
    CAS_FN sn_atomic_compare_exchange_int64_t, ldr x5, [x1], ldxr x4, [x0], stxr w6, x2, [x0], cmp x4, x5, str x4, [x1]
    CAS_FN sn_atomic_compare_exchange_uint8_t, ldrb w5, [x1], ldxrb w4, [x0], stxrb w6, w2, [x0], cmp w4, w5, strb w4, [x1]
    CAS_FN sn_atomic_compare_exchange_uint16_t, ldrh w5, [x1], ldxrh w4, [x0], stxrh w6, w2, [x0], cmp w4, w5, strh w4, [x1]
    CAS_FN sn_atomic_compare_exchange_uint32_t, ldr w5, [x1], ldxr w4, [x0], stxr w6, w2, [x0], cmp w4, w5, str w4, [x1]
    CAS_FN sn_atomic_compare_exchange_uint64_t, ldr x5, [x1], ldxr x4, [x0], stxr w6, x2, [x0], cmp x4, x5, str x4, [x1]

; Fetch-add via LL/SC
    MACRO
    FETCH_ADD_FN $name, $ldxr, $stxr, $mov_ret
$name PROC
    dmb ish
{retry}
    $ldxr
    add w5, w4, w1
    $stxr
    cbnz w3, {retry}
    $mov_ret
    dmb ish
    ret
    ENDP
    MEND

    MACRO
    FETCH_ADD64_FN $name
$name PROC
    dmb ish
{retry}
    ldxr x4, [x0]
    add x5, x4, x1
    stxr w3, x5, [x0]
    cbnz w3, {retry}
    mov x0, x4
    dmb ish
    ret
    ENDP
    MEND

    FETCH_ADD_FN sn_atomic_fetch_add_int8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_add_int16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_add_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_ADD64_FN sn_atomic_fetch_add_int64_t
    FETCH_ADD_FN sn_atomic_fetch_add_uint8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_add_uint16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_add_uint32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_ADD64_FN sn_atomic_fetch_add_uint64_t

; Fetch-sub via LL/SC
    FETCH_ADD_FN sn_atomic_fetch_sub_int8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_sub_int16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_sub_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_ADD64_FN sn_atomic_fetch_sub_int64_t
    FETCH_ADD_FN sn_atomic_fetch_sub_uint8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_sub_uint16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_ADD_FN sn_atomic_fetch_sub_uint32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_ADD64_FN sn_atomic_fetch_sub_uint64_t

; Fetch-or via LL/SC
    MACRO
    FETCH_OR_FN $name, $ldxr, $stxr, $mov_ret
$name PROC
    PRE_ATOMIC_RMW_FENCE W2
{retry}
    $ldxr
    orr w5, w4, w1
    $stxr
    cbnz w3, {retry}
    $mov_ret
    POST_ATOMIC_RMW_FENCE W2
    ret
    ENDP
    MEND

    MACRO
    FETCH_OR64_FN $name
$name PROC
    PRE_ATOMIC_RMW_FENCE W2
{retry}
    ldxr x4, [x0]
    orr x5, x4, x1
    stxr w3, x5, [x0]
    cbnz w3, {retry}
    mov x0, x4
    POST_ATOMIC_RMW_FENCE W2
    ret
    ENDP
    MEND

    FETCH_OR_FN sn_atomic_fetch_or_int8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_or_int16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_or_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_or_int64_t
    FETCH_OR_FN sn_atomic_fetch_or_uint8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_or_uint16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_or_uint32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_or_uint64_t

; Fetch-xor via LL/SC
    FETCH_OR_FN sn_atomic_fetch_xor_int8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_xor_int16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_xor_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_xor_int64_t
    FETCH_OR_FN sn_atomic_fetch_xor_uint8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_xor_uint16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_xor_uint32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_xor_uint64_t

; Fetch-and via LL/SC
    FETCH_OR_FN sn_atomic_fetch_and_int8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_and_int16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_and_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_and_int64_t
    FETCH_OR_FN sn_atomic_fetch_and_uint8_t, ldxrb w4, [x0], stxrb w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_and_uint16_t, ldxrh w4, [x0], stxrh w3, w5, [x0], mov w0, w4
    FETCH_OR_FN sn_atomic_fetch_and_int32_t, ldxr w4, [x0], stxr w3, w5, [x0], mov w0, w4
    FETCH_OR64_FN sn_atomic_fetch_and_uint64_t

; Memory fence
sn_memory_fence PROC
    cmp w0, #0
    beq {done}
    cmp w0, #1
    beq {acquire}
    cmp w0, #2
    beq {release}
    cmp w0, #3
    beq {full}
    cmp w0, #4
    bne {done}
{full}
    dmb ish
    ret
{release}
    dmb ishst
    ret
{acquire}
    dmb ishld
{done}
    ret
    ENDP

; Pause instruction
sn_pause_instruction PROC
    yield
    ret
    ENDP

; Atomic flag test and set
sn_atomic_flag_test_and_set_explicit PROC
    dmb ish
{retry}
    ldaxrb w0, [x0]
    mov w4, #1
    stlxrb w3, w4, [x0]
    cbnz w3, {retry}
    dmb ish
    ret
    ENDP

; Atomic flag clear
sn_atomic_flag_clear_explicit PROC
    PRE_ATOMIC_RMW_FENCE W1
    strb wzr, [x0]
    POST_ATOMIC_RMW_FENCE W1
    ret
    ENDP

; Atomic flag load
sn_atomic_flag_load_explicit PROC
    PRE_ATOMIC_LOAD_FENCE W1
    ldrb w0, [x0]
    POST_ATOMIC_LOAD_FENCE W1
    ret
    ENDP

END
