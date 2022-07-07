### Dependency ###

experimental_support_hmac_src = files(
    'hmac.cpp',
)

experimental_support_hmac = declare_dependency(
    sources : experimental_support_hmac_src
)

experimental_library_deps += experimental_support_hmac

### Tests ###

experimental_unit_tests += {
    'name'      : 'HMAC Unit Test',
    'src'       : files('tests/unit_tests/hmac_tests.cpp',
                        'hmac.cpp'),
}
