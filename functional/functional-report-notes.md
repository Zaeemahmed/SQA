# Functional testing notes - Assignment 1

## Scope
These notes cover the functional-testing part for `AntPathMatcher.match(pattern, path)`.

## Chosen categories
1. **MatcherConfig**
   - `DEFAULT`
   - `CASE_INSENSITIVE`

   This is the main environment element that changes observable behaviour of `match()` without changing the method signature.

2. **PatternKind**
   - `EXACT`
   - `QUESTION`
   - `STAR_SEGMENT`
   - `DOUBLESTAR`
   - `URI_REGEX`
   - `EMPTY`
   - `NULL_PATTERN`

   These values capture the main specification features of Ant-style matching and also include singular/error-oriented inputs.

3. **PathKind**
   - `EXACT_PATH`
   - `SINGLE_CHAR_VARIANT`
   - `CHILD_PATH`
   - `DEEP_PATH`
   - `DIFFERENT_PATH`
   - `EMPTY_PATH`
   - `NULL_PATH`
   - `CASE_VARIANT`
   - `URI_VALID`
   - `URI_INVALID`

   These values distinguish normal paths, wildcard-sensitive paths, URI-template paths, and singular/error situations.

## Constraints rationale
Some combinations are meaningless or redundant. For example:
- `NULL_PATTERN` and `NULL_PATH` are singular situations, so only one focused test is needed for each.
- `URI_REGEX` only makes sense with URI-oriented paths.
- `CASE_VARIANT` only matters when case sensitivity is under study.

## Ready-to-run PICT command
### Windows PowerShell
```powershell
pict .\functional\model.pict > .\functional\pict-output.txt
