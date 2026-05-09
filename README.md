# hashira-kuljeet

C++ polynomial builder. Reads a JSON file containing `n` numerical roots
(each in a base from 2 to 16), converts the first `k` of them to integers,
then prints the coefficients of `(x − r₁)(x − r₂) ⋯ (x − r_k)`.

## Build

```sh
brew install boost              # macOS
# sudo apt-get install libboost-all-dev   # Debian/Ubuntu

make
```

The `Makefile` includes nlohmann/json + boost::multiprecision (for big-int
coefficients — see the testcase note below).

## Run

```sh
./main testcase1.json    # small input
./main testcase.json     # 21-digit base-6 numbers; needs big-int
make test                # runs both
```

## Input format

```json
{
  "keys": { "n": 7, "k": 7 },
  "1": { "base": "6",  "value": "13444211440455345511" },
  "2": { "base": "15", "value": "aed7015a346d635" },
  ...
}
```

## Notes

- Switched from `long long` to `boost::multiprecision::cpp_int` because the
  product of large roots produces 100+ digit coefficients — `long long`
  silently overflowed on `testcase.json`.
- The polynomial-times-`(x − r)` step uses the standard distributive
  expansion, no symbolic algebra dependency.
