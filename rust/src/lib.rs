extern crate libc;

use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_void};
use libsql_ffi::{sqlite3_context, sqlite3_free, sqlite3_int64, sqlite3_result_text, SQLITE3_TEXT, sqlite3_value, sqlite3_value_text, sqlite3_value_type, SQLITE_STATIC, SQLITE_TRANSIENT};


#[no_mangle]
pub extern "C" fn greet(context: *mut sqlite3_context, argc: sqlite3_int64, argv: *const *mut sqlite3_value) {
    assert_eq!(argc, 1);
    unsafe {
        let rust_string = "Hellooooo!, World!";
        let c_string = CString::new(rust_string).unwrap();
        let c_string_raw = c_string.into_raw();
        sqlite3_result_text(context, c_string_raw, -1, SQLITE_TRANSIENT());
    }
}


pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
