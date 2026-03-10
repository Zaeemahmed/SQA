# Functional test cases for `AntPathMatcher.match()`

This table maps the planned JUnit tests to concrete values.

| ID | Matcher config | Pattern | Path | Expected |
|---|---|---|---|---|
| TC01 | default | `machin/truc/file.js` | `machin/truc/file.js` | `true` |
| TC02 | default | `/test` | `/other` | `false` |
| TC03 | default | `/t?st` | `/test` | `true` |
| TC04 | default | `/t?st` | `/toast` | `false` |
| TC05 | default | `/test/*` | `/test/abc` | `true` |
| TC06 | default | `/test/*` | `/test/abc/def` | `false` |
| TC07 | default | `/test/**` | `/test/abc/def` | `true` |
| TC08 | default | `/**/test.jsp` | `/a/b/test.jsp` | `true` |
| TC09 | default | `/com/{filename:\\w+}.jsp` | `/com/test.jsp` | `true` |
| TC10 | default | `/com/{filename:\\w+}.jsp` | `/com/test.html` | `false` |
| TC11 | default | `/test` | `test` | `false` |
| TC12 | default | `` | `` | `true` |
| TC13 | default | `` | `/test` | `false` |
| TC14 | default | `/test` | `null` | `false` |
| TC15 | default | `null` | `/test` | `NullPointerException` |
| TC16 | case-insensitive | `/TeSt` | `/test` | `true` |
