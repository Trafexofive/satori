// tests/test_module_system.c - Direct test of module system
//
// Manually creates bytecode to test the module loading and native function calls.

#include "runtime/vm.h"
#include "runtime/module.h"
#include "core/value.h"
#include <stdio.h>

int main(void) {
  VM vm;
  vm_init(&vm);
  
  printf("=== Module System Test ===\n\n");
  
  // Test 1: Load io module
  printf("Test 1: Loading io module... ");
  if (module_load(&vm, "io")) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  // Test 2: Load string module
  printf("Test 2: Loading string module... ");
  if (module_load(&vm, "string")) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  // Test 3: Re-load io module (should be cached)
  printf("Test 3: Re-loading io module (should be cached)... ");
  if (module_load(&vm, "io")) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  // Test 4: Check that functions are registered
  printf("Test 4: Checking io.println is registered... ");
  Value fn_value;
  if (table_get(&vm.globals, "io.println", &fn_value) && IS_NATIVE_FN(fn_value)) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  printf("Test 5: Checking io.print is registered... ");
  if (table_get(&vm.globals, "io.print", &fn_value) && IS_NATIVE_FN(fn_value)) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  printf("Test 6: Checking string.to_upper is registered... ");
  if (table_get(&vm.globals, "string.to_upper", &fn_value) && IS_NATIVE_FN(fn_value)) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  printf("Test 7: Checking string.to_lower is registered... ");
  if (table_get(&vm.globals, "string.to_lower", &fn_value) && IS_NATIVE_FN(fn_value)) {
    printf("SUCCESS\n");
  } else {
    printf("FAILED\n");
    return 1;
  }
  
  // Test 8: Call io.println directly
  printf("\nTest 8: Calling io.println directly:\n");
  Value println_fn;
  table_get(&vm.globals, "io.println", &println_fn);
  NativeFn println = AS_NATIVE_FN(println_fn);
  
  Value args[2];
  args[0] = value_make_string("Hello from module system!");
  println(1, args);
  value_free(args[0]);
  
  // Test 9: Call io.println with format string
  printf("\nTest 9: Calling io.println with format:\n");
  args[0] = value_make_string("Number: {}");
  args[1] = value_make_int(42);
  println(2, args);
  value_free(args[0]);
  
  // Test 10: Call string.to_upper
  printf("\nTest 10: Calling string.to_upper:\n");
  Value to_upper_fn;
  table_get(&vm.globals, "string.to_upper", &to_upper_fn);
  NativeFn to_upper = AS_NATIVE_FN(to_upper_fn);
  
  args[0] = value_make_string("hello world");
  Value result = to_upper(1, args);
  printf("  Input: ");
  value_print(args[0]);
  printf("\n  Output: ");
  value_print(result);
  printf("\n");
  value_free(args[0]);
  value_free(result);
  
  printf("\n=== All tests passed! ===\n");
  
  vm_free(&vm);
  return 0;
}
