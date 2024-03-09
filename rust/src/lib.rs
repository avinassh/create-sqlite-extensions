extern crate libc;

use std::ffi::{CStr, CString};
use libsql_ffi::{sqlite3_context, sqlite3_free, sqlite3_int64, sqlite3_result_text, SQLITE3_TEXT, sqlite3_value, sqlite3_value_text, sqlite3_value_type, SQLITE_TRANSIENT};


#[no_mangle]
pub extern "C" fn greet(context: *mut sqlite3_context, argc: sqlite3_int64, argv: *const *mut sqlite3_value) {
    assert_eq!(argc, 1);
    unsafe {
        if argv.is_null() {
            return;
        }
        let param = *argv.offset(0);
        if param.is_null() {
            // error out
        }
        if sqlite3_value_type(param) != SQLITE3_TEXT {
            // error out
        }
        let c_char = sqlite3_value_text(param);
        if c_char.is_null() {
            // error out
        }
        let input_str = CStr::from_ptr(c_char as *const std::os::raw::c_char);
        let input = input_str.to_str().unwrap();

        let result = format!("Hellooo, {}!", input);
        let c_result = CString::new(result.as_str()).unwrap();
        let c_result_ptr = c_result.into_raw();
        sqlite3_result_text(context, c_result_ptr, -1, SQLITE_TRANSIENT());
        sqlite3_free(c_result_ptr as *mut std::ffi::c_void);
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
