so.version
==========

Compare [SemVer.org] described versions.


API
---

See [version.hpp](include/version.hpp).

- Construct from SemVer text.

  ```cpp
  static so::version::parse(std::string) -> so::version
  ```

  E.g.
  ```cpp
  auto v1 = so::version::parse("1.2.3-hello+linux");
  auto v2 = so::version::parse("1.2.3+world");
  ```

- Convert to SemVer text.

  ```cpp
  so::version::operator std::string()
  ```

- Compare 2 `so::version` objects.

  - `==`, `<`
  - `!=`, `<=`, `>`, `>=`

  E.g.
  ```cpp
  std::cout << std::boolalpha << (v1 < v2);
  ```
  > ```
  > true
  > ```

- Deduce related versions.

  ```cpp
  so::version::next_major() -> so::version
  so::version::next_minor() -> so::version
  so::version::next_patch() -> so::version
  so::version::release() -> so::version
  so::version::general() -> so::version
  ```

  Plus a handy way.

  ```cpp
  so::version::operator+(so::version::change) -> so::version
  ```

  > **Under The Hood!**
  > `so::version::change` is a bit flag enumeration.
  > Bitwise **or** `|` and **xor** `^` are available. E.g.
  > ```cpp
  > using change = so::version::change;
  > std::cout << std::string{v1 + (change::internal | change::compatible)};
  > ```
  >> ```
  >> 1.3.0+linux
  >> ```

- Trivial check.

  ```cpp
  so::is::stable(so::version) -> bool
  ```


License
-------
![LGPLv3]

Copyright (C) 2015  Iceberg YOUNG

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


---

[SemVer.org]: http://semver.org/spec/v2.0.0.html
"Semantic Versioning 2.0.0"

[LGPLv3]: http://www.gnu.org/graphics/lgplv3-88x31.png
"GNU Lesser General Public License version 3"
