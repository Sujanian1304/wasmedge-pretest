(module
  (type $type0 (func (param i32 i32)(result i32)))
  ;; Import the "add" function which calculate "a + b".
  (import "math" "add" (func $math-add (type $type0)))
  ;; Import the "mul" function which calculate "a * b".
  (import "math" "mul" (func $math-mul (type $type0)))
  (func (export "add_and_square") (param i32 i32) (result i32)
    ;; Function to add 2 numbers and square it ((a + b)^2).
    ;; Exported as "add_and_square".
    (call $math-mul
      (call $math-add (local.get 0) (local.get 1))
      (call $math-add (local.get 0) (local.get 1))
    )
  )
  (func (export "sum_of_squares") (param i32 i32) (result i32)
    ;; Function to calculate the sum of squares (a^2 + b^2).
    ;; Exported as "sum_of_squares".
    (call $math-add
      (call $math-mul (local.get 0) (local.get 0))
      (call $math-mul (local.get 1) (local.get 1))
    )
  )
)